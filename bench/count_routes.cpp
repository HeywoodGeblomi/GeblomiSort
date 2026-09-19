// GeblomiSort/bench route counter. Unpublished bench. Not a filed title.
// Frozen triple: pdqsort, libstdc++ std::sort, geblomi::sort.
// Meter: comparator invocations. n=1024. ns descriptive only.

#include "../GeblomiSort.hpp"

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

template <typename Fn>
TimedCount time_count(const std::vector<std::int64_t>& src, Fn fn) {
    auto copy = src;
    std::uint64_t n = 0;
    const auto t0 = std::chrono::steady_clock::now();
    fn(copy, n);
    const auto t1 = std::chrono::steady_clock::now();
    return TimedCount{n, static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count())};
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
    std::cout << ",c_pdq,c_std,c_geb,ns_pdq,ns_std,ns_geb,r_star\n";

    for (const char* gen : GENS) {
        for (int seed = 1; seed <= 30; ++seed) {
            auto s = make_array(gen, static_cast<std::uint64_t>(seed));
            auto pdq = time_count(s, [](auto& copy, std::uint64_t& n) {
                pdqsort(copy.begin(), copy.end(), CountComp{&n});
            });
            auto stds = time_count(s, [](auto& copy, std::uint64_t& n) {
                std::sort(copy.begin(), copy.end(), CountComp{&n});
            });
            auto geb = time_count(s, [](auto& copy, std::uint64_t& n) {
                geblomi::sort(copy.begin(), copy.end(), CountComp{&n});
            });
            const char* star = star_of(pdq.cmps, stds.cmps, geb.cmps);
            std::cout << gen << "," << seed;
            for (int i = 0; i < PROBE; ++i) std::cout << "," << s[i];
            std::cout << "," << pdq.cmps << "," << stds.cmps << "," << geb.cmps
                      << "," << pdq.ns << "," << stds.ns << "," << geb.ns
                      << "," << star << "\n";
        }
    }
    return 0;
}
