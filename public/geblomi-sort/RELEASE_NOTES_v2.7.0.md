# GeblomiSort v2.7.0 — Release Notes

**Date:** 2026-07-31
**Team:** Grok, Harper, Lucas, Benjamin (Benji), Heywood
**Tag:** v2.7.0

## Highlights

1. **Organpipe probe fix** — Dual sample/element run counters; element-span max_run promotion only when direction_changes <= 2. Organpipe ~1.3–1.5 ms at n=1M (was ~30 ms pdq).
2. **Joint Merge eligibility gate** — strong_runs AND low_flips (SHAP-motivated).
3. **Marketing-grade packaging** — Two-header layout (GeblomiSort.hpp + residual_automaton.hpp), release gate, honest limitations.
4. **CAP = 2** locked (Phase 0).

## Efficacy scorecard (n=1M int, -O3)

| Pattern | Result |
|---------|--------|
| Sorted | ~0.01 ms |
| Reverse | ~0.14 ms |
| Random | ~15 ms (≈ ska) |
| Sawtooth | ~13 ms, merge blocked |
| Organpipe | ~1.3–1.5 ms |
| 4–8 rev runs | ~pdq (~11 ms) CAP residual |

## Verify

```bash
g++ -O3 -std=c++20 -I. benches/release_gate.cpp -o release_gate && ./release_gate
# ALL_RELEASE_GATE_OK=1
```
