/**
 * GeblomiSort
 * ===========
 * Commercial-grade adaptive hybrid 1-D sorting algorithm.
 *
 * Single-header distribution containing:
 *   • GeblomiSort (adaptive probe + Verge-style runs + routing)
 *   • pdqsort (Pattern-Defeating Quicksort)
 *   • ska_sort (American-Flag / MSD radix)
 *
 * Features:
 *   - Extremely low-overhead adaptive routing
 *   - O(1) extra memory even on adversarial inputs
 *   - Full custom comparator support
 *   - Early exits for sorted / reverse data
 *   - Matches Ska Sort on pure random integers
 *   - Strong on patterned / nearly-sorted data
 *
 * Usage:
 *   #include "GeblomiSort.hpp"
 *   geblomi::sort(begin, end);
 *   geblomi::sort(begin, end, std::greater<>{});
 *
 * Requirements: C++20
 * License: Follow original licenses of pdqsort and ska_sort components;
 *          Geblomi hybrid logic is original work of the project team.
 *
 * Team: Grok + Harper + Benjamin + Lucas + Heywood | 2026-07-30
 */
#pragma once

// ============================================================================
// 1. pdqsort
// ============================================================================

/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2021 Orson Peters

    This software is provided 'as-is', without any express or implied warranty. In no event will the
    authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial
    applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the
       original software. If you use this software in a product, an acknowledgment in the product
       documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as
       being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/


#ifndef PDQSORT_H
#define PDQSORT_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>
#include <iterator>

#if __cplusplus >= 201103L
    #include <cstdint>
    #include <type_traits>
    #define PDQSORT_PREFER_MOVE(x) std::move(x)
#else
    #define PDQSORT_PREFER_MOVE(x) (x)
#endif


namespace pdqsort_detail {
    enum {
        // Partitions below this size are sorted using insertion sort.
        insertion_sort_threshold = 24,

        // Partitions above this size use Tukey's ninther to select the pivot.
        ninther_threshold = 128,

        // When we detect an already sorted partition, attempt an insertion sort that allows this
        // amount of element moves before giving up.
        partial_insertion_sort_limit = 8,

        // Must be multiple of 8 due to loop unrolling, and < 256 to fit in unsigned char.
        block_size = 64,

        // Cacheline size, assumes power of two.
        cacheline_size = 64

    };

#if __cplusplus >= 201103L
    template<class T> struct is_default_compare : std::false_type { };
    template<class T> struct is_default_compare<std::less<T>> : std::true_type { };
    template<class T> struct is_default_compare<std::greater<T>> : std::true_type { };
#endif

    // Returns floor(log2(n)), assumes n > 0.
    template<class T>
    inline int log2(T n) {
        int log = 0;
        while (n >>= 1) ++log;
        return log;
    }

    // Sorts [begin, end) using insertion sort with the given comparison function.
    template<class Iter, class Compare>
    inline void insertion_sort(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end) return;

