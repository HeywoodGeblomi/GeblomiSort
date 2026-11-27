// GeblomiSort/bench route counter. Unpublished bench. Not a filed title.
// Frozen triple: pdqsort, libstdc++ std::sort, geblomi::sort.
// geblomi_full measures the plain std::less<> route users take.
// CountLess is routed like std::less<> in the bench-only header copy.
// d_geb counts ska_sort digit reads. c_geb_fb is the custom-comparator fallback.
// Product header is not modified. n=1024. Seeds 1..30. ns descriptive only.
// Every sort is checked against std::sort's output.

#include "metered/GeblomiSort.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr int N = 1024;
constexpr int PROBE = 32;

struct CountComp {
    std::uint64_t* n;
    bool operator()(std::int64_t a, std::int64_t b) const {
        ++*n;
        return a < b;
    }
};

std::vector<std::int64_t> make_array(const std::string& gen, std::uint64_t seed) {
    std::mt19937_64 rng(seed);
    std::vector<std::int64_t> a(N);
    if (gen == "random") {
        for (int i = 0; i < N; ++i) a[i] = static_cast<std::int64_t>(rng() & 0xFFFFFFFFull);
    } else if (gen == "already-sorted") {
        for (int i = 0; i < N; ++i) a[i] = i;
    } else if (gen == "reverse") {
        for (int i = 0; i < N; ++i) a[i] = N - 1 - i;
    } else if (gen == "organ-pipe") {
        for (int i = 0; i < N; ++i) a[i] = (i < N / 2) ? i : (N - 1 - i);
    } else if (gen == "two-value") {
        for (int i = 0; i < N; ++i) a[i] = static_cast<std::int64_t>(rng() & 1ull);
    } else if (gen == "noisy_ramp") {
        for (int i = 0; i < N; ++i)
            a[i] = static_cast<std::int64_t>(i) + static_cast<std::int64_t>(rng() % 31) - 15;
    } else if (gen == "biased_walk") {
        std::int64_t v = 0;
        for (int i = 0; i < N; ++i) {
            a[i] = v;
            v += static_cast<std::int64_t>(rng() % 5) - 1;
        }
    } else {
        std::cerr << "unknown generator: " << gen << "\n";
        std::exit(1);
    }
    return a;
}

struct TimedCount {
    std::uint64_t cmps = 0;
    std::uint64_t ns = 0;
};

void require_sorted(const char* what, const char* gen, int seed,
                    const std::vector<std::int64_t>& got,
                    const std::vector<std::int64_t>& ref) {
    if (got != ref) {
        std::cerr << "sorted-output check failed: " << what
                  << " gen=" << gen << " seed=" << seed << "\n";
        std::exit(1);
    }
}

template <typename Fn>
TimedCount time_count_sorted(const std::vector<std::int64_t>& src,
                             const std::vector<std::int64_t>& ref,
                             const char* what, const char* gen, int seed, Fn fn) {
    auto copy = src;
    std::uint64_t n = 0;
    const auto t0 = std::chrono::steady_clock::now();
    fn(copy, n);
    const auto t1 = std::chrono::steady_clock::now();
    require_sorted(what, gen, seed, copy, ref);
    return TimedCount{n, static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count())};
}

const char* route_name(geblomi::Route route) {
    switch (route) {
    case geblomi::Route::Small: return "small";
    case geblomi::Route::Sorted: return "sorted";
    case geblomi::Route::Reverse: return "reverse";
    case geblomi::Route::Patterned: return "patterned";
    case geblomi::Route::Random: return "random";
    }
    return "unknown";
}

const char* GENS[] = {
    "random", "already-sorted", "reverse", "organ-pipe",
    "two-value", "noisy_ramp", "biased_walk"};

const char* star_of(std::uint64_t c_pdq, std::uint64_t c_std, std::uint64_t c_geb) {
    std::uint64_t best = c_pdq;
    const char* name = "pdq_full";
    if (c_std < best) {
        best = c_std;
        name = "std_sort_full";
    }
    if (c_geb < best) {
        name = "geblomi_full";
    }
    return name;
}

}  // namespace

int main() {
    std::cout << "generator,seed";
    for (int i = 0; i < PROBE; ++i) std::cout << ",a" << i;
    std::cout << ",c_pdq,c_std,c_geb,d_geb,c_geb_fb"
                 ",ns_pdq,ns_std,ns_geb,ns_geb_fb,geb_route,r_star\n";

    for (const char* gen : GENS) {
        for (int seed = 1; seed <= 30; ++seed) {
            auto s = make_array(gen, static_cast<std::uint64_t>(seed));
            auto ref = s;
            std::sort(ref.begin(), ref.end());

            const char* route = route_name(geblomi::probe(s.begin(), s.end()).route);

            auto pdq = time_count_sorted(s, ref, "pdq_full", gen, seed,
                [](auto& copy, std::uint64_t& n) {
                    pdqsort(copy.begin(), copy.end(), CountComp{&n});
                });
            auto stds = time_count_sorted(s, ref, "std_sort_full", gen, seed,
                [](auto& copy, std::uint64_t& n) {
                    std::sort(copy.begin(), copy.end(), CountComp{&n});
                });

            // Custom comparator: not std::less, so random scalars take pdqsort.
            geb_meter::reset();
            auto fb = time_count_sorted(s, ref, "geblomi_fallback", gen, seed,
                [](auto& copy, std::uint64_t& n) {
                    geblomi::sort(copy.begin(), copy.end(), CountComp{&n});
                });
            if (geb_meter::digit != 0 || geb_meter::cmp != 0) {
                std::cerr << "fallback route touched the radix meter: gen="
                          << gen << " seed=" << seed << "\n";
                std::exit(1);
            }

            // Plain std::less<> route. CountLess is routed like std::less<>.
            geb_meter::reset();
            auto geb = time_count_sorted(s, ref, "geblomi_full", gen, seed,
                [](auto& copy, std::uint64_t& n) {
                    geblomi::sort(copy.begin(), copy.end(), geb_meter::CountLess{});
                    n = geb_meter::cmp;
                });
            const std::uint64_t digits = geb_meter::digit;
            if (std::string(route) == "random") {
                if (digits == 0) {
                    std::cerr << "random std::less route took no digit reads: gen="
                              << gen << " seed=" << seed << "\n";
                    std::exit(1);
                }
            } else if (digits != 0) {
                std::cerr << "non-random route took digit reads: gen="
                          << gen << " seed=" << seed << " route=" << route << "\n";
                std::exit(1);
            }

            {
                auto plain = s;
                geblomi::sort(plain.begin(), plain.end());
                require_sorted("geblomi_std_less", gen, seed, plain, ref);
            }

            const char* star = star_of(pdq.cmps, stds.cmps, geb.cmps);
            std::cout << gen << "," << seed;
            for (int i = 0; i < PROBE; ++i) std::cout << "," << s[i];
            std::cout << "," << pdq.cmps << "," << stds.cmps << "," << geb.cmps
                      << "," << digits << "," << fb.cmps
                      << "," << pdq.ns << "," << stds.ns << "," << geb.ns
                      << "," << fb.ns << "," << route << "," << star << "\n";
        }
    }
    return 0;
}
