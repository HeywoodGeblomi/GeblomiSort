/**
 * geblomi_probe.hpp — Richer Gyro / ProbeResult for GeblomiSort
 * 80/20 enhancement + A+E render: sparse-guard, equal-bias, hierarchical signals
 * Team: Grok + Harper + Benjamin + Lucas + Heywood | 2026-07-31
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <concepts>
#include <functional>
#include <type_traits>

namespace geblomi {

enum class Route { Small, Sorted, Reverse, Patterned, Random };

struct ProbeResult {
    Route route = Route::Random;
    bool is_fully_sorted = false;
    bool is_fully_reverse = false;
    // richer gyro output
    double inv_ratio = 0.5;
    size_t max_run = 1;
    size_t direction_changes = 0;
    size_t equal_count = 0;
    float confidence = 1.0f;
    size_t samples = 0;
    size_t n = 0;
};

template <typename It>
concept RandomAccessIterator = std::random_access_iterator<It>;

template <RandomAccessIterator It, typename Compare = std::less<>>
ProbeResult probe(It begin, It end, Compare comp = Compare{}) {
    using T = typename std::iterator_traits<It>::value_type;
    const size_t n = static_cast<size_t>(end - begin);
    ProbeResult r;
    r.n = n;

    if (n < 2) {
        r.route = Route::Small;
        r.is_fully_sorted = true;
        r.inv_ratio = 0.0;
        r.max_run = n;
        r.confidence = 1.0f;
        return r;
    }
    if (n < 64) {
        r.route = Route::Small;
        r.confidence = 0.8f;
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
    size_t direction_changes = 0;
    size_t equal_count = 0;

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
        if (!comp(a, b) && !comp(b, a)) ++equal_count;
        const bool asc = !comp(b, a);
        if (asc != prev_asc) {
            ++direction_changes;
            max_run = std::max(max_run, cur_run);
            cur_run = 1;
        } else {
            ++cur_run;
        }
        prev_asc = asc;
        ++samples;
    }

    // Sparse-inversion guard (A residual): if head clean and large n, force extra samples
    if (all_sorted && n > 1024 && direction_changes == 0) {
        for (size_t extra : {n/4, n/2, 3*n/4}) {
            if (extra >= 1 && extra < n) {
                if (comp(*(begin + extra), *(begin + (extra - 1)))) {
                    all_sorted = false;
                    ++inversions;
                    ++samples;
                    break;
                }
            }
        }
    }

    // Safe strided start
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
            r.inv_ratio = 0.0;
            r.max_run = n;
            r.direction_changes = 0;
            r.equal_count = equal_count;
            r.samples = samples;
            r.confidence = 1.0f;
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
            r.inv_ratio = 1.0;
            r.max_run = n;
            r.direction_changes = 0;
            r.equal_count = equal_count;
            r.samples = samples;
            r.confidence = 1.0f;
            return r;
        }
        all_reverse = false;
    }

    // Remaining strided sampling
    for (size_t i = strided_start; i < n; i += stride) {
        const T& a = *(begin + (i - stride));
        const T& b = *(begin + i);
        if (comp(b, a)) ++inversions;
        if (!comp(a, b) && !comp(b, a)) ++equal_count;
        const bool asc = !comp(b, a);
        if (asc != prev_asc) {
            ++direction_changes;
            max_run = std::max(max_run, cur_run);
            cur_run = 1;
        } else {
            ++cur_run;
        }
        prev_asc = asc;
        ++samples;
    }
    max_run = std::max(max_run, cur_run);

    double inv_ratio = samples ? static_cast<double>(inversions) / static_cast<double>(samples) : 0.5;
    const double thr = (n < 10000) ? 0.05 : (n < 100000 ? 0.08 : 0.11);

    // Equal-ratio bias for hierarchical / constant disposition (E + A)
    if (samples > 0 && equal_count > samples / 2) {
        inv_ratio *= 0.5; // bias toward Patterned / none-flux
    }

    r.inv_ratio = inv_ratio;
    r.max_run = max_run;
    r.direction_changes = direction_changes;
    r.equal_count = equal_count;
    r.samples = samples;

    if (inv_ratio < thr || max_run > n / 6) {
        r.route = Route::Patterned;
    } else {
        r.route = Route::Random;
    }

    // confidence: higher when decision is clear or high-equal hierarchical signal
    double dist = std::abs(inv_ratio - thr);
    r.confidence = static_cast<float>(std::min(1.0, 0.4 + dist / (thr + 1e-9) * 0.6));
    if (equal_count > samples / 2) r.confidence = std::min(1.0f, r.confidence + 0.2f);

    return r;
}

} // namespace geblomi