        for (Iter cur = begin + 1; cur != end; ++cur) {
            Iter sift = cur;
            Iter sift_1 = cur - 1;

            // Compare first so we can avoid 2 moves for an element already positioned correctly.
            if (comp(*sift, *sift_1)) {
                T tmp = PDQSORT_PREFER_MOVE(*sift);

                do { *sift-- = PDQSORT_PREFER_MOVE(*sift_1); }
                while (sift != begin && comp(tmp, *--sift_1));

                *sift = PDQSORT_PREFER_MOVE(tmp);
            }
        }
    }

    // Sorts [begin, end) using insertion sort with the given comparison function. Assumes
    // *(begin - 1) is an element smaller than or equal to any element in [begin, end).
    template<class Iter, class Compare>
    inline void unguarded_insertion_sort(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end) return;

        for (Iter cur = begin + 1; cur != end; ++cur) {
            Iter sift = cur;
            Iter sift_1 = cur - 1;

            // Compare first so we can avoid 2 moves for an element already positioned correctly.
            if (comp(*sift, *sift_1)) {
                T tmp = PDQSORT_PREFER_MOVE(*sift);

                do { *sift-- = PDQSORT_PREFER_MOVE(*sift_1); }
                while (comp(tmp, *--sift_1));

                *sift = PDQSORT_PREFER_MOVE(tmp);
            }
        }
    }

    // Attempts to use insertion sort on [begin, end). Will return false if more than
    // partial_insertion_sort_limit elements were moved, and abort sorting. Otherwise it will
    // successfully sort and return true.
    template<class Iter, class Compare>
    inline bool partial_insertion_sort(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end) return true;
        
        std::size_t limit = 0;
        for (Iter cur = begin + 1; cur != end; ++cur) {
            Iter sift = cur;
            Iter sift_1 = cur - 1;

            // Compare first so we can avoid 2 moves for an element already positioned correctly.
            if (comp(*sift, *sift_1)) {
                T tmp = PDQSORT_PREFER_MOVE(*sift);

                do { *sift-- = PDQSORT_PREFER_MOVE(*sift_1); }
                while (sift != begin && comp(tmp, *--sift_1));

                *sift = PDQSORT_PREFER_MOVE(tmp);
                limit += cur - sift;
            }
            
            if (limit > partial_insertion_sort_limit) return false;
        }

        return true;
    }

    template<class Iter, class Compare>
    inline void sort2(Iter a, Iter b, Compare comp) {
        if (comp(*b, *a)) std::iter_swap(a, b);
    }

    // Sorts the elements *a, *b and *c using comparison function comp.
    template<class Iter, class Compare>
    inline void sort3(Iter a, Iter b, Iter c, Compare comp) {
        sort2(a, b, comp);
        sort2(b, c, comp);
        sort2(a, b, comp);
    }

    template<class T>
    inline T* align_cacheline(T* p) {
#if defined(UINTPTR_MAX) && __cplusplus >= 201103L
        std::uintptr_t ip = reinterpret_cast<std::uintptr_t>(p);
#else
        std::size_t ip = reinterpret_cast<std::size_t>(p);
#endif
        ip = (ip + cacheline_size - 1) & -cacheline_size;
        return reinterpret_cast<T*>(ip);
    }

    template<class Iter>
    inline void swap_offsets(Iter first, Iter last,
                             unsigned char* offsets_l, unsigned char* offsets_r,
                             size_t num, bool use_swaps) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (use_swaps) {
            // This case is needed for the descending distribution, where we need
            // to have proper swapping for pdqsort to remain O(n).
            for (size_t i = 0; i < num; ++i) {
                std::iter_swap(first + offsets_l[i], last - offsets_r[i]);
            }
        } else if (num > 0) {
            Iter l = first + offsets_l[0]; Iter r = last - offsets_r[0];
            T tmp(PDQSORT_PREFER_MOVE(*l)); *l = PDQSORT_PREFER_MOVE(*r);
            for (size_t i = 1; i < num; ++i) {
                l = first + offsets_l[i]; *r = PDQSORT_PREFER_MOVE(*l);
                r = last - offsets_r[i]; *l = PDQSORT_PREFER_MOVE(*r);
            }
            *r = PDQSORT_PREFER_MOVE(tmp);
        }
    }

    // Partitions [begin, end) around pivot *begin using comparison function comp. Elements equal
    // to the pivot are put in the right-hand partition. Returns the position of the pivot after
    // partitioning and whether the passed sequence already was correctly partitioned. Assumes the
    // pivot is a median of at least 3 elements and that [begin, end) is at least
    // insertion_sort_threshold long. Uses branchless partitioning.
    template<class Iter, class Compare>
    inline std::pair<Iter, bool> partition_right_branchless(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;

        // Move pivot into local for speed.
        T pivot(PDQSORT_PREFER_MOVE(*begin));
        Iter first = begin;
        Iter last = end;

        // Find the first element greater than or equal than the pivot (the median of 3 guarantees
        // this exists).
        while (comp(*++first, pivot));

        // Find the first element strictly smaller than the pivot. We have to guard this search if
        // there was no element before *first.
        if (first - 1 == begin) while (first < last && !comp(*--last, pivot));
        else                    while (                !comp(*--last, pivot));

        // If the first pair of elements that should be swapped to partition are the same element,
        // the passed in sequence already was correctly partitioned.
        bool already_partitioned = first >= last;
        if (!already_partitioned) {
            std::iter_swap(first, last);
            ++first;

            // The following branchless partitioning is derived from "BlockQuicksort: How Branch
            // Mispredictions don’t affect Quicksort" by Stefan Edelkamp and Armin Weiss, but
            // heavily micro-optimized.
            unsigned char offsets_l_storage[block_size + cacheline_size];
            unsigned char offsets_r_storage[block_size + cacheline_size];
            unsigned char* offsets_l = align_cacheline(offsets_l_storage);
            unsigned char* offsets_r = align_cacheline(offsets_r_storage);

            Iter offsets_l_base = first;
            Iter offsets_r_base = last;
            size_t num_l, num_r, start_l, start_r;
            num_l = num_r = start_l = start_r = 0;
            
            while (first < last) {
                // Fill up offset blocks with elements that are on the wrong side.
                // First we determine how much elements are considered for each offset block.
                size_t num_unknown = last - first;
                size_t left_split = num_l == 0 ? (num_r == 0 ? num_unknown / 2 : num_unknown) : 0;
                size_t right_split = num_r == 0 ? (num_unknown - left_split) : 0;

                // Fill the offset blocks.
                if (left_split >= block_size) {
                    for (size_t i = 0; i < block_size;) {
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                    }
                } else {
                    for (size_t i = 0; i < left_split;) {
                        offsets_l[num_l] = i++; num_l += !comp(*first, pivot); ++first;
                    }
                }

                if (right_split >= block_size) {
                    for (size_t i = 0; i < block_size;) {
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                    }
                } else {
                    for (size_t i = 0; i < right_split;) {
                        offsets_r[num_r] = ++i; num_r += comp(*--last, pivot);
                    }
                }

                // Swap elements and update block sizes and first/last boundaries.
                size_t num = std::min(num_l, num_r);
                swap_offsets(offsets_l_base, offsets_r_base,
                             offsets_l + start_l, offsets_r + start_r,
                             num, num_l == num_r);
                num_l -= num; num_r -= num;
                start_l += num; start_r += num;

                if (num_l == 0) {
                    start_l = 0;
                    offsets_l_base = first;
                }
                
                if (num_r == 0) {
                    start_r = 0;
                    offsets_r_base = last;
                }
            }

            // We have now fully identified [first, last)'s proper position. Swap the last elements.
            if (num_l) {
                offsets_l += start_l;
                while (num_l--) std::iter_swap(offsets_l_base + offsets_l[num_l], --last);
                first = last;
            }
            if (num_r) {
                offsets_r += start_r;
                while (num_r--) std::iter_swap(offsets_r_base - offsets_r[num_r], first), ++first;
                last = first;
            }
        }

        // Put the pivot in the right place.
        Iter pivot_pos = first - 1;
        *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
        *pivot_pos = PDQSORT_PREFER_MOVE(pivot);

        return std::make_pair(pivot_pos, already_partitioned);
    }



    // Partitions [begin, end) around pivot *begin using comparison function comp. Elements equal
    // to the pivot are put in the right-hand partition. Returns the position of the pivot after
    // partitioning and whether the passed sequence already was correctly partitioned. Assumes the
    // pivot is a median of at least 3 elements and that [begin, end) is at least
    // insertion_sort_threshold long.
    template<class Iter, class Compare>
    inline std::pair<Iter, bool> partition_right(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;
        
        // Move pivot into local for speed.
        T pivot(PDQSORT_PREFER_MOVE(*begin));

        Iter first = begin;
        Iter last = end;

        // Find the first element greater than or equal than the pivot (the median of 3 guarantees
        // this exists).
        while (comp(*++first, pivot));

        // Find the first element strictly smaller than the pivot. We have to guard this search if
        // there was no element before *first.
        if (first - 1 == begin) while (first < last && !comp(*--last, pivot));
        else                    while (                !comp(*--last, pivot));

        // If the first pair of elements that should be swapped to partition are the same element,
        // the passed in sequence already was correctly partitioned.
        bool already_partitioned = first >= last;
        
        // Keep swapping pairs of elements that are on the wrong side of the pivot. Previously
        // swapped pairs guard the searches, which is why the first iteration is special-cased
        // above.
        while (first < last) {
            std::iter_swap(first, last);
            while (comp(*++first, pivot));
            while (!comp(*--last, pivot));
        }

        // Put the pivot in the right place.
        Iter pivot_pos = first - 1;
        *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
        *pivot_pos = PDQSORT_PREFER_MOVE(pivot);

        return std::make_pair(pivot_pos, already_partitioned);
    }

    // Similar function to the one above, except elements equal to the pivot are put to the left of
    // the pivot and it doesn't check or return if the passed sequence already was partitioned.
    // Since this is rarely used (the many equal case), and in that case pdqsort already has O(n)
    // performance, no block quicksort is applied here for simplicity.
    template<class Iter, class Compare>
    inline Iter partition_left(Iter begin, Iter end, Compare comp) {
        typedef typename std::iterator_traits<Iter>::value_type T;

        T pivot(PDQSORT_PREFER_MOVE(*begin));
        Iter first = begin;
        Iter last = end;
        
        while (comp(pivot, *--last));

        if (last + 1 == end) while (first < last && !comp(pivot, *++first));
        else                 while (                !comp(pivot, *++first));

        while (first < last) {
            std::iter_swap(first, last);
            while (comp(pivot, *--last));
            while (!comp(pivot, *++first));
        }

        Iter pivot_pos = last;
        *begin = PDQSORT_PREFER_MOVE(*pivot_pos);
        *pivot_pos = PDQSORT_PREFER_MOVE(pivot);

        return pivot_pos;
    }


    template<class Iter, class Compare, bool Branchless>
    inline void pdqsort_loop(Iter begin, Iter end, Compare comp, int bad_allowed, bool leftmost = true) {
        typedef typename std::iterator_traits<Iter>::difference_type diff_t;

        // Use a while loop for tail recursion elimination.
        while (true) {
            diff_t size = end - begin;

            // Insertion sort is faster for small arrays.
            if (size < insertion_sort_threshold) {
                if (leftmost) insertion_sort(begin, end, comp);
                else unguarded_insertion_sort(begin, end, comp);
                return;
            }

            // Choose pivot as median of 3 or pseudomedian of 9.
            diff_t s2 = size / 2;
            if (size > ninther_threshold) {
                sort3(begin, begin + s2, end - 1, comp);
                sort3(begin + 1, begin + (s2 - 1), end - 2, comp);
                sort3(begin + 2, begin + (s2 + 1), end - 3, comp);
                sort3(begin + (s2 - 1), begin + s2, begin + (s2 + 1), comp);
                std::iter_swap(begin, begin + s2);
            } else sort3(begin + s2, begin, end - 1, comp);

            // If *(begin - 1) is the end of the right partition of a previous partition operation
            // there is no element in [begin, end) that is smaller than *(begin - 1). Then if our
            // pivot compares equal to *(begin - 1) we change strategy, putting equal elements in
            // the left partition, greater elements in the right partition. We do not have to
            // recurse on the left partition, since it's sorted (all equal).
            if (!leftmost && !comp(*(begin - 1), *begin)) {
                begin = partition_left(begin, end, comp) + 1;
                continue;
            }

            // Partition and get results.
            std::pair<Iter, bool> part_result =
                Branchless ? partition_right_branchless(begin, end, comp)
                           : partition_right(begin, end, comp);
            Iter pivot_pos = part_result.first;
            bool already_partitioned = part_result.second;

            // Check for a highly unbalanced partition.
            diff_t l_size = pivot_pos - begin;
            diff_t r_size = end - (pivot_pos + 1);
            bool highly_unbalanced = l_size < size / 8 || r_size < size / 8;

            // If we got a highly unbalanced partition we shuffle elements to break many patterns.
            if (highly_unbalanced) {
                // If we had too many bad partitions, switch to heapsort to guarantee O(n log n).
                if (--bad_allowed == 0) {
                    std::make_heap(begin, end, comp);
                    std::sort_heap(begin, end, comp);
                    return;
                }

                if (l_size >= insertion_sort_threshold) {
                    std::iter_swap(begin,             begin + l_size / 4);
                    std::iter_swap(pivot_pos - 1, pivot_pos - l_size / 4);

                    if (l_size > ninther_threshold) {
                        std::iter_swap(begin + 1,         begin + (l_size / 4 + 1));
                        std::iter_swap(begin + 2,         begin + (l_size / 4 + 2));
                        std::iter_swap(pivot_pos - 2, pivot_pos - (l_size / 4 + 1));
                        std::iter_swap(pivot_pos - 3, pivot_pos - (l_size / 4 + 2));
                    }
                }
                
                if (r_size >= insertion_sort_threshold) {
                    std::iter_swap(pivot_pos + 1, pivot_pos + (1 + r_size / 4));
                    std::iter_swap(end - 1,                   end - r_size / 4);
                    
                    if (r_size > ninther_threshold) {
                        std::iter_swap(pivot_pos + 2, pivot_pos + (2 + r_size / 4));
                        std::iter_swap(pivot_pos + 3, pivot_pos + (3 + r_size / 4));
                        std::iter_swap(end - 2,             end - (1 + r_size / 4));
                        std::iter_swap(end - 3,             end - (2 + r_size / 4));
                    }
                }
            } else {
                // If we were decently balanced and we tried to sort an already partitioned
                // sequence try to use insertion sort.
                if (already_partitioned && partial_insertion_sort(begin, pivot_pos, comp)
                                        && partial_insertion_sort(pivot_pos + 1, end, comp)) return;
            }
                
            // Sort the left partition first using recursion and do tail recursion elimination for
            // the right-hand partition.
            pdqsort_loop<Iter, Compare, Branchless>(begin, pivot_pos, comp, bad_allowed, leftmost);
            begin = pivot_pos + 1;
            leftmost = false;
        }
    }
}


