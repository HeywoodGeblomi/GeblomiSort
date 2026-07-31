// GeblomiSort demo
#include "../GeblomiSort.hpp"
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>

int main() {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1'000'000);

    // Random
    {
        std::vector<int> v(100000);
        for (auto& x : v) x = dist(rng);
        auto t0 = std::chrono::high_resolution_clock::now();
        geblomi::sort(v.begin(), v.end());
        auto t1 = std::chrono::high_resolution_clock::now();
        bool ok = std::is_sorted(v.begin(), v.end());
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "Random 100k: " << ms << " ms, sorted=" << ok << "\n";
    }

    // Already sorted (early exit)
    {
        std::vector<int> v(100000);
        std::iota(v.begin(), v.end(), 0);
        auto t0 = std::chrono::high_resolution_clock::now();
        geblomi::sort(v.begin(), v.end());
        auto t1 = std::chrono::high_resolution_clock::now();
        bool ok = std::is_sorted(v.begin(), v.end());
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "Sorted 100k: " << ms << " ms, sorted=" << ok << "\n";
    }

    // Reverse
    {
        std::vector<int> v(100000);
        std::iota(v.begin(), v.end(), 0);
        std::reverse(v.begin(), v.end());
        auto t0 = std::chrono::high_resolution_clock::now();
        geblomi::sort(v.begin(), v.end());
        auto t1 = std::chrono::high_resolution_clock::now();
        bool ok = std::is_sorted(v.begin(), v.end());
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "Reverse 100k: " << ms << " ms, sorted=" << ok << "\n";
    }

    std::cout << "Demo OK\n";
    return 0;
}
