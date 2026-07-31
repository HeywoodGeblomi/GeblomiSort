/**
 * geblomi_decision.hpp — Non-dominated incentive packages for AmalgaSort Phase 1
 * Part of GeblomiSort v2.6
 *
 * Lightweight multi-objective decision helpers.
 * Default package: ScalarizedPreference (high-ROI, preserves v2.5 strengths).
 * Borderline-only activation recommended for zero regression.
 *
 * Team: Benjamin (dominance) + Lucas (packages) + Harper (instrumentation) + Grok
 * Correctness receipt: unit-tested 2026-07-31 (Python proxy + by-construction legacy path).
 */
#pragma once
#include <cstddef>

namespace geblomi {

enum class IncentivePackage {
    ScalarizedPreference,  // default high-ROI
    ParetoDominance
};

struct ObjVec {
    float time_proxy = 0.f;   // lower better
    float adapt_score = 0.f;  // higher better
    float space_proxy = 0.f;  // lower better (0 for O(1))
};

inline bool dominates(const ObjVec& a, const ObjVec& b) noexcept {
    const bool leq_time  = a.time_proxy  <= b.time_proxy;
    const bool geq_adapt = a.adapt_score >= b.adapt_score;
    const bool leq_space = a.space_proxy <= b.space_proxy;
    const bool strict = (a.time_proxy < b.time_proxy) ||
                        (a.adapt_score > b.adapt_score) ||
                        (a.space_proxy < b.space_proxy);
    return leq_time && geq_adapt && leq_space && strict;
}

inline float scalarize(const ObjVec& v, float w_time = 0.55f, float w_adapt = 0.30f, float w_space = 0.15f) noexcept {
    return -w_time * v.time_proxy + w_adapt * v.adapt_score - w_space * v.space_proxy;
}

template <size_t K>
inline size_t select_non_dominated(const ObjVec (&cands)[K], size_t n_cands) noexcept {
    if (n_cands == 0) return 0;
    bool is_nd[K] = {};
    size_t nd_count = 0;
    for (size_t i = 0; i < n_cands; ++i) {
        is_nd[i] = true;
        for (size_t j = 0; j < n_cands; ++j) {
            if (i != j && dominates(cands[j], cands[i])) {
                is_nd[i] = false;
                break;
            }
        }
        if (is_nd[i]) ++nd_count;
    }
    size_t best = 0;
    float best_score = -1e30f;
    for (size_t i = 0; i < n_cands; ++i) {
        if (nd_count > 0 && !is_nd[i]) continue;
        const float s = scalarize(cands[i]);
        if (s > best_score) {
            best_score = s;
            best = i;
        }
    }
    return best;
}

} // namespace geblomi