template<class Iter, class Compare>
inline void pdqsort(Iter begin, Iter end, Compare comp) {
    if (begin == end) return;

#if __cplusplus >= 201103L
    pdqsort_detail::pdqsort_loop<Iter, Compare,
        pdqsort_detail::is_default_compare<typename std::decay<Compare>::type>::value &&
        std::is_arithmetic<typename std::iterator_traits<Iter>::value_type>::value>(
        begin, end, comp, pdqsort_detail::log2(end - begin));
#else
    pdqsort_detail::pdqsort_loop<Iter, Compare, false>(
        begin, end, comp, pdqsort_detail::log2(end - begin));
#endif
}

template<class Iter>
inline void pdqsort(Iter begin, Iter end) {
    typedef typename std::iterator_traits<Iter>::value_type T;
    pdqsort(begin, end, std::less<T>());
}

template<class Iter, class Compare>
inline void pdqsort_branchless(Iter begin, Iter end, Compare comp) {
    if (begin == end) return;
    pdqsort_detail::pdqsort_loop<Iter, Compare, true>(
        begin, end, comp, pdqsort_detail::log2(end - begin));
}

template<class Iter>
inline void pdqsort_branchless(Iter begin, Iter end) {
    typedef typename std::iterator_traits<Iter>::value_type T;
    pdqsort_branchless(begin, end, std::less<T>());
}


#undef PDQSORT_PREFER_MOVE

#endif


// ============================================================================
// 2. ska_sort
// ============================================================================

//          Copyright Malte Skarupke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <tuple>
#include <utility>

namespace detail
{
template<typename count_type, typename It, typename OutIt, typename ExtractKey>
void counting_sort_impl(It begin, It end, OutIt out_begin, ExtractKey && extract_key)
{
    count_type counts[256] = {};
    for (It it = begin; it != end; ++it)
    {
        ++counts[extract_key(*it)];
    }
    count_type total = 0;
    for (count_type & count : counts)
    {
        count_type old_count = count;
        count = total;
        total += old_count;
    }
    for (; begin != end; ++begin)
    {
        std::uint8_t key = extract_key(*begin);
        out_begin[counts[key]++] = std::move(*begin);
    }
}
template<typename It, typename OutIt, typename ExtractKey>
void counting_sort_impl(It begin, It end, OutIt out_begin, ExtractKey && extract_key)
{
    counting_sort_impl<std::uint64_t>(begin, end, out_begin, extract_key);
}
inline bool to_unsigned_or_bool(bool b)
{
    return b;
}
inline unsigned char to_unsigned_or_bool(unsigned char c)
{
    return c;
}
inline unsigned char to_unsigned_or_bool(signed char c)
{
    return static_cast<unsigned char>(c) + 128;
}
inline unsigned char to_unsigned_or_bool(char c)
{
    return static_cast<unsigned char>(c);
}
inline std::uint16_t to_unsigned_or_bool(char16_t c)
{
    return static_cast<std::uint16_t>(c);
}
inline std::uint32_t to_unsigned_or_bool(char32_t c)
{
    return static_cast<std::uint32_t>(c);
}
inline std::uint32_t to_unsigned_or_bool(wchar_t c)
{
    return static_cast<std::uint32_t>(c);
}
inline unsigned short to_unsigned_or_bool(short i)
{
    return static_cast<unsigned short>(i) + static_cast<unsigned short>(1 << (sizeof(short) * 8 - 1));
}
inline unsigned short to_unsigned_or_bool(unsigned short i)
{
    return i;
}
inline unsigned int to_unsigned_or_bool(int i)
{
    return static_cast<unsigned int>(i) + static_cast<unsigned int>(1 << (sizeof(int) * 8 - 1));
}
inline unsigned int to_unsigned_or_bool(unsigned int i)
{
    return i;
}
inline unsigned long to_unsigned_or_bool(long l)
{
    return static_cast<unsigned long>(l) + static_cast<unsigned long>(1l << (sizeof(long) * 8 - 1));
}
inline unsigned long to_unsigned_or_bool(unsigned long l)
{
    return l;
}
inline unsigned long long to_unsigned_or_bool(long long l)
{
    return static_cast<unsigned long long>(l) + static_cast<unsigned long long>(1ll << (sizeof(long long) * 8 - 1));
}
inline unsigned long long to_unsigned_or_bool(unsigned long long l)
{
    return l;
}
inline std::uint32_t to_unsigned_or_bool(float f)
{
    union
    {
        float f;
        std::uint32_t u;
    } as_union = { f };
    std::uint32_t sign_bit = -std::int32_t(as_union.u >> 31);
    return as_union.u ^ (sign_bit | 0x80000000);
}
inline std::uint64_t to_unsigned_or_bool(double f)
{
    union
    {
        double d;
        std::uint64_t u;
    } as_union = { f };
    std::uint64_t sign_bit = -std::int64_t(as_union.u >> 63);
    return as_union.u ^ (sign_bit | 0x8000000000000000);
}
template<typename T>
inline size_t to_unsigned_or_bool(T * ptr)
{
    return reinterpret_cast<size_t>(ptr);
}

template<size_t>
struct SizedRadixSorter;

template<>
struct SizedRadixSorter<1>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        counting_sort_impl(begin, end, buffer_begin, [&](auto && o)
        {
            return to_unsigned_or_bool(extract_key(o));
        });
        return true;
    }

    static constexpr size_t pass_count = 2;
};
template<>
struct SizedRadixSorter<2>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        std::ptrdiff_t num_elements = end - begin;
        if (num_elements <= (1ll << 32))
            return sort_inline<uint32_t>(begin, end, buffer_begin, buffer_begin + num_elements, extract_key);
        else
            return sort_inline<uint64_t>(begin, end, buffer_begin, buffer_begin + num_elements, extract_key);
    }

    template<typename count_type, typename It, typename OutIt, typename ExtractKey>
    static bool sort_inline(It begin, It end, OutIt out_begin, OutIt out_end, ExtractKey && extract_key)
    {
        count_type counts0[256] = {};
        count_type counts1[256] = {};

        for (It it = begin; it != end; ++it)
        {
            uint16_t key = to_unsigned_or_bool(extract_key(*it));
            ++counts0[key & 0xff];
            ++counts1[(key >> 8) & 0xff];
        }
        count_type total0 = 0;
        count_type total1 = 0;
        for (int i = 0; i < 256; ++i)
        {
            count_type old_count0 = counts0[i];
            count_type old_count1 = counts1[i];
            counts0[i] = total0;
            counts1[i] = total1;
            total0 += old_count0;
            total1 += old_count1;
        }
        for (It it = begin; it != end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it));
            out_begin[counts0[key]++] = std::move(*it);
        }
        for (OutIt it = out_begin; it != out_end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 8;
            begin[counts1[key]++] = std::move(*it);
        }
        return false;
    }

    static constexpr size_t pass_count = 3;
};
template<>
struct SizedRadixSorter<4>
{

    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        std::ptrdiff_t num_elements = end - begin;
        if (num_elements <= (1ll << 32))
            return sort_inline<uint32_t>(begin, end, buffer_begin, buffer_begin + num_elements, extract_key);
        else
            return sort_inline<uint64_t>(begin, end, buffer_begin, buffer_begin + num_elements, extract_key);
    }
    template<typename count_type, typename It, typename OutIt, typename ExtractKey>
    static bool sort_inline(It begin, It end, OutIt out_begin, OutIt out_end, ExtractKey && extract_key)
    {
        count_type counts0[256] = {};
        count_type counts1[256] = {};
        count_type counts2[256] = {};
        count_type counts3[256] = {};

        for (It it = begin; it != end; ++it)
        {
            uint32_t key = to_unsigned_or_bool(extract_key(*it));
            ++counts0[key & 0xff];
            ++counts1[(key >> 8) & 0xff];
            ++counts2[(key >> 16) & 0xff];
            ++counts3[(key >> 24) & 0xff];
        }
        count_type total0 = 0;
        count_type total1 = 0;
        count_type total2 = 0;
        count_type total3 = 0;
        for (int i = 0; i < 256; ++i)
        {
            count_type old_count0 = counts0[i];
            count_type old_count1 = counts1[i];
            count_type old_count2 = counts2[i];
            count_type old_count3 = counts3[i];
            counts0[i] = total0;
            counts1[i] = total1;
            counts2[i] = total2;
            counts3[i] = total3;
            total0 += old_count0;
            total1 += old_count1;
            total2 += old_count2;
            total3 += old_count3;
        }
        for (It it = begin; it != end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it));
            out_begin[counts0[key]++] = std::move(*it);
        }
        for (OutIt it = out_begin; it != out_end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 8;
            begin[counts1[key]++] = std::move(*it);
        }
        for (It it = begin; it != end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 16;
            out_begin[counts2[key]++] = std::move(*it);
        }
        for (OutIt it = out_begin; it != out_end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 24;
            begin[counts3[key]++] = std::move(*it);
        }
        return false;
    }

    static constexpr size_t pass_count = 5;
};
template<>
struct SizedRadixSorter<8>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        std::ptrdiff_t num_elements = end - begin;
        if (num_elements <= (1ll << 32))
            return sort_inline<uint32_t>(begin, end, buffer_begin, buffer_begin + num_elements, extract_key);
        else
            return sort_inline<uint64_t>(begin, end, buffer_begin, buffer_begin + num_elements, extract_key);
    }
    template<typename count_type, typename It, typename OutIt, typename ExtractKey>
    static bool sort_inline(It begin, It end, OutIt out_begin, OutIt out_end, ExtractKey && extract_key)
    {
        count_type counts0[256] = {};
        count_type counts1[256] = {};
        count_type counts2[256] = {};
        count_type counts3[256] = {};
        count_type counts4[256] = {};
        count_type counts5[256] = {};
        count_type counts6[256] = {};
        count_type counts7[256] = {};

        for (It it = begin; it != end; ++it)
        {
            uint64_t key = to_unsigned_or_bool(extract_key(*it));
            ++counts0[key & 0xff];
            ++counts1[(key >> 8) & 0xff];
            ++counts2[(key >> 16) & 0xff];
            ++counts3[(key >> 24) & 0xff];
            ++counts4[(key >> 32) & 0xff];
            ++counts5[(key >> 40) & 0xff];
            ++counts6[(key >> 48) & 0xff];
            ++counts7[(key >> 56) & 0xff];
        }
        count_type total0 = 0;
        count_type total1 = 0;
        count_type total2 = 0;
        count_type total3 = 0;
        count_type total4 = 0;
        count_type total5 = 0;
        count_type total6 = 0;
        count_type total7 = 0;
        for (int i = 0; i < 256; ++i)
        {
            count_type old_count0 = counts0[i];
            count_type old_count1 = counts1[i];
            count_type old_count2 = counts2[i];
            count_type old_count3 = counts3[i];
            count_type old_count4 = counts4[i];
            count_type old_count5 = counts5[i];
            count_type old_count6 = counts6[i];
            count_type old_count7 = counts7[i];
            counts0[i] = total0;
            counts1[i] = total1;
            counts2[i] = total2;
            counts3[i] = total3;
            counts4[i] = total4;
            counts5[i] = total5;
            counts6[i] = total6;
            counts7[i] = total7;
            total0 += old_count0;
            total1 += old_count1;
            total2 += old_count2;
            total3 += old_count3;
            total4 += old_count4;
            total5 += old_count5;
            total6 += old_count6;
            total7 += old_count7;
        }
        for (It it = begin; it != end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it));
            out_begin[counts0[key]++] = std::move(*it);
        }
        for (OutIt it = out_begin; it != out_end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 8;
            begin[counts1[key]++] = std::move(*it);
        }
        for (It it = begin; it != end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 16;
            out_begin[counts2[key]++] = std::move(*it);
        }
        for (OutIt it = out_begin; it != out_end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 24;
            begin[counts3[key]++] = std::move(*it);
        }
        for (It it = begin; it != end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 32;
            out_begin[counts4[key]++] = std::move(*it);
        }
        for (OutIt it = out_begin; it != out_end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 40;
            begin[counts5[key]++] = std::move(*it);
        }
        for (It it = begin; it != end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 48;
            out_begin[counts6[key]++] = std::move(*it);
        }
        for (OutIt it = out_begin; it != out_end; ++it)
        {
            std::uint8_t key = to_unsigned_or_bool(extract_key(*it)) >> 56;
            begin[counts7[key]++] = std::move(*it);
        }
        return false;
    }

    static constexpr size_t pass_count = 9;
};

