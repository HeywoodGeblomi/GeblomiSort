# GeblomiSort v2.7.0 — Release Notes

**Date:** 2026-07-31  
**Team:** Grok, Harper, Lucas, Benjamin (Benji), Heywood  
**Intended tag:** `v2.7.0`

Marketing-grade **1-D** adaptive hybrid sort. Timings from release-gate host (`g++ -O3 -std=c++20`, n = 1 000 000 `int`). Absolute ms are machine-dependent.

## Highlights

1. **Organpipe probe fix** — Dual sample/element run counters; element-span max_run only when direction_changes <= 2 (CAP=2).
2. **Joint Merge eligibility gate** — strong runs AND low direction-change rate.
3. **Two-header packaging** — GeblomiSort.hpp + residual_automaton.hpp.
4. **CAP = 2 locked** (Phase 0).

## Efficacy scorecard (reference host)

| Pattern | Geblomi | pdqsort | ska |
|---------|---------|---------|-----|
| Sorted | ~0.01 ms | ~0.7 ms | ~12 ms |
| Reverse | ~0.14 ms | ~1.4 ms | ~14 ms |
| Random | ~15 ms | ~29 ms | ~15 ms |
| Sawtooth | ~13 ms (merge blocked) | ~25 ms | ~13 ms |
| Organpipe | ~1.3–1.5 ms | ~31 ms | ~17 ms |
| 4–8 rev runs | ~11 ms (CAP residual) | ~11 ms | ~13 ms |

## Safety

- Sawtooth: merge_eligible == false
- Organpipe: merge_eligible == true, max_run promoted

## Layout

GeblomiSort.hpp + residual_automaton.hpp (required). benches/release_gate.cpp. experimental/ unsupported.

## Limitations

CAP=2 residual; sampling probe; ska for scalar ints; not multi-D; no world-fastest claim.

*See public/geblomi-sort/RELEASE_NOTES_v2.7.0.md for the full accuracy-pass notes.*
