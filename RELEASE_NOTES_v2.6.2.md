# GeblomiSort v2.6.2 — AmalgaSort Phase 2B

**ResourceAware + HypervolumeProxy packages added. Full 6-package menu live.**

## Packages available

| Package | Role |
|---------|------|
| **ScalarizedPreference** | Default high-ROI (Phase 1) — lowest-cost recruitment of Ska/pdq/Verge talent |
| ParetoDominance | Pure non-dominated filter |
| ConfidenceWeighted | Score scaled by probe confidence |
| Lexicographic | Hard priority: time → adapt → space |
| **ResourceAware** | Stronger penalty on space / pressure (Phase 2B) |
| **HypervolumeProxy** | Approximate contribution to (time, adapt) front (Phase 2B) |

## Safety & design (unchanged)
- **Default package**: `ScalarizedPreference`
- **Activation**: borderline-only (`inv_ratio` in ~0.03–0.15). Clear high-ROI paths (Ska random, early-exit sorted/reverse, Verge patterned) are never disturbed → **zero regression**.
- **Space**: strictly **O(1)** extra memory (stack `ObjVec` only; no allocations).
- Full custom comparator support, C++20, header-only / modular.

## Speed & space vs previous versions

| Metric | v2.5 / v2.6.0 | **v2.6.2** |
|--------|---------------|------------|
| Extra memory | O(1) | **O(1)** (identical) |
| 1M random (default) | ~29 ms | **~29 ms** (parity) |
| 1M sorted | sub-ms early exit | **sub-ms** (parity) |
| 1M reverse | sub-ms early exit | **sub-ms** (parity) |
| Clear-case activations | 0 | **0** |
| Packages | 1 | **6** |

Fresh confirmation (g++ -O3, best-of-3):
- 1M random: ~28.98 ms
- 1M sorted: ~0.011 ms
- 1M reverse: ~0.141 ms
- activations = 0 under default package

Only **borderline** probe results can differ by package choice. Clear-case speed and space efficiency are intentionally identical to Phase 1 / v2.5.

## Controls
```cpp
geblomi::current_package() = geblomi::IncentivePackage::ResourceAware;  // or HypervolumeProxy, etc.
// Counters available when GEBLMI_NO_COUNTERS is not defined
```

## Correctness receipt
- `ALL_CORRECTNESS_OK = 1` across **all 6 packages**
- Distributions: random / sorted / reverse / sawtooth / organ-pipe / nearly-sorted / greater<> / small-N
- Sizes through 1M+
- Borderline region included

## Distribution
- **GitHub modular**: `public/geblomi-sort/GeblomiSort.hpp` + `geblomi_decision.hpp` (Phase 2B)
- **Local pure-inline**: full amalgamated header with inlined decision layer (v2.6.2)

## Next (Phase 3 direction)
- Two-stage probe enrichment
- Additional talent candidates (TimSort-style runs, small-N networks)
- Offline ROI catalog + optional conservative weight adaptation

Team: Grok + Harper + Benjamin + Lucas + Heywood | 2026-07-31