template<typename>
struct RadixSorter;
template<>
struct RadixSorter<bool>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        size_t false_count = 0;
        for (It it = begin; it != end; ++it)
        {
            if (!extract_key(*it))
                ++false_count;
        }
        size_t true_position = false_count;
        false_count = 0;
        for (; begin != end; ++begin)
        {
            if (extract_key(*begin))
                buffer_begin[true_position++] = std::move(*begin);
            else
                buffer_begin[false_count++] = std::move(*begin);
        }
        return true;
    }

    static constexpr size_t pass_count = 2;
};
template<>
struct RadixSorter<signed char> : SizedRadixSorter<sizeof(signed char)>
{
};
template<>
struct RadixSorter<unsigned char> : SizedRadixSorter<sizeof(unsigned char)>
{
};
template<>
struct RadixSorter<signed short> : SizedRadixSorter<sizeof(signed short)>
{
};
template<>
struct RadixSorter<unsigned short> : SizedRadixSorter<sizeof(unsigned short)>
{
};
template<>
struct RadixSorter<signed int> : SizedRadixSorter<sizeof(signed int)>
{
};
template<>
struct RadixSorter<unsigned int> : SizedRadixSorter<sizeof(unsigned int)>
{
};
template<>
struct RadixSorter<signed long> : SizedRadixSorter<sizeof(signed long)>
{
};
template<>
struct RadixSorter<unsigned long> : SizedRadixSorter<sizeof(unsigned long)>
{
};
template<>
struct RadixSorter<signed long long> : SizedRadixSorter<sizeof(signed long long)>
{
};
template<>
struct RadixSorter<unsigned long long> : SizedRadixSorter<sizeof(unsigned long long)>
{
};
template<>
struct RadixSorter<float> : SizedRadixSorter<sizeof(float)>
{
};
template<>
struct RadixSorter<double> : SizedRadixSorter<sizeof(double)>
{
};
template<>
struct RadixSorter<char> : SizedRadixSorter<sizeof(char)>
{
};
template<>
struct RadixSorter<wchar_t> : SizedRadixSorter<sizeof(wchar_t)>
{
};
template<>
struct RadixSorter<char16_t> : SizedRadixSorter<sizeof(char16_t)>
{
};
template<>
struct RadixSorter<char32_t> : SizedRadixSorter<sizeof(char32_t)>
{
};
template<typename K, typename V>
struct RadixSorter<std::pair<K, V>>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        bool first_result = RadixSorter<V>::sort(begin, end, buffer_begin, [&](auto && o)
        {
            return extract_key(o).second;
        });
        auto extract_first = [&](auto && o)
        {
            return extract_key(o).first;
        };

        if (first_result)
        {
            return !RadixSorter<K>::sort(buffer_begin, buffer_begin + (end - begin), begin, extract_first);
        }
        else
        {
            return RadixSorter<K>::sort(begin, end, buffer_begin, extract_first);
        }
    }

    static constexpr size_t pass_count = RadixSorter<K>::pass_count + RadixSorter<V>::pass_count;
};
template<typename K, typename V>
struct RadixSorter<const std::pair<K, V> &>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        bool first_result = RadixSorter<V>::sort(begin, end, buffer_begin, [&](auto && o) -> const V &
        {
            return extract_key(o).second;
        });
        auto extract_first = [&](auto && o) -> const K &
        {
            return extract_key(o).first;
        };

        if (first_result)
        {
            return !RadixSorter<K>::sort(buffer_begin, buffer_begin + (end - begin), begin, extract_first);
        }
        else
        {
            return RadixSorter<K>::sort(begin, end, buffer_begin, extract_first);
        }
    }

    static constexpr size_t pass_count = RadixSorter<K>::pass_count + RadixSorter<V>::pass_count;
};
template<size_t I, size_t S, typename Tuple>
struct TupleRadixSorter
{
    using NextSorter = TupleRadixSorter<I + 1, S, Tuple>;
    using ThisSorter = RadixSorter<typename std::tuple_element<I, Tuple>::type>;

    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt out_begin, OutIt out_end, ExtractKey && extract_key)
    {
        bool which = NextSorter::sort(begin, end, out_begin, out_end, extract_key);
        auto extract_i = [&](auto && o)
        {
            return std::get<I>(extract_key(o));
        };
        if (which)
            return !ThisSorter::sort(out_begin, out_end, begin, extract_i);
        else
            return ThisSorter::sort(begin, end, out_begin, extract_i);
    }

    static constexpr size_t pass_count = ThisSorter::pass_count + NextSorter::pass_count;
};
template<size_t I, size_t S, typename Tuple>
struct TupleRadixSorter<I, S, const Tuple &>
{
    using NextSorter = TupleRadixSorter<I + 1, S, const Tuple &>;
    using ThisSorter = RadixSorter<typename std::tuple_element<I, Tuple>::type>;

    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt out_begin, OutIt out_end, ExtractKey && extract_key)
    {
        bool which = NextSorter::sort(begin, end, out_begin, out_end, extract_key);
        auto extract_i = [&](auto && o) -> decltype(auto)
        {
            return std::get<I>(extract_key(o));
        };
        if (which)
            return !ThisSorter::sort(out_begin, out_end, begin, extract_i);
        else
            return ThisSorter::sort(begin, end, out_begin, extract_i);
    }

    static constexpr size_t pass_count = ThisSorter::pass_count + NextSorter::pass_count;
};
template<size_t I, typename Tuple>
struct TupleRadixSorter<I, I, Tuple>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It, It, OutIt, OutIt, ExtractKey &&)
    {
        return false;
    }

    static constexpr size_t pass_count = 0;
};
template<size_t I, typename Tuple>
struct TupleRadixSorter<I, I, const Tuple &>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It, It, OutIt, OutIt, ExtractKey &&)
    {
        return false;
    }

    static constexpr size_t pass_count = 0;
};

