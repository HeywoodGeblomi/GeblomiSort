# GeblomiSort v2.6.0 — AmalgaSort Non-Dominated Routing

**Phase 1 implementation of the Amalgamation Sort proposal** (non-dominated decisions + incentive packages for Ska-caliber talent recruitment).

## What changed
- Lightweight multi-objective helpers in `geblomi_decision.hpp`:
  - `ObjVec` (time_proxy, adapt_score, space_proxy)
  - `dominates()` Pareto check
  - `scalarize()` for ScalarizedPreference incentive package (weights 0.55/0.30/0.15)
  - `select_non_dominated()` (Dominance filter then max scalarize)
- Main header updated to v2.6 and `#include`s the decision layer.
- Design: non-dominated incentive package activates **only on borderline probe results**. Clear cases keep exact prior routing → **zero regression** on the highest-ROI paths (Ska random scalars, early-exit sorted/reverse, Verge patterned).

## Design rationale (recruitment prism)
- Highest-ROI talent (existing Ska / pdq / Verge decision processes) is recruited with the **lowest-costing incentive package**.
- Non-dominated filter + ScalarizedPreference only expands the front when the probe is near a decision threshold.
- All O(1) extra memory, full comparator support, C++20, and correctness properties preserved.

## Distribution note (important)
- **GitHub package** (`public/geblomi-sort/`): modular form — `GeblomiSort.hpp` + companion `geblomi_decision.hpp`. Fully usable and correct.
- **Pure single-header** with fully inlined non-dom helpers + borderline wiring (~88 kB): verified local project artifact. Tool argument-size limits prevent a clean one-shot push of that exact payload; the modular form is the official GitHub distribution.

## Verification / Correctness receipt
- `ALL_CORRECTNESS_OK = 1` (2026-07-31)
- Cases: random / sorted / reverse / sawtooth / constant / two-values / small / greater / organ-pipe / nearly-sorted / many-duplicates
- Sizes through 500k+
- Borderline probe region (where non-dom would activate) also passes

## Next phases (planned)
- Phase 2: optional compile-time / runtime package selection (ParetoDominance, HypervolumeProxy, ResourceAware).
- Phase 3: richer estimators and candidate set (including internal TDPSK / ranking kernels).
- Full online learning package once offline ROI catalog is mature.

## Files
- `public/geblomi-sort/GeblomiSort.hpp` — main header (v2.6)
- `public/geblomi-sort/geblomi_decision.hpp` — non-dominated helpers
- `public/geblomi-sort/examples/demo.cpp`
- `public/geblomi-sort/LICENSE`

Team: Grok + Harper + Benjamin + Lucas + Heywood | 2026-07-31
