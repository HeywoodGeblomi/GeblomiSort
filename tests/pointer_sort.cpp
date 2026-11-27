// geblomi::sort(T*, T*) must reach the real implementation.
// The pointer convenience overload used to call itself.
// Build: g++ -O2 -std=c++20 -I public/geblomi-sort tests/pointer_sort.cpp -o pointer_sort && ./pointer_sort

#include "GeblomiSort.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <vector>

namespace {

int g_failures = 0;

void fail(const char* name) {
    ++g_failures;
    std::fprintf(stderr, "FAIL %s\n", name);
}

void check_ptr(const char* name, std::vector<long long> v) {
    auto oracle = v;
    std::sort(oracle.begin(), oracle.end());
    geblomi::sort(v.data(), v.data() + v.size());
    if (!std::is_sorted(v.begin(), v.end()) || v != oracle) fail(name);
}

std::vector<long long> pattern(std::size_t n) {
    std::vector<long long> v(n);
    for (std::size_t i = 0; i < n; ++i) {
        v[i] = static_cast<long long>((i * 2654435761ull) % (n * 2 + 3))
             - static_cast<long long>(i % 7);
    }
    return v;
}

}  // namespace

int main() {
    std::vector<long long> v = {3, 1, 2};
    geblomi::sort(v.begin(), v.end());
    if (!std::is_sorted(v.begin(), v.end())) fail("iter {3,1,2}");
    std::puts("iter ok");

    v = {3, 1, 2};
    geblomi::sort(v.data(), v.data() + v.size());
    if (v != std::vector<long long>({1, 2, 3})) fail("ptr {3,1,2}");
    std::puts("ptr ok");

    check_ptr("ptr n=0", {});
    check_ptr("ptr n=1", {42});
    check_ptr("ptr n=2", {2, 1});

    const std::size_t sizes[] = {3, 17, 64, 4096, 4097, 100000};
    for (std::size_t n : sizes) {
        const std::string name = "ptr pattern n=" + std::to_string(n);
        check_ptr(name.c_str(), pattern(n));

        auto sorted = pattern(n);
        std::sort(sorted.begin(), sorted.end());
        check_ptr(("ptr sorted n=" + std::to_string(n)).c_str(), sorted);

        auto rev = sorted;
        std::reverse(rev.begin(), rev.end());
        check_ptr(("ptr reversed n=" + std::to_string(n)).c_str(), rev);
    }

    // Pointers must hit the same full scan as iterators.
    {
        const std::size_t n = 100000;
        std::vector<long long> a(n);
        for (std::size_t i = 0; i < n; ++i) a[i] = static_cast<long long>(i);
        std::swap(a[1000], a[1001]);
        check_ptr("ptr almost-sorted n=100000", a);

        std::vector<long long> b(n);
        for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<long long>(n - 1 - i);
        std::swap(b[1000], b[1001]);
        check_ptr("ptr almost-reversed n=100000", b);
    }

    if (g_failures != 0) {
        std::fprintf(stderr, "pointer_sort failures=%d\n", g_failures);
        return 1;
    }
    std::printf("pointer_sort OK\n");
    return 0;
}