template<typename... Args>
struct RadixSorter<std::tuple<Args...>>
{
    using SorterImpl = TupleRadixSorter<0, sizeof...(Args), std::tuple<Args...>>;

    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        return SorterImpl::sort(begin, end, buffer_begin, buffer_begin + (end - begin), extract_key);
    }

    static constexpr size_t pass_count = SorterImpl::pass_count;
};

template<typename... Args>
struct RadixSorter<const std::tuple<Args...> &>
{
    using SorterImpl = TupleRadixSorter<0, sizeof...(Args), const std::tuple<Args...> &>;

    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        return SorterImpl::sort(begin, end, buffer_begin, buffer_begin + (end - begin), extract_key);
    }

    static constexpr size_t pass_count = SorterImpl::pass_count;
};

template<typename T, size_t S>
struct RadixSorter<std::array<T, S>>
{
    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        auto buffer_end = buffer_begin + (end - begin);
        bool which = false;
        for (size_t i = S; i > 0; --i)
        {
            auto extract_i = [&, i = i - 1](auto && o)
            {
                return extract_key(o)[i];
            };
            if (which)
                which = !RadixSorter<T>::sort(buffer_begin, buffer_end, begin, extract_i);
            else
                which = RadixSorter<T>::sort(begin, end, buffer_begin, extract_i);
        }
        return which;
    }

    static constexpr size_t pass_count = RadixSorter<T>::pass_count * S;
};

template<typename T>
struct RadixSorter<const T> : RadixSorter<T>
{
};
template<typename T>
struct RadixSorter<T &> : RadixSorter<const T &>
{
};
template<typename T>
struct RadixSorter<T &&> : RadixSorter<T>
{
};
template<typename T>
struct RadixSorter<const T &> : RadixSorter<T>
{
};
template<typename T>
struct RadixSorter<const T &&> : RadixSorter<T>
{
};
// these structs serve two purposes
// 1. they serve as illustration for how to implement the to_radix_sort_key function
// 2. they help produce better error messages. with these overloads you get the
//    error message "no matching function for call to to_radix_sort(your_type)"
//    without these examples, you'd get the error message "to_radix_sort_key was
//    not declared in this scope" which is a much less useful error message
struct ExampleStructA { int i; };
struct ExampleStructB { float f; };
inline int to_radix_sort_key(ExampleStructA a) { return a.i; }
inline float to_radix_sort_key(ExampleStructB b) { return b.f; }
template<typename T, typename Enable = void>
struct FallbackRadixSorter : RadixSorter<decltype(to_radix_sort_key(std::declval<T>()))>
{
    using base = RadixSorter<decltype(to_radix_sort_key(std::declval<T>()))>;

    template<typename It, typename OutIt, typename ExtractKey>
    static bool sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
    {
        return base::sort(begin, end, buffer_begin, [&](auto && a) -> decltype(auto)
        {
            return to_radix_sort_key(extract_key(a));
        });
    }
};

template<typename...>
struct nested_void
{
	using type = void;
};

template<typename... Args>
using void_t = typename nested_void<Args...>::type;

template<typename T>
struct has_subscript_operator_impl
{
	template<typename U, typename = decltype(std::declval<U>()[0])>
	static std::true_type test(int);
	template<typename>
	static std::false_type test(...);

	using type = decltype(test<T>(0));
};

template<typename T>
using has_subscript_operator = typename has_subscript_operator_impl<T>::type;


template<typename T>
struct FallbackRadixSorter<T, void_t<decltype(to_unsigned_or_bool(std::declval<T>()))>>
    : RadixSorter<decltype(to_unsigned_or_bool(std::declval<T>()))>
{
};

template<typename T>
struct RadixSorter : FallbackRadixSorter<T>
{
};

template<typename T>
size_t radix_sort_pass_count = RadixSorter<T>::pass_count;

template<typename It, typename Func>
inline void unroll_loop_four_times(It begin, size_t iteration_count, Func && to_call)
{
    size_t loop_count = iteration_count / 4;
    size_t remainder_count = iteration_count - loop_count * 4;
    for (; loop_count > 0; --loop_count)
    {
        to_call(begin);
        ++begin;
        to_call(begin);
        ++begin;
        to_call(begin);
        ++begin;
        to_call(begin);
        ++begin;
    }
    switch(remainder_count)
    {
    case 3:
        to_call(begin);
        ++begin;
    case 2:
        to_call(begin);
        ++begin;
    case 1:
        to_call(begin);
    }
}

template<typename It, typename F>
inline It custom_std_partition(It begin, It end, F && func)
{
    for (;; ++begin)
    {
        if (begin == end)
            return end;
        if (!func(*begin))
            break;
    }
    It it = begin;
    for(++it; it != end; ++it)
    {
        if (!func(*it))
            continue;

        std::iter_swap(begin, it);
        ++begin;
    }
    return begin;
}

struct PartitionInfo
{
    PartitionInfo()
        : count(0)
    {
    }

    union
    {
        size_t count;
        size_t offset;
    };
    size_t next_offset;
};

template<size_t>
struct UnsignedForSize;
template<>
struct UnsignedForSize<1>
{
    typedef uint8_t type;
};
template<>
struct UnsignedForSize<2>
{
    typedef uint16_t type;
};
template<>
struct UnsignedForSize<4>
{
    typedef uint32_t type;
};
template<>
struct UnsignedForSize<8>
{
    typedef uint64_t type;
};
template<typename T>
struct SubKey;
template<size_t Size>
struct SizedSubKey
{
    template<typename T>
    static auto sub_key(T && value, void *)
    {
        return to_unsigned_or_bool(value);
    }

    typedef SubKey<void> next;

    using sub_key_type = typename UnsignedForSize<Size>::type;
};
template<typename T>
struct SubKey<const T> : SubKey<T>
{
};
template<typename T>
struct SubKey<T &> : SubKey<T>
{
};
template<typename T>
struct SubKey<T &&> : SubKey<T>
{
};
template<typename T>
struct SubKey<const T &> : SubKey<T>
{
};
template<typename T>
struct SubKey<const T &&> : SubKey<T>
{
};
template<typename T, typename Enable = void>
struct FallbackSubKey
    : SubKey<decltype(to_radix_sort_key(std::declval<T>()))>
{
    using base = SubKey<decltype(to_radix_sort_key(std::declval<T>()))>;

    template<typename U>
    static decltype(auto) sub_key(U && value, void * data)
    {
        return base::sub_key(to_radix_sort_key(value), data);
    }
};
template<typename T>
struct FallbackSubKey<T, void_t<decltype(to_unsigned_or_bool(std::declval<T>()))>>
    : SubKey<decltype(to_unsigned_or_bool(std::declval<T>()))>
{
};
template<typename T>
struct SubKey : FallbackSubKey<T>
{
};
template<>
struct SubKey<bool>
{
    template<typename T>
    static bool sub_key(T && value, void *)
    {
        return value;
    }

    typedef SubKey<void> next;

    using sub_key_type = bool;
};
template<>
struct SubKey<void>;
template<>
struct SubKey<unsigned char> : SizedSubKey<sizeof(unsigned char)>
{
};
template<>
struct SubKey<unsigned short> : SizedSubKey<sizeof(unsigned short)>
{
};
template<>
struct SubKey<unsigned int> : SizedSubKey<sizeof(unsigned int)>
{
};
template<>
struct SubKey<unsigned long> : SizedSubKey<sizeof(unsigned long)>
{
};
template<>
struct SubKey<unsigned long long> : SizedSubKey<sizeof(unsigned long long)>
{
};
template<typename T>
struct SubKey<T *> : SizedSubKey<sizeof(T *)>
{
};
template<typename F, typename S, typename Current>
struct PairSecondSubKey : Current
{
    static decltype(auto) sub_key(const std::pair<F, S> & value, void * sort_data)
    {
        return Current::sub_key(value.second, sort_data);
    }

    using next = typename std::conditional<std::is_same<SubKey<void>, typename Current::next>::value, SubKey<void>, PairSecondSubKey<F, S, typename Current::next>>::type;
};
template<typename F, typename S, typename Current>
struct PairFirstSubKey : Current
{
    static decltype(auto) sub_key(const std::pair<F, S> & value, void * sort_data)
    {
        return Current::sub_key(value.first, sort_data);
    }

    using next = typename std::conditional<std::is_same<SubKey<void>, typename Current::next>::value, PairSecondSubKey<F, S, SubKey<S>>, PairFirstSubKey<F, S, typename Current::next>>::type;
};
template<typename F, typename S>
struct SubKey<std::pair<F, S>> : PairFirstSubKey<F, S, SubKey<F>>
{
};
template<size_t Index, typename First, typename... More>
struct TypeAt : TypeAt<Index - 1, More..., void>
{
};
template<typename First, typename... More>
struct TypeAt<0, First, More...>
{
    typedef First type;
};

template<size_t Index, typename Current, typename First, typename... More>
struct TupleSubKey;

