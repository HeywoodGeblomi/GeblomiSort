// GEB-ACE-001 Phase A — Correctness harness
// Oracle: std::sort with same comparator. Assert sorted + permutation of input.
// Build: g++ -O2 -std=c++20 -I public/geblomi-sort tests/correctness.cpp -o correctness && ./correctness
// Exit 0 on every fixture class × comparator × size.

#include "GeblomiSort.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace {

int g_failures = 0;
int g_cases = 0;

template <typename T, typename Comp>
bool is_permutation_of(std::vector<T> a, std::vector<T> b, Comp comp) {
    if (a.size() != b.size()) return false;
    std::sort(a.begin(), a.end(), comp);
    std::sort(b.begin(), b.end(), comp);
    return a == b;
}

template <typename T, typename Comp>
void check_one(const char* name, std::vector<T> input, Comp comp) {
    ++g_cases;
    std::vector<T> geblomi_copy = input;
    std::vector<T> oracle = input;

    geblomi::sort(geblomi_copy.begin(), geblomi_copy.end(), comp);
    std::sort(oracle.begin(), oracle.end(), comp);

    const bool sorted_ok = std::is_sorted(geblomi_copy.begin(), geblomi_copy.end(), comp);
    const bool equal_ok = (geblomi_copy == oracle);
    const bool perm_ok = is_permutation_of(input, geblomi_copy, comp);

    if (!sorted_ok || !equal_ok || !perm_ok) {
        ++g_failures;
        std::cerr << "FAIL " << name
                  << " n=" << input.size()
                  << " sorted=" << sorted_ok
                  << " equal_oracle=" << equal_ok
                  << " permutation=" << perm_ok << "\n";
    }
}

// ---------- fixtures (ints) ----------
std::vector<int> fix_empty() { return {}; }
std::vector<int> fix_one() { return {42}; }
std::vector<int> fix_sorted(size_t n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    return v;
}
std::vector<int> fix_reverse(size_t n) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = static_cast<int>(n - 1 - i);
    return v;
}
std::vector<int> fix_all_equal(size_t n) { return std::vector<int>(n, 7); }
std::vector<int> fix_two_unique(size_t n) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (i % 2 == 0) ? 1 : 2;
    return v;
}
std::vector<int> fix_random(size_t n, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 1'000'000);
    std::vector<int> v(n);
    for (auto& x : v) x = dist(rng);
    return v;
}
std::vector<int> fix_sawtooth(size_t n) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = static_cast<int>(i % 64);
    return v;
}
std::vector<int> fix_organpipe(size_t n) {
    std::vector<int> v(n);
    size_t half = n / 2;
    for (size_t i = 0; i < half; ++i) v[i] = static_cast<int>(i);
    for (size_t i = half; i < n; ++i) v[i] = static_cast<int>(n - 1 - i);
    return v;
}
std::vector<int> fix_few_unique(size_t n, int k) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = static_cast<int>(i % static_cast<size_t>(k));
    return v;
}
std::vector<int> fix_adversarial_reverse_segments(size_t n) {
    std::vector<int> v(n);
    const size_t seg = 64;
    for (size_t i = 0; i < n; ++i) {
        size_t block = i / seg;
        size_t pos = i % seg;
        size_t base = block * seg;
        size_t len = std::min(seg, n - base);
        v[i] = static_cast<int>(base + (len - 1 - pos));
    }
    return v;
}

using Pair = std::pair<int, int>;
std::vector<Pair> fix_pairs_random(size_t n, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 1000);
    std::vector<Pair> v(n);
    for (auto& p : v) p = {dist(rng), dist(rng)};
    return v;
}

template <typename Comp>
void run_int_suite(const char* comp_name, Comp comp) {
    const size_t sizes[] = {0, 1, 64, 1000, 100000};

    for (size_t n : sizes) {
        auto tag = [&](const char* fix) {
            return std::string(comp_name) + "/" + fix + "/n=" + std::to_string(n);
        };

        if (n == 0) {
            check_one(tag("empty").c_str(), fix_empty(), comp);
            continue;
        }
        if (n == 1) {
            check_one(tag("one").c_str(), fix_one(), comp);
            continue;
        }

        check_one(tag("sorted").c_str(), fix_sorted(n), comp);
        check_one(tag("reverse").c_str(), fix_reverse(n), comp);
        check_one(tag("all_equal").c_str(), fix_all_equal(n), comp);
        check_one(tag("two_unique").c_str(), fix_two_unique(n), comp);
        check_one(tag("random").c_str(), fix_random(n, 42u + static_cast<uint32_t>(n)), comp);
        check_one(tag("sawtooth").c_str(), fix_sawtooth(n), comp);
        check_one(tag("organpipe").c_str(), fix_organpipe(n), comp);
        check_one(tag("few_unique_k4").c_str(), fix_few_unique(n, 4), comp);
        check_one(tag("few_unique_k16").c_str(), fix_few_unique(n, 16), comp);
        check_one(tag("adv_rev_seg").c_str(), fix_adversarial_reverse_segments(n), comp);
    }
}

void run_pair_suite() {
    const size_t sizes[] = {0, 1, 64, 1000};
    auto less_pair = [](const Pair& a, const Pair& b) { return a < b; };
    auto greater_pair = [](const Pair& a, const Pair& b) { return a > b; };

    for (size_t n : sizes) {
        std::string base = "pair/n=" + std::to_string(n);
        if (n == 0) {
            check_one((base + "/empty/less").c_str(), std::vector<Pair>{}, less_pair);
            check_one((base + "/empty/greater").c_str(), std::vector<Pair>{}, greater_pair);
            continue;
        }
        auto v = fix_pairs_random(n, 7u + static_cast<uint32_t>(n));
        check_one((base + "/random/less").c_str(), v, less_pair);
        check_one((base + "/random/greater").c_str(), v, greater_pair);
    }
}

}  // namespace

int main() {
    std::cout << "GEB-ACE-001 Phase A correctness harness\n";

    run_int_suite("less", std::less<int>{});
    run_int_suite("greater", std::greater<int>{});
    run_pair_suite();

    std::cout << "cases=" << g_cases << " failures=" << g_failures << "\n";
    if (g_failures == 0) {
        std::cout << "ALL_CORRECTNESS_OK=1\n";
        return 0;
    }
    std::cout << "ALL_CORRECTNESS_OK=0\n";
    return 1;
}
