/**
 * residual_automaton.hpp -- Capacity-guard automaton for residual disposition
 * -------------------------------------------------------------------------
 * GeblomiSort v2.7.0 required companion header.
 *
 * Formal residual automaton used by continuous titration / disposition.
 * States represent feasible residual resource occupancy.
 * Transitions represent capacity-safe acceptance of a residual operation class.
 * Construction is offline (or at init); queries are O(1).
 *
 * Team: Grok + Harper + Benjamin + Lucas + Heywood | 2026-07-31
 */
#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>
#include <limits>
#include <cassert>

namespace geblomi {
namespace residual {

using ResourceMask = uint64_t;

struct FU {
    static constexpr ResourceMask Arith0   = 1ull << 0;
    static constexpr ResourceMask Arith1   = 1ull << 1;
    static constexpr ResourceMask Gemm     = 1ull << 2;
    static constexpr ResourceMask Merge    = 1ull << 3;
    static constexpr ResourceMask Probe    = 1ull << 4;
    static constexpr ResourceMask Decision = 1ull << 5;
};

struct StageDemand {
    ResourceMask units = 0;
    uint8_t      cycles = 1;
};

struct ResourceVector {
    static constexpr size_t kMaxStages = 8;
    StageDemand stages[kMaxStages]{};
    uint8_t     num_stages = 0;

    void add_stage(ResourceMask units, uint8_t cycles = 1) noexcept {
        assert(num_stages < kMaxStages);
        stages[num_stages++] = StageDemand{units, cycles};
    }
};

enum class OpClass : uint8_t {
    PanelReflector = 0,
    CompactApply   = 1,
    RunMerge       = 2,
    ProbeSample    = 3,
    DecisionEval   = 4,
    Count          = 5
};

inline ResourceVector demand_PanelReflector() noexcept {
    ResourceVector rv;
    rv.add_stage(FU::Arith0, 10);
    rv.add_stage(FU::Gemm,   10);
    rv.add_stage(FU::Arith1, 10);
    rv.add_stage(FU::Arith0 | FU::Arith1, 2);
    return rv;
}

inline ResourceVector demand_CompactApply() noexcept {
    ResourceVector rv;
    rv.add_stage(FU::Gemm,   28);
    rv.add_stage(FU::Arith0,  8);
    rv.add_stage(FU::Gemm,   28);
    return rv;
}

inline ResourceVector demand_RunMerge() noexcept {
    ResourceVector rv;
    rv.add_stage(FU::Probe, 4);
    rv.add_stage(FU::Merge, 52);
    rv.add_stage(FU::Merge | FU::Arith0, 8);
    return rv;
}

inline ResourceVector demand_ProbeSample() noexcept {
    ResourceVector rv;
    rv.add_stage(FU::Probe, 1);
    return rv;
}

inline ResourceVector demand_DecisionEval() noexcept {
    ResourceVector rv;
    rv.add_stage(FU::Decision, 1);
    rv.add_stage(FU::Decision | FU::Arith0, 1);
    return rv;
}

class Automaton {
public:
    static constexpr size_t kNumClasses = static_cast<size_t>(OpClass::Count);

    Automaton() noexcept {
        set_demand(OpClass::PanelReflector, demand_PanelReflector());
        set_demand(OpClass::CompactApply,   demand_CompactApply());
        set_demand(OpClass::RunMerge,       demand_RunMerge());
        set_demand(OpClass::ProbeSample,    demand_ProbeSample());
        set_demand(OpClass::DecisionEval,   demand_DecisionEval());
    }

    void clear() noexcept { state_ = 0; }
    ResourceMask state() const noexcept { return state_; }

    static ResourceMask footprint(const ResourceVector& rv) noexcept {
        ResourceMask m = 0;
        for (uint8_t i = 0; i < rv.num_stages; ++i)
            m |= rv.stages[i].units;
        return m;
    }

    bool can_accept(OpClass op) const noexcept {
        const ResourceMask need = footprint(demands_[static_cast<size_t>(op)]);
        return (state_ & need) == 0;
    }

    void accept(OpClass op) noexcept {
        assert(can_accept(op));
        state_ |= footprint(demands_[static_cast<size_t>(op)]);
    }

    bool try_accept(OpClass op) noexcept {
        if (!can_accept(op))
            return false;
        accept(op);
        return true;
    }

    void set_demand(OpClass op, ResourceMask units, uint8_t cycles = 1) noexcept {
        ResourceVector rv;
        rv.add_stage(units, cycles);
        demands_[static_cast<size_t>(op)] = rv;
    }

    void set_demand(OpClass op, const ResourceVector& rv) noexcept {
        demands_[static_cast<size_t>(op)] = rv;
    }

    const ResourceVector& demand(OpClass op) const noexcept {
        return demands_[static_cast<size_t>(op)];
    }

private:
    ResourceMask state_ = 0;
    ResourceVector demands_[kNumClasses]{};
};

class CycleGuard {
public:
    explicit CycleGuard(Automaton& a) noexcept : aut_(a) { aut_.clear(); }
    bool can_dispose(OpClass op) const noexcept { return aut_.can_accept(op); }
    bool dispose(OpClass op) noexcept { return aut_.try_accept(op); }
    void clear() noexcept { aut_.clear(); }
    ResourceMask occupancy() const noexcept { return aut_.state(); }
private:
    Automaton& aut_;
};

} // namespace residual
} // namespace geblomi