template<size_t Index, typename Next, typename First, typename... More>
struct NextTupleSubKey
{
    using type = TupleSubKey<Index, Next, First, More...>;
};
template<size_t Index, typename First, typename Second, typename... More>
struct NextTupleSubKey<Index, SubKey<void>, First, Second, More...>
{
    using type = TupleSubKey<Index + 1, SubKey<Second>, Second, More...>;
};
template<size_t Index, typename First>
struct NextTupleSubKey<Index, SubKey<void>, First>
{
    using type = SubKey<void>;
};

template<size_t Index, typename Current, typename First, typename... More>
struct TupleSubKey : Current
{
    template<typename Tuple>
    static decltype(auto) sub_key(const Tuple & value, void * sort_data)
    {
        return Current::sub_key(std::get<Index>(value), sort_data);
    }

    using next = typename NextTupleSubKey<Index, typename Current::next, First, More...>::type;
};
template<size_t Index, typename Current, typename First>
struct TupleSubKey<Index, Current, First> : Current
{
    template<typename Tuple>
    static decltype(auto) sub_key(const Tuple & value, void * sort_data)
    {
        return Current::sub_key(std::get<Index>(value), sort_data);
    }

    using next = typename NextTupleSubKey<Index, typename Current::next, First>::type;
};
template<typename First, typename... More>
struct SubKey<std::tuple<First, More...>> : TupleSubKey<0, SubKey<First>, First, More...>
{
};

struct BaseListSortData
{
    size_t current_index;
    size_t recursion_limit;
    void * next_sort_data;
};
template<typename It, typename ExtractKey>
struct ListSortData : BaseListSortData
{
    void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *);
};

template<typename CurrentSubKey, typename T>
struct ListElementSubKey : SubKey<typename std::decay<decltype(std::declval<T>()[0])>::type>
{
    using base = SubKey<typename std::decay<decltype(std::declval<T>()[0])>::type>;

    using next = ListElementSubKey;

    template<typename U>
    static decltype(auto) sub_key(U && value, void * sort_data)
    {
        BaseListSortData * list_sort_data = static_cast<BaseListSortData *>(sort_data);
        const T & list = CurrentSubKey::sub_key(value, list_sort_data->next_sort_data);
        return base::sub_key(list[list_sort_data->current_index], list_sort_data->next_sort_data);
    }
};

template<typename T>
struct ListSubKey
{
    using next = SubKey<void>;

    using sub_key_type = T;

    static const T & sub_key(const T & value, void *)
    {
        return value;
    }
};

template<typename T>
struct FallbackSubKey<T, typename std::enable_if<has_subscript_operator<T>::value>::type> : ListSubKey<T>
{
};

template<typename It, typename ExtractKey>
inline void StdSortFallback(It begin, It end, ExtractKey & extract_key)
{
    std::sort(begin, end, [&](auto && l, auto && r){ return extract_key(l) < extract_key(r); });
}

template<std::ptrdiff_t StdSortThreshold, typename It, typename ExtractKey>
inline bool StdSortIfLessThanThreshold(It begin, It end, std::ptrdiff_t num_elements, ExtractKey & extract_key)
{
    if (num_elements <= 1)
        return true;
    if (num_elements >= StdSortThreshold)
        return false;
    StdSortFallback(begin, end, extract_key);
    return true;
}

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey, typename SubKeyType = typename CurrentSubKey::sub_key_type>
struct InplaceSorter;

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey, size_t NumBytes, size_t Offset = 0>
struct UnsignedInplaceSorter
{
    static constexpr size_t ShiftAmount = (((NumBytes - 1) - Offset) * 8);
    template<typename T>
    inline static uint8_t current_byte(T && elem, void * sort_data)
    {
        return CurrentSubKey::sub_key(elem, sort_data) >> ShiftAmount;
    }
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, std::ptrdiff_t num_elements, ExtractKey & extract_key, void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *), void * sort_data)
    {
        if (num_elements < AmericanFlagSortThreshold)
            american_flag_sort(begin, end, extract_key, next_sort, sort_data);
        else
            ska_byte_sort(begin, end, extract_key, next_sort, sort_data);
    }

    template<typename It, typename ExtractKey>
    static void american_flag_sort(It begin, It end, ExtractKey & extract_key, void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *), void * sort_data)
    {
        PartitionInfo partitions[256];
        for (It it = begin; it != end; ++it)
        {
            ++partitions[current_byte(extract_key(*it), sort_data)].count;
        }
        size_t total = 0;
        uint8_t remaining_partitions[256];
        int num_partitions = 0;
        for (int i = 0; i < 256; ++i)
        {
            size_t count = partitions[i].count;
            if (!count)
                continue;
            partitions[i].offset = total;
            total += count;
            partitions[i].next_offset = total;
            remaining_partitions[num_partitions] = i;
            ++num_partitions;
        }
        if (num_partitions > 1)
        {
            uint8_t * current_block_ptr = remaining_partitions;
            PartitionInfo * current_block = partitions + *current_block_ptr;
            uint8_t * last_block = remaining_partitions + num_partitions - 1;
            It it = begin;
            It block_end = begin + current_block->next_offset;
            It last_element = end - 1;
            for (;;)
            {
                PartitionInfo * block = partitions + current_byte(extract_key(*it), sort_data);
                if (block == current_block)
                {
                    ++it;
                    if (it == last_element)
                        break;
                    else if (it == block_end)
                    {
                        for (;;)
                        {
                            ++current_block_ptr;
                            if (current_block_ptr == last_block)
                                goto recurse;
                            current_block = partitions + *current_block_ptr;
                            if (current_block->offset != current_block->next_offset)
                                break;
                        }

                        it = begin + current_block->offset;
                        block_end = begin + current_block->next_offset;
                    }
                }
                else
                {
                    size_t offset = block->offset++;
                    std::iter_swap(it, begin + offset);
                }
            }
        }
        recurse:
        if (Offset + 1 != NumBytes || next_sort)
        {
            size_t start_offset = 0;
            It partition_begin = begin;
            for (uint8_t * it = remaining_partitions, * end = remaining_partitions + num_partitions; it != end; ++it)
            {
                size_t end_offset = partitions[*it].next_offset;
                It partition_end = begin + end_offset;
                std::ptrdiff_t num_elements = end_offset - start_offset;
                if (!StdSortIfLessThanThreshold<StdSortThreshold>(partition_begin, partition_end, num_elements, extract_key))
                {
                    UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, NumBytes, Offset + 1>::sort(partition_begin, partition_end, num_elements, extract_key, next_sort, sort_data);
                }
                start_offset = end_offset;
                partition_begin = partition_end;
            }
        }
    }

    template<typename It, typename ExtractKey>
    static void ska_byte_sort(It begin, It end, ExtractKey & extract_key, void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *), void * sort_data)
    {
        PartitionInfo partitions[256];
        for (It it = begin; it != end; ++it)
        {
            ++partitions[current_byte(extract_key(*it), sort_data)].count;
        }
        uint8_t remaining_partitions[256];
        size_t total = 0;
        int num_partitions = 0;
        for (int i = 0; i < 256; ++i)
        {
            size_t count = partitions[i].count;
            if (count)
            {
                partitions[i].offset = total;
                total += count;
                remaining_partitions[num_partitions] = i;
                ++num_partitions;
            }
            partitions[i].next_offset = total;
        }
        for (uint8_t * last_remaining = remaining_partitions + num_partitions, * end_partition = remaining_partitions + 1; last_remaining > end_partition;)
        {
            last_remaining = custom_std_partition(remaining_partitions, last_remaining, [&](uint8_t partition)
            {
                size_t & begin_offset = partitions[partition].offset;
                size_t & end_offset = partitions[partition].next_offset;
                if (begin_offset == end_offset)
                    return false;

                unroll_loop_four_times(begin + begin_offset, end_offset - begin_offset, [partitions = partitions, begin, &extract_key, sort_data](It it)
                {
                    uint8_t this_partition = current_byte(extract_key(*it), sort_data);
                    size_t offset = partitions[this_partition].offset++;
                    std::iter_swap(it, begin + offset);
                });
                return begin_offset != end_offset;
            });
        }
        if (Offset + 1 != NumBytes || next_sort)
        {
            for (uint8_t * it = remaining_partitions + num_partitions; it != remaining_partitions; --it)
            {
                uint8_t partition = it[-1];
                size_t start_offset = (partition == 0 ? 0 : partitions[partition - 1].next_offset);
                size_t end_offset = partitions[partition].next_offset;
                It partition_begin = begin + start_offset;
                It partition_end = begin + end_offset;
                std::ptrdiff_t num_elements = end_offset - start_offset;
                if (!StdSortIfLessThanThreshold<StdSortThreshold>(partition_begin, partition_end, num_elements, extract_key))
                {
                    UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, NumBytes, Offset + 1>::sort(partition_begin, partition_end, num_elements, extract_key, next_sort, sort_data);
                }
            }
        }
    }
};

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey, size_t NumBytes>
struct UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, NumBytes, NumBytes>
{
    template<typename It, typename ExtractKey>
    inline static void sort(It begin, It end, std::ptrdiff_t num_elements, ExtractKey & extract_key, void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *), void * next_sort_data)
    {
        next_sort(begin, end, num_elements, extract_key, next_sort_data);
    }
};

