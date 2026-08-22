// GEB-ACE-002 Phase B+ — Pinned charged bench
// Generators fixed. N=1e6. Three independent trials; print each + mean/range.
// Soft@1.20 vs pdq/ska is informational — CI does not fail on losses.
// Build: g++ -O2 -std=c++20 -I public/geblomi-sort tests/bench.cpp -o bench && ./bench

#include "GeblomiSort.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

// ---- pinned generators (GEB-ACE-002) ----
// N always 1'000'000. Seeds and definitions must not change without a new ticket.
constexpr size_t kN = 1'000'000;
constexpr uint32_t kRandomSeed = 42;
constexpr int kSawtoothMod = 64;  // v[i] = i % 64

std::vector<int> make_random(size_t n = kN) {
    std::mt19937 rng(kRandomSeed);
    std::uniform_int_distribution<int> dist(0, 1'000'000);
    std::vector<int> v(n);
    for (auto& x : v) x = dist(rng);
    return v;
}
std::vector<int> make_sorted(size_t n = kN) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    return v;
}
std::vector<int> make_reverse(size_t n = kN) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = static_cast<int>(n - 1 - i);
    return v;
}
// Patterned = organpipe: ascend [0..half), descend thereafter.
std::vector<int> make_patterned(size_t n = kN) {
    std::vector<int> v(n);
    size_t half = n / 2;
    for (size_t i = 0; i < half; ++i) v[i] = static_cast<int>(i);
    for (size_t i = half; i < n; ++i) v[i] = static_cast<int>(n - 1 - i);
    return v;
}
std::vector<int> make_sawtooth(size_t n = kN) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = static_cast<int>(i % kSawtoothMod);
    return v;
}

// Discard first call as warmup, then best-of-3.
template <typename F>
double time_ms_best3(F&& fn) {
    fn();  // warmup (discarded)
    double best = 1e300;
    for (int r = 0; r < 3; ++r) {
        auto t0 = Clock::now();
        fn();
        auto t1 = Clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        if (ms < best) best = ms;
    }
    return best;
}

struct Times {
    double geblomi = 0, stds = 0, pdq = 0, ska = 0;
};

Times measure_one(const std::vector<int>& base) {
    Times t;
    t.geblomi = time_ms_best3([&] {
        auto v = base;
        geblomi::sort(v.begin(), v.end());
    });
    t.stds = time_ms_best3([&] {
        auto v = base;
        std::sort(v.begin(), v.end());
    });
    t.pdq = time_ms_best3([&] {
        auto v = base;
        pdqsort(v.begin(), v.end());
    });
    t.ska = time_ms_best3([&] {
        auto v = base;
        ska_sort(v.begin(), v.end());
    });
    return t;
}

const char* verdict(double geblomi, double other, double soft = 1.20) {
    if (other <= 0) return "?";
    double ratio = geblomi / other;
    if (ratio <= 1.0 / soft) return "win";
    if (ratio >= soft) return "loss";
    return "tie";
}

struct DistSpec {
    const char* name;
    std::vector<int> (*make)(size_t);
};

}  // namespace

int main() {
    const DistSpec dists[] = {
        {"random", make_random},
        {"sorted", make_sorted},
        {"reverse", make_reverse},
        {"patterned", make_patterned},
        {"sawtooth", make_sawtooth},
    };
    constexpr int kTrials = 3;

    std::cout << "GEB-ACE-002 Phase B+ pinned bench  N=" << kN
              << "  trials=" << kTrials << "  best-of-3+warmup  seed=" << kRandomSeed
              << "  sawtooth_mod=" << kSawtoothMod << "\n";
    std::cout << std::fixed << std::setprecision(2);

    for (const auto& d : dists) {
        auto base = d.make(kN);
        std::vector<Times> trials;
        trials.reserve(kTrials);
        for (int i = 0; i < kTrials; ++i) trials.push_back(measure_one(base));

        auto mean = [&](auto mem) {
            double s = 0;
            for (const auto& t : trials) s += t.*mem;
            return s / kTrials;
        };
        auto mn = [&](auto mem) {
            double m = 1e300;
            for (const auto& t : trials) m = std::min(m, t.*mem);
            return m;
        };
        auto mx = [&](auto mem) {
            double m = 0;
            for (const auto& t : trials) m = std::max(m, t.*mem);
            return m;
        };

        double g_mean = mean(&Times::geblomi);
        double p_mean = mean(&Times::pdq);
        double s_mean = mean(&Times::ska);
        double st_mean = mean(&Times::stds);

        std::cout << "\n=== " << d.name << " ===\n";
        for (int i = 0; i < kTrials; ++i) {
            const auto& t = trials[i];
            std::cout << "  trial" << (i + 1)
                      << "  geblomi=" << t.geblomi
                      << "  std=" << t.stds
                      << "  pdq=" << t.pdq
                      << "  ska=" << t.ska
                      << "  vs_pdq=" << verdict(t.geblomi, t.pdq)
                      << "  vs_ska=" << verdict(t.geblomi, t.ska)
                      << "\n";
        }
        std::cout << "  mean     geblomi=" << g_mean
                  << "  std=" << st_mean
                  << "  pdq=" << p_mean
                  << "  ska=" << s_mean
                  << "  vs_pdq=" << verdict(g_mean, p_mean)
                  << "  vs_ska=" << verdict(g_mean, s_mean)
                  << "\n";
        std::cout << "  range    geblomi=[" << mn(&Times::geblomi) << "," << mx(&Times::geblomi) << "]"
                  << "  pdq=[" << mn(&Times::pdq) << "," << mx(&Times::pdq) << "]"
                  << "  ska=[" << mn(&Times::ska) << "," << mx(&Times::ska) << "]\n";

        int pdq_agree = 0, ska_agree = 0;
        const char* vp_mean = verdict(g_mean, p_mean);
        const char* vs_mean = verdict(g_mean, s_mean);
        for (const auto& t : trials) {
            if (std::string(verdict(t.geblomi, t.pdq)) == vp_mean) ++pdq_agree;
            if (std::string(verdict(t.geblomi, t.ska)) == vs_mean) ++ska_agree;
        }
        std::cout << "  stable   vs_pdq=" << (pdq_agree == kTrials ? "YES" : "NO")
                  << "(" << pdq_agree << "/" << kTrials << ")"
                  << "  vs_ska=" << (ska_agree == kTrials ? "YES" : "NO")
                  << "(" << ska_agree << "/" << kTrials << ")\n";
    }

    std::cout << "\nNOTE: losses/UNSTABLE reported, not CI-fail. Correctness is the hard gate.\n";
    return 0;
}
