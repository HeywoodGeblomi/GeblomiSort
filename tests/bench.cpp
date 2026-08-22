// GEB-ACE-001 Phase B — Charged bench
// N=1e6, best-of-3 wall time. Compare geblomi::sort / std::sort / pdqsort / ska_sort.
// Build: g++ -O2 -std=c++20 -I public/geblomi-sort tests/bench.cpp -o bench && ./bench
// Soft@1.20 vs pdq is informational — CI does not fail on losses. Correctness is hard-fail (Phase A).

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

std::vector<int> make_random(size_t n, uint32_t seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 1'000'000);
    std::vector<int> v(n);
    for (auto& x : v) x = dist(rng);
    return v;
}
std::vector<int> make_sorted(size_t n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    return v;
}
std::vector<int> make_reverse(size_t n) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = static_cast<int>(n - 1 - i);
    return v;
}
std::vector<int> make_patterned(size_t n) {
    std::vector<int> v(n);
    size_t half = n / 2;
    for (size_t i = 0; i < half; ++i) v[i] = static_cast<int>(i);
    for (size_t i = half; i < n; ++i) v[i] = static_cast<int>(n - 1 - i);
    return v;
}
std::vector<int> make_sawtooth(size_t n) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = static_cast<int>(i % 64);
    return v;
}

template <typename F>
double time_ms(F&& fn, int reps = 3) {
    double best = 1e300;
    for (int r = 0; r < reps; ++r) {
        auto t0 = Clock::now();
        fn();
        auto t1 = Clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        if (ms < best) best = ms;
    }
    return best;
}

struct Row {
    std::string dist;
    double geblomi_ms;
    double std_ms;
    double pdq_ms;
    double ska_ms;
};

const char* verdict(double geblomi, double other, double soft = 1.20) {
    if (other <= 0) return "?";
    double ratio = geblomi / other;
    if (ratio <= 1.0 / soft) return "win";
    if (ratio >= soft) return "loss";
    return "tie";
}

void run_dist(const char* name, const std::vector<int>& base, std::vector<Row>& rows) {
    auto t_geblomi = time_ms([&] {
        auto v = base;
        geblomi::sort(v.begin(), v.end());
    });
    auto t_std = time_ms([&] {
        auto v = base;
        std::sort(v.begin(), v.end());
    });
    auto t_pdq = time_ms([&] {
        auto v = base;
        pdqsort(v.begin(), v.end());
    });
    auto t_ska = time_ms([&] {
        auto v = base;
        ska_sort(v.begin(), v.end());
    });
    rows.push_back({name, t_geblomi, t_std, t_pdq, t_ska});
}

}  // namespace

int main() {
    constexpr size_t N = 1'000'000;
    std::cout << "GEB-ACE-001 Phase B charged bench  N=" << N << "  best-of-3  g++ -O2\n";
    std::cout << std::fixed << std::setprecision(2);

    std::vector<Row> rows;
    run_dist("random", make_random(N), rows);
    run_dist("sorted", make_sorted(N), rows);
    run_dist("reverse", make_reverse(N), rows);
    run_dist("patterned", make_patterned(N), rows);
    run_dist("sawtooth", make_sawtooth(N), rows);

    std::cout << "\n"
              << std::left << std::setw(12) << "dist"
              << std::right
              << std::setw(12) << "geblomi"
              << std::setw(12) << "std::sort"
              << std::setw(12) << "pdqsort"
              << std::setw(12) << "ska_sort"
              << std::setw(10) << "vs_pdq"
              << std::setw(10) << "vs_ska"
              << std::setw(10) << "vs_std"
              << "\n";

    int soft_losses_pdq = 0;
    for (const auto& r : rows) {
        const char* vp = verdict(r.geblomi_ms, r.pdq_ms);
        const char* vs = verdict(r.geblomi_ms, r.ska_ms);
        const char* vstd = verdict(r.geblomi_ms, r.std_ms);
        if (std::string(vp) == "loss") ++soft_losses_pdq;

        std::cout << std::left << std::setw(12) << r.dist
                  << std::right
                  << std::setw(12) << r.geblomi_ms
                  << std::setw(12) << r.std_ms
                  << std::setw(12) << r.pdq_ms
                  << std::setw(12) << r.ska_ms
                  << std::setw(10) << vp
                  << std::setw(10) << vs
                  << std::setw(10) << vstd
                  << "\n";
    }

    std::cout << "\nsoft@1.20 losses vs pdq: " << soft_losses_pdq << " / " << rows.size() << "\n";
    std::cout << "NOTE: losses are reported, not CI-fail. Phase A correctness is the hard gate.\n";
    return 0;
}