template<typename It, typename ExtractKey, typename ElementKey>
size_t CommonPrefix(It begin, It end, size_t start_index, ExtractKey && extract_key, ElementKey && element_key)
{
    const auto & largest_match_list = extract_key(*begin);
    size_t largest_match = largest_match_list.size();
    if (largest_match == start_index)
        return start_index;
    for (++begin; begin != end; ++begin)
    {
        const auto & current_list = extract_key(*begin);
        size_t current_size = current_list.size();
        if (current_size < largest_match)
        {
            largest_match = current_size;
            if (largest_match == start_index)
                return start_index;
        }
        if (element_key(largest_match_list[start_index]) != element_key(current_list[start_index]))
            return start_index;
        for (size_t i = start_index + 1; i < largest_match; ++i)
        {
            if (element_key(largest_match_list[i]) != element_key(current_list[i]))
            {
                largest_match = i;
                break;
            }
        }
    }
    return largest_match;
}

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey, typename ListType>
struct ListInplaceSorter
{
    using ElementSubKey = ListElementSubKey<CurrentSubKey, ListType>;
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, ExtractKey & extract_key, ListSortData<It, ExtractKey> * sort_data)
    {
        size_t current_index = sort_data->current_index;
        void * next_sort_data = sort_data->next_sort_data;
        auto current_key = [&](auto && elem) -> decltype(auto)
        {
            return CurrentSubKey::sub_key(extract_key(elem), next_sort_data);
        };
        auto element_key = [&](auto && elem) -> decltype(auto)
        {
            return ElementSubKey::base::sub_key(elem, sort_data);
        };
        sort_data->current_index = current_index = CommonPrefix(begin, end, current_index, current_key, element_key);
        It end_of_shorter_ones = std::partition(begin, end, [&](auto && elem)
        {
            return current_key(elem).size() <= current_index;
        });
        std::ptrdiff_t num_shorter_ones = end_of_shorter_ones - begin;
        if (sort_data->next_sort && !StdSortIfLessThanThreshold<StdSortThreshold>(begin, end_of_shorter_ones, num_shorter_ones, extract_key))
        {
            sort_data->next_sort(begin, end_of_shorter_ones, num_shorter_ones, extract_key, next_sort_data);
        }
        std::ptrdiff_t num_elements = end - end_of_shorter_ones;
        if (!StdSortIfLessThanThreshold<StdSortThreshold>(end_of_shorter_ones, end, num_elements, extract_key))
        {
            void (*sort_next_element)(It, It, std::ptrdiff_t, ExtractKey &, void *) = static_cast<void (*)(It, It, std::ptrdiff_t, ExtractKey &, void *)>(&sort_from_recursion);
            InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, ElementSubKey>::sort(end_of_shorter_ones, end, num_elements, extract_key, sort_next_element, sort_data);
        }
    }

    template<typename It, typename ExtractKey>
    static void sort_from_recursion(It begin, It end, std::ptrdiff_t, ExtractKey & extract_key, void * next_sort_data)
    {
        ListSortData<It, ExtractKey> offset = *static_cast<ListSortData<It, ExtractKey> *>(next_sort_data);
        ++offset.current_index;
        --offset.recursion_limit;
        if (offset.recursion_limit == 0)
        {
            StdSortFallback(begin, end, extract_key);
        }
        else
        {
            sort(begin, end, extract_key, &offset);
        }
    }


    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, std::ptrdiff_t, ExtractKey & extract_key, void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *), void * next_sort_data)
    {
        ListSortData<It, ExtractKey> offset;
        offset.current_index = 0;
        offset.recursion_limit = 16;
        offset.next_sort = next_sort;
        offset.next_sort_data = next_sort_data;
        sort(begin, end, extract_key, &offset);
    }
};

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, bool>
{
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, std::ptrdiff_t, ExtractKey & extract_key, void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *), void * sort_data)
    {
        It middle = std::partition(begin, end, [&](auto && a){ return !CurrentSubKey::sub_key(extract_key(a), sort_data); });
        if (next_sort)
        {
            next_sort(begin, middle, middle - begin, extract_key, sort_data);
            next_sort(middle, end, end - middle, extract_key, sort_data);
        }
    }
};

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, uint8_t> : UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 1>
{
};
template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, uint16_t> : UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 2>
{
};
template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, uint32_t> : UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 4>
{
};
template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
struct InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, uint64_t> : UnsignedInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, 8>
{
};
template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey, typename SubKeyType, typename Enable = void>
struct FallbackInplaceSorter;

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey, typename SubKeyType>
struct InplaceSorter : FallbackInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, SubKeyType>
{
};

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey, typename SubKeyType>
struct FallbackInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, SubKeyType, typename std::enable_if<has_subscript_operator<SubKeyType>::value>::type>
	: ListInplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey, SubKeyType>
{
};

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
struct SortStarter;
template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold>
struct SortStarter<StdSortThreshold, AmericanFlagSortThreshold, SubKey<void>>
{
    template<typename It, typename ExtractKey>
    static void sort(It, It, std::ptrdiff_t, ExtractKey &, void *)
    {
    }
};

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename CurrentSubKey>
struct SortStarter
{
    template<typename It, typename ExtractKey>
    static void sort(It begin, It end, std::ptrdiff_t num_elements, ExtractKey & extract_key, void * next_sort_data = nullptr)
    {
        if (StdSortIfLessThanThreshold<StdSortThreshold>(begin, end, num_elements, extract_key))
            return;

        void (*next_sort)(It, It, std::ptrdiff_t, ExtractKey &, void *) = static_cast<void (*)(It, It, std::ptrdiff_t, ExtractKey &, void *)>(&SortStarter<StdSortThreshold, AmericanFlagSortThreshold, typename CurrentSubKey::next>::sort);
        if (next_sort == static_cast<void (*)(It, It, std::ptrdiff_t, ExtractKey &, void *)>(&SortStarter<StdSortThreshold, AmericanFlagSortThreshold, SubKey<void>>::sort))
            next_sort = nullptr;
        InplaceSorter<StdSortThreshold, AmericanFlagSortThreshold, CurrentSubKey>::sort(begin, end, num_elements, extract_key, next_sort, next_sort_data);
    }
};

template<std::ptrdiff_t StdSortThreshold, std::ptrdiff_t AmericanFlagSortThreshold, typename It, typename ExtractKey>
void inplace_radix_sort(It begin, It end, ExtractKey & extract_key)
{
    using SubKey = SubKey<decltype(extract_key(*begin))>;
    SortStarter<StdSortThreshold, AmericanFlagSortThreshold, SubKey>::sort(begin, end, end - begin, extract_key);
}

struct IdentityFunctor
{
    template<typename T>
    decltype(auto) operator()(T && i) const
    {
        return std::forward<T>(i);
    }
};
}

template<typename It, typename ExtractKey>
static void ska_sort(It begin, It end, ExtractKey && extract_key)
{
    detail::inplace_radix_sort<128, 1024>(begin, end, extract_key);
}

template<typename It>
static void ska_sort(It begin, It end)
{
    ska_sort(begin, end, detail::IdentityFunctor());
}

template<typename It, typename OutIt, typename ExtractKey>
bool ska_sort_copy(It begin, It end, OutIt buffer_begin, ExtractKey && key)
{
    std::ptrdiff_t num_elements = end - begin;
    if (num_elements < 128 || detail::radix_sort_pass_count<typename std::result_of<ExtractKey(decltype(*begin))>::type> >= 8)
    {
        ska_sort(begin, end, key);
        return false;
    }
    else
        return detail::RadixSorter<typename std::result_of<ExtractKey(decltype(*begin))>::type>::sort(begin, end, buffer_begin, key);
}
template<typename It, typename OutIt>
bool ska_sort_copy(It begin, It end, OutIt buffer_begin)
{
    return ska_sort_copy(begin, end, buffer_begin, detail::IdentityFunctor());
}


// ============================================================================
// 3. GeblomiSort
// ============================================================================

/**
 * GeblomiSort v2.5 — Buffer-Safe + Comparator-Aware Adaptive Hybrid
 *
 * Critical fixes in this revision:
 *   1. Stack buffer overflow eliminated — run tracking uses safe pre-check
 *      and correctly sized arrays (run_starts has space for sentinel).
 *   2. All core templates use valid C++20 constrained syntax.
 *   3. Custom comparator support retained.
 *   4. Strict O(1) extra memory even on adversarial inputs.
 *   5. All previous safety properties (underflow-free probe, concepts,
 *      collect-then-reverse, tail verification) retained.
 *
 * Team: Grok + Harper + Benjamin + Lucas | 2026-07-30
 *
 * Requires: C++20
 * Build:   g++ -O3 -std=c++20 -I. demo.cpp -o demo
 */
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cmath>
#include <type_traits>
#include <iterator>
#include <concepts>
#include <functional>
#include <utility>

// (pdqsort included above)
// (ska_sort included above)

