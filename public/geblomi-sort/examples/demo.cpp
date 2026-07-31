// GeblomiSort demo
// Compile: g++ -O3 -std=c++20 -I.. demo.cpp -o demo

#include "../GeblomiSort.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <numeric>

template <typename It>
bool is_sorted_asc(It first, It last) {
    return std::is_sorted(first, last);
}

int main() {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1'000'000);

    const size_t N = 1'000'000;
    std::vector<int> data(N);

    // 1. Random
    for (auto& x : data) x = dist(rng);
    auto t0 = std::chrono::high_resolution_clock::now();
    geblomi::sort(data.begin(), data.end());
    auto t1 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "Random (" << N << "): " << std::fixed << std::setprecision(2)
              << ms << " ms  sorted=" << is_sorted_asc(data.begin(), data.end()) << "\n";

    // 2. Already sorted
    std::iota(data.begin(), data.end(), 0);
    t0 = std::chrono::high_resolution_clock::now();
    geblomi::sort(data.begin(), data.end());
    t1 = std::chrono::high_resolution_clock::now();
    ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "Sorted  (" << N << "): " << ms << " ms  sorted="
              << is_sorted_asc(data.begin(), data.end()) << "\n";

    // 3. Reverse
    std::reverse(data.begin(), data.end());
    t0 = std::chrono::high_resolution_clock::now();
    geblomi::sort(data.begin(), data.end());
    t1 = std::chrono::high_resolution_clock::now();
    ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "Reverse (" << N << "): " << ms << " ms  sorted="
              << is_sorted_asc(data.begin(), data.end()) << "\n";

    // 4. Custom comparator (descending)
    for (auto& x : data) x = dist(rng);
    t0 = std::chrono::high_resolution_clock::now();
    geblomi::sort(data.begin(), data.end(), std::greater<>{});
    t1 = std::chrono::high_resolution_clock::now();
    ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    bool desc_ok = std::is_sorted(data.begin(), data.end(), std::greater<>{});
    std::cout << "Greater (" << N << "): " << ms << " ms  sorted_desc=" << desc_ok << "\n";

    // 5. Patterned (many short runs)
    for (size_t i = 0; i < N; ++i) data[i] = static_cast<int>((i / 100) % 1000);
    t0 = std::chrono::high_resolution_clock::now();
    geblomi::sort(data.begin(), data.end());
    t1 = std::chrono::high_resolution_clock::now();
    ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "Pattern (" << N << "): " << ms << " ms  sorted="
              << is_sorted_asc(data.begin(), data.end()) << "\n";

    std::cout << "\nAll demos completed successfully.\n";
    return 0;
}
