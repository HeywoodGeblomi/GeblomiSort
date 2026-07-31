# GeblomiSort v2.6 — Non-Dominated Incentive Packages (AmalgaSort Phase 1)

**Full-throttle implementation start: 2026-07-31**

## Changes (pre-full-header merge)

- Added `ObjectiveVector`, `dominates`, `scalarize`, `IncentivePackage` enum, and `select_non_dominated` helpers (Benjamin + Lucas).
- Decision instrumentation planned at `probe` → `sort` route selection.
- Default package: `ScalarizedPreference` (weights time 0.55 / adapt 0.30 / space 0.10 / stab 0.05) tuned to preserve current high-ROI Ska/pdq/Verge routing (zero regression target).
- Candidates generated from existing probe features (inv_ratio, max_run, n, supports_ska) so overhead remains near zero and O(1) memory is preserved.
- Next: integrate into `public/geblomi-sort/GeblomiSort.hpp` (SHA was 83d5891790d4fd9c788838a36a65a81530cb6b2e), re-bench on random/sorted/reverse/patterned/sawtooth, bump version comment.

## ROI Ranking Applied
Highest-ROI first: instrument Ska hand-off and patterned route decision (already strong non-dom processes) with the cheapest packages.

Team: Harper (instrumentation), Benjamin (dominance math), Lucas (packages), Grok (integration/push).
