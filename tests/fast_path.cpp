// Sorted / reversed fast paths must look at every adjacent pair.
// A stride of n/64 (n > 4096) used to accept almost-sorted and
// almost-reversed inputs and return them unsorted.
// Build: g++ -O2 -std=c++20 -I public/geblomi-sort tests/fast_path.cpp -o fast_path && ./fast_path

#include "GeblomiSort.hpp"

#include <algorithm>
#include <cstdio>
#include <numeric>
#include <string>
#include <vector>

namespace {

int g_failures = 0;

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

void fail(const std::string& name, const char* why) {
    ++g_failures;
    std::fprintf(stderr, "FAIL %s: %s\n", name.c_str(), why);
}

void expect_sorted_output(const std::string& name, std::vector<long long> v) {
    auto oracle = v;
    std::sort(oracle.begin(), oracle.end());
    geblomi::sort(v.begin(), v.end());
    if (!std::is_sorted(v.begin(), v.end()) || v != oracle) {
        fail(name, "output is not the sorted permutation");
    }
}

void expect_route(const std::string& name, const std::vector<long long>& v,
                  geblomi::Route want) {
    const auto got = geblomi::probe(v.begin(), v.end()).route;
    if (got != want) {
        std::fprintf(stderr, "FAIL %s: route %s, want %s\n",
                     name.c_str(), route_name(got), route_name(want));
        ++g_failures;
    }
}

void expect_not_route(const std::string& name, const std::vector<long long>& v,
                      geblomi::Route bad) {
    const auto got = geblomi::probe(v.begin(), v.end()).route;
    if (got == bad) {
        std::fprintf(stderr, "FAIL %s: route %s must not be taken\n",
                     name.c_str(), route_name(got));
        ++g_failures;
    }
}

std::vector<long long> sorted_n(std::size_t n) {
    std::vector<long long> v(n);
    std::iota(v.begin(), v.end(), 0);
    return v;
}

std::vector<long long> reversed_n(std::size_t n) {
    std::vector<long long> v(n);
    for (std::size_t i = 0; i < n; ++i) v[i] = static_cast<long long>(n - 1 - i);
    return v;
}

void check_clean(std::size_t n) {
    const std::string ns = "n=" + std::to_string(n);
    {
        auto v = sorted_n(n);
        expect_route(ns + "/sorted", v, geblomi::Route::Sorted);
        expect_sorted_output(ns + "/sorted", v);
    }
    {
        auto v = reversed_n(n);
        expect_route(ns + "/reversed", v, geblomi::Route::Reverse);
        expect_sorted_output(ns + "/reversed", v);
    }
    {
        std::vector<long long> v(n, 7);
        expect_route(ns + "/all-equal", v, geblomi::Route::Sorted);
        expect_sorted_output(ns + "/all-equal", v);
    }
    {
        // Strictly decreasing head, then one equal pair in the tail.
        // Non-increasing over every pair, so the reverse path is valid.
        auto v = reversed_n(n);
        v[1000] = v[999];
        expect_route(ns + "/reverse-equal-run", v, geblomi::Route::Reverse);
        expect_sorted_output(ns + "/reverse-equal-run", v);
    }
}

void check_swap(std::size_t n, std::size_t i, const char* kind) {
    auto v = (std::string(kind) == "almost-sorted") ? sorted_n(n) : reversed_n(n);
    std::swap(v[i], v[i + 1]);
    const std::string name = "n=" + std::to_string(n) + "/" + kind +
                             "/swap=" + std::to_string(i);
    expect_not_route(name, v, geblomi::Route::Sorted);
    expect_not_route(name, v, geblomi::Route::Reverse);
    expect_sorted_output(name, v);
}

}  // namespace

int main() {
    const std::size_t sizes[] = {4097, 100000, 10000000};
    for (std::size_t n : sizes) {
        check_clean(n);
        const std::size_t swaps[] = {0, 1000, 2000, n / 2, n - 2};
        for (std::size_t i : swaps) {
            check_swap(n, i, "almost-sorted");
            check_swap(n, i, "almost-reversed");
        }
    }

    // The reported repro: both lines must come out sorted.
    {
        const std::size_t n = 100000;
        auto a = sorted_n(n);
        std::swap(a[1000], a[1001]);
        geblomi::sort(a.begin(), a.end());
        std::printf("almost-sorted: %d\n", (int)std::is_sorted(a.begin(), a.end()));
        if (!std::is_sorted(a.begin(), a.end())) fail("repro/almost-sorted", "not sorted");

        auto b = reversed_n(n);
        std::swap(b[1000], b[1001]);
        geblomi::sort(b.begin(), b.end());
        std::printf("almost-reversed: %d\n", (int)std::is_sorted(b.begin(), b.end()));
        if (!std::is_sorted(b.begin(), b.end())) fail("repro/almost-reversed", "not sorted");
    }

    if (g_failures != 0) {
        std::fprintf(stderr, "fast_path failures=%d\n", g_failures);
        return 1;
    }
    std::printf("fast_path OK\n");
    return 0;
}
