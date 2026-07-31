/**
 * geblomi_decision.hpp — Non-dominated incentive packages for AmalgaSort
 * Phase 1 + Phase 2A + Phase 2B (ResourceAware, HypervolumeProxy)
 * Default: ScalarizedPreference. Borderline-only recommended for zero regression.
 * Team: Benjamin + Lucas + Harper + Grok | 2026-07-31 KEEP PUSHING HARDER
 */
#pragma once
#include <cstddef>
#include <cstdint>
#include <algorithm>

namespace geblomi {

enum class IncentivePackage : uint8_t {
    ScalarizedPreference = 0,
    ParetoDominance      = 1,
    ConfidenceWeighted   = 2,
    Lexicographic        = 3,
    ResourceAware        = 4,
    HypervolumeProxy     = 5
};

inline IncentivePackage& current_package() noexcept {
    static thread_local IncentivePackage pkg = IncentivePackage::ScalarizedPreference;
    return pkg;
}

#ifndef GEBLMI_NO_COUNTERS
struct PackageCounters {
    uint64_t scalarized = 0;
    uint64_t pareto     = 0;
    uint64_t confidence = 0;
    uint64_t lex        = 0;
    uint64_t resource   = 0;
    uint64_t hypervol   = 0;
    uint64_t borderline_fires = 0;
};
inline PackageCounters& counters() noexcept {
    static thread_local PackageCounters c;
    return c;
}
#endif

struct ObjVec {
    float time_proxy     = 0.f;
    float adapt_score    = 0.f;
    float space_proxy    = 0.f;
    float confidence     = 1.f;
    float resource_proxy = 0.f;
};

inline bool dominates(const ObjVec& a, const ObjVec& b) noexcept {
    const bool leq_time  = a.time_proxy  <= b.time_proxy;
    const bool geq_adapt = a.adapt_score >= b.adapt_score;
    const bool leq_space = a.space_proxy <= b.space_proxy;
    const bool leq_res   = a.resource_proxy <= b.resource_proxy;
    const bool strict = (a.time_proxy < b.time_proxy) ||
                        (a.adapt_score > b.adapt_score) ||
                        (a.space_proxy < b.space_proxy) ||
                        (a.resource_proxy < b.resource_proxy);
    return leq_time && geq_adapt && leq_space && leq_res && strict;
}

inline float scalarize(const ObjVec& v, float w_time = 0.50f, float w_adapt = 0.25f,
                       float w_space = 0.10f, float w_res = 0.15f) noexcept {
    return -w_time * v.time_proxy + w_adapt * v.adapt_score
           - w_space * v.space_proxy - w_res * v.resource_proxy;
}

inline float scalarize_confidence(const ObjVec& v) noexcept {
    return scalarize(v) * (0.25f + 0.75f * v.confidence);
}

inline float scalarize_lex(const ObjVec& v) noexcept {
    return -v.time_proxy * 1e6f + v.adapt_score * 1e3f - v.space_proxy * 10.f - v.resource_proxy;
}

inline float scalarize_resource(const ObjVec& v) noexcept {
    return scalarize(v, 0.40f, 0.20f, 0.10f, 0.30f);
}

inline float hypervolume_contrib(const ObjVec& v) noexcept {
    const float t = std::max(0.f, 1.f - v.time_proxy);
    const float a = std::max(0.f, v.adapt_score);
    return t * a;
}

template <size_t K>
inline size_t select_non_dominated(const ObjVec (&cands)[K], size_t n_cands,
                                   IncentivePackage pkg = IncentivePackage::ScalarizedPreference) noexcept {
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
        float s = 0.f;
        switch (pkg) {
            case IncentivePackage::ParetoDominance:    s = scalarize(cands[i]); break;
            case IncentivePackage::ConfidenceWeighted: s = scalarize_confidence(cands[i]); break;
            case IncentivePackage::Lexicographic:      s = scalarize_lex(cands[i]); break;
            case IncentivePackage::ResourceAware:      s = scalarize_resource(cands[i]); break;
            case IncentivePackage::HypervolumeProxy:   s = hypervolume_contrib(cands[i]); break;
            default:                                   s = scalarize(cands[i]); break;
        }
        if (s > best_score) { best_score = s; best = i; }
    }
#ifndef GEBLMI_NO_COUNTERS
    auto& c = counters();
    switch (pkg) {
        case IncentivePackage::ParetoDominance:    ++c.pareto; break;
        case IncentivePackage::ConfidenceWeighted: ++c.confidence; break;
        case IncentivePackage::Lexicographic:      ++c.lex; break;
        case IncentivePackage::ResourceAware:      ++c.resource; break;
        case IncentivePackage::HypervolumeProxy:   ++c.hypervol; break;
        default:                                   ++c.scalarized; break;
    }
#endif
    return best;
}

template <size_t K>
inline size_t select_non_dominated(const ObjVec (&cands)[K], size_t n_cands) noexcept {
    return select_non_dominated(cands, n_cands, current_package());
}

} // namespace geblomi