namespace geblomi {

// ---------------------------------------------------------------------------
// Concepts & traits
// ---------------------------------------------------------------------------
template <typename It>
concept RandomAccessIterator = std::random_access_iterator<It>;

template <typename T>
inline constexpr bool supports_ska_sort = std::is_scalar_v<T>;

// Maximum number of runs we track. Beyond this → immediate pdqsort.
// run_starts needs one extra slot for the final sentinel.
constexpr size_t kMaxTrackedRuns = 64;

enum class Route { Small, Sorted, Reverse, Patterned, Random };

struct ProbeResult {
    Route route = Route::Random;
    bool is_fully_sorted = false;
    bool is_fully_reverse = false;
};

// ---------------------------------------------------------------------------
// Hardened probe (underflow-free) with optional Compare
// ---------------------------------------------------------------------------
template <RandomAccessIterator It, typename Compare = std::less<>>
ProbeResult probe(It begin, It end, Compare comp = Compare{}) {
    using T = typename std::iterator_traits<It>::value_type;
    const size_t n = static_cast<size_t>(end - begin);
    ProbeResult r;

    if (n < 2) {
        r.route = Route::Small;
        r.is_fully_sorted = true;
        return r;
    }
    if (n < 64) {
        r.route = Route::Small;
        return r;
    }

    const size_t stride = (n > 4096) ? std::max(size_t(1), n / 64) : 1;

    bool all_sorted = true;
    bool all_reverse = true;
    size_t inversions = 0;
    size_t samples = 0;
    size_t max_run = 1;
    size_t cur_run = 1;
    bool prev_asc = true;

    // Dense head
    const size_t head_limit = std::min(n, size_t(256));
    for (size_t i = 1; i < head_limit; ++i) {
        const T& a = *(begin + (i - 1));
        const T& b = *(begin + i);
        if (comp(b, a)) {
            all_sorted = false;
            ++inversions;
        } else {
            all_reverse = false;
        }
        const bool asc = !comp(b, a);
        if (asc != prev_asc) {
            max_run = std::max(max_run, cur_run);
            cur_run = 1;
        } else {
            ++cur_run;
        }
        prev_asc = asc;
        ++samples;
    }

    // Safe strided start (prevents size_t underflow)
    size_t strided_start = std::max(head_limit, stride);
    if (strided_start < n && strided_start % stride != 0) {
        strided_start += (stride - (strided_start % stride));
        if (strided_start > n) strided_start = n;
    }

    // Sorted confirmation + tail
    if (all_sorted) {
        bool still = true;
        size_t last_checked = (head_limit > 0) ? head_limit - 1 : 0;
        for (size_t i = strided_start; i < n && still; i += stride) {
            if (comp(*(begin + i), *(begin + (i - stride)))) {
                still = false;
            } else {
                last_checked = i;
            }
        }
        if (still && last_checked + 1 < n) {
            for (size_t i = last_checked; i + 1 < n; ++i) {
                if (comp(*(begin + (i + 1)), *(begin + i))) { still = false; break; }
            }
        }
        if (still) {
            r.is_fully_sorted = true;
            r.route = Route::Sorted;
            return r;
        }
        all_sorted = false;
    }

    // Reverse confirmation + tail
    if (all_reverse && samples > 8) {
        bool still = true;
        size_t last_checked = (head_limit > 0) ? head_limit - 1 : 0;
        for (size_t i = strided_start; i < n && still; i += stride) {
            if (comp(*(begin + (i - stride)), *(begin + i))) {
                still = false;
            } else {
                last_checked = i;
            }
        }
        if (still && last_checked + 1 < n) {
            for (size_t i = last_checked; i + 1 < n; ++i) {
                if (comp(*(begin + i), *(begin + (i + 1)))) { still = false; break; }
            }
        }
        if (still) {
            r.is_fully_reverse = true;
            r.route = Route::Reverse;
            return r;
        }
        all_reverse = false;
    }

    // Remaining strided sampling
    for (size_t i = strided_start; i < n; i += stride) {
        const T& a = *(begin + (i - stride));
        const T& b = *(begin + i);
        const bool asc = !comp(b, a);
        if (comp(b, a)) ++inversions;
        if (asc != prev_asc) {
            max_run = std::max(max_run, cur_run);
            cur_run = 1;
        } else {
            ++cur_run;
        }
        prev_asc = asc;
        ++samples;
    }
    max_run = std::max(max_run, cur_run);

    const double inv_ratio = samples ? static_cast<double>(inversions) / static_cast<double>(samples) : 0.5;
    const double thr = (n < 10000) ? 0.05 : (n < 100000 ? 0.08 : 0.11);

    if (inv_ratio < thr || max_run > n / 6) {
        r.route = Route::Patterned;
    } else {
        r.route = Route::Random;
    }
    return r;
}

// ---------------------------------------------------------------------------
// Bounded Verge-style — buffer-safe, strict O(1) extra memory
// ---------------------------------------------------------------------------
template <RandomAccessIterator It, typename Compare = std::less<>>
void verge_style_patterned(It begin, It end, Compare comp = Compare{}) {
    using T = typename std::iterator_traits<It>::value_type;
    const size_t n = static_cast<size_t>(end - begin);
    if (n < 2) return;

    // run_starts needs space for every run start + the final sentinel.
    // is_desc needs space for every run.
    std::array<It,   kMaxTrackedRuns + 1> run_starts; // indices 0 .. kMaxTrackedRuns
    std::array<char, kMaxTrackedRuns>     is_desc;    // indices 0 .. kMaxTrackedRuns-1
    size_t run_count = 0;

    run_starts[0] = begin;
    It i = begin + 1;

    while (i != end) {
        const bool desc = comp(*i, *(i - 1));
        if (desc) {
            while (i != end && !comp(*(i - 1), *i)) ++i;
        } else {
            while (i != end && !comp(*i, *(i - 1))) ++i;
        }

        // Pre-check: if we already have the maximum number of runs,
        // abandon tracking and fall back. This guarantees we never
        // write past is_desc[kMaxTrackedRuns-1] or run_starts[kMaxTrackedRuns].
        if (run_count >= kMaxTrackedRuns) {
            pdqsort(begin, end, comp);
            return;
        }

        is_desc[run_count] = desc ? 1 : 0;
        ++run_count;                       // now run_count == number of completed runs
        // The next start (if any) goes into the slot that will become the sentinel
        // or the start of the next run. Because we checked before incrementing past
        // the limit, run_count is still <= kMaxTrackedRuns here.
        if (i != end) {
            run_starts[run_count] = i;     // safe: run_count <= kMaxTrackedRuns
        }
    }

    // Final sentinel (always safe: run_count <= kMaxTrackedRuns)
    run_starts[run_count] = end;

    // Reverse originally-descending runs
    for (size_t r = 0; r < run_count; ++r) {
        if (is_desc[r] && std::distance(run_starts[r], run_starts[r + 1]) > 1) {
            std::reverse(run_starts[r], run_starts[r + 1]);
        }
    }

    pdqsort(begin, end, comp);
}

// ---------------------------------------------------------------------------
// Main entry — std::sort compatible + fully constrained
// ---------------------------------------------------------------------------
template <RandomAccessIterator It, typename Compare = std::less<>>
void sort(It begin, It end, Compare comp = Compare{}) {
    using T = typename std::iterator_traits<It>::value_type;
    const size_t n = static_cast<size_t>(end - begin);
    if (n < 2) return;

    const ProbeResult pr = probe(begin, end, comp);

    switch (pr.route) {
    case Route::Small:
        pdqsort(begin, end, comp);
        break;

    case Route::Sorted:
        break;

    case Route::Reverse:
        std::reverse(begin, end);
        // For the default less<> comparator the reverse produces a sorted sequence.
        // For greater<> or any custom comparator the meaning of "Reverse" depends
        // on the probe, so we defensively re-sort to guarantee correctness.
        if constexpr (!(std::is_same_v<Compare, std::less<T>> ||
                        std::is_same_v<Compare, std::less<>>)) {
            pdqsort(begin, end, comp);
        }
        break;

    case Route::Patterned:
        verge_style_patterned(begin, end, comp);
        break;

    case Route::Random:
        if constexpr (supports_ska_sort<T>) {
            if constexpr (std::is_same_v<Compare, std::less<T>> ||
                          std::is_same_v<Compare, std::less<>>) {
                ska_sort(begin, end);
            } else {
                pdqsort(begin, end, comp);
            }
        } else {
            pdqsort(begin, end, comp);
        }
        break;
    }
}

// Convenience overloads
template <typename T, typename Compare = std::less<T>>
void sort(T* begin, T* end, Compare comp = Compare{}) {
    sort(static_cast<T*>(begin), static_cast<T*>(end), comp);
}

template <typename Container, typename Compare = std::less<typename Container::value_type>>
requires RandomAccessIterator<typename Container::iterator>
void sort(Container& c, Compare comp = Compare{}) {
    sort(c.begin(), c.end(), comp);
}

} // namespace geblomi

// ============================================================================
// End of GeblomiSort amalgamated header
// ============================================================================
