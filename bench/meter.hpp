#pragma once

// Bench-only meter. The product header does not include this.
// CountLess is a std::less-shaped comparator. meter_header.py teaches the
// bench copy of the sorter to route it like std::less<>. cmp counts element
// comparisons (comparator calls plus ska_sort's std::sort fallback). digit
// counts radix byte reads (current_byte) on the ska_sort route.

#include <cstdint>
#include <type_traits>

namespace geb_meter {

inline std::uint64_t cmp = 0;
inline std::uint64_t digit = 0;

struct CountLess {
    template <class A, class B>
    bool operator()(const A& a, const B& b) const {
        ++cmp;
        return a < b;
    }
};

template <class C>
struct is_counting_less : std::false_type {};

template <>
struct is_counting_less<CountLess> : std::true_type {};

inline void reset() {
    cmp = 0;
    digit = 0;
}

}  // namespace geb_meter
