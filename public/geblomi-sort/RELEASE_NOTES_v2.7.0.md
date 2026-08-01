# GeblomiSort v2.7.0 — Release Notes

**Date:** 2026-07-31  
**Team:** Grok, Harper, Lucas, Benjamin (Benji), Heywood  
**Intended tag:** `v2.7.0`

Marketing-grade **1-D** adaptive hybrid sort. Timings below are from the release-gate host (`g++ -O3 -std=c++20`, n = 1 000 000 `int`, best-of-several). Absolute milliseconds are machine-dependent; relative ordering is the claim that matters.

---

## Highlights

1. **Organpipe probe fix** — Dual sample/element run counters. Element-span `max_run` is used only when `direction_changes <= 2` (CAP = 2), with an optional mid-span adjacent monotone check. Restores Verge on organpipe-class inputs without reopening sawtooth false positives.
2. **Joint Merge eligibility gate** — Verge/RunMerge only when **strong runs and low direction-change rate** both hold (SHAP-motivated conjunction). Prevents Merge over-trigger on high-flip patterns such as sawtooth.
3. **Packaging** — Two required headers: `GeblomiSort.hpp` + `residual_automaton.hpp`. Includes `benches/release_gate.cpp` and honest limitations.
4. **Policy freeze** — CAP = 2 locked for this tag (Phase 0). No CAP raise in v2.7.0.

---

## Efficacy scorecard (reference host)

| Pattern | Geblomi route | Geblomi | pdqsort | ska_sort |
|---------|---------------|---------|---------|----------|
| Sorted | Sorted (early exit) | **~0.01 ms** | ~0.7 ms | ~12 ms |
| Reverse | Reverse | **~0.14 ms** | ~1.4 ms | ~14 ms |
| Random | Random → ska | **~15 ms** | ~29 ms | **~15 ms** |
| Sawtooth | Random; merge **blocked** | **~13 ms** | ~25 ms | **~13 ms** |
| Organpipe | Patterned; merge **eligible** | **~1.3–1.5 ms** | ~31 ms | ~17 ms |
| 2 reversed long runs | Patterned; eligible | **~1.7 ms** | ~2.4 ms | ~12 ms |
| 4–8 reversed long runs | Patterned; CAP residual | **~11 ms** | **~11 ms** | ~13 ms |

**Interpretation**

- None-flux (sorted/reverse): large win vs full-work baselines.
- Random / sawtooth: matches standalone ska; about 2× pdqsort on this host.
- Organpipe: ~20× faster than pdqsort after the probe fix (Verge path).
- 4–8 long runs: CAP = 2 does **not** promote element `max_run`; behavior ties residual pdqsort. Documented residual, not a regression vs pdq.

---

## Safety invariants (release gate)

| Check | Expected |
|-------|----------|
| Sawtooth `merge_eligible` | **false** |
| Sawtooth `max_run` | stays small (no element-unit false positive) |
| Organpipe `merge_eligible` | **true** |
| Organpipe `max_run` | promoted (element span, CAP = 2 path) |
| Correctness | sorted / reverse / random / sawtooth / organpipe / rev_runs 2/4/8 |

```bash
g++ -O3 -std=c++20 -I. benches/release_gate.cpp -o release_gate && ./release_gate
# Expect: ALL_RELEASE_GATE_OK=1
```

---

## Layout

| File | Role |
|------|------|
| `GeblomiSort.hpp` | Amalgam: probe, routing, Verge, pdqsort, ska |
| `residual_automaton.hpp` | **Required** capacity-guard companion |
| `benches/release_gate.cpp` | Correctness + safety + timing smoke |
| `examples/basic.cpp` | Minimal usage |
| `experimental/` | **Unsupported** scaffolds (e.g. 2-D) |

---

## Limitations

1. **CAP = 2** — Element-span run promotion only for ≤ 2 direction changes in the probe sample path. Multi-run structure with more flips often falls through to pdqsort.
2. **Probe is sampling-based** — Adversarial layouts can mis-route; residual pdqsort still sorts correctly.
3. **Ska path** — Aimed at scalar integers with default ordering; custom comparators use pdqsort.
4. **Not a multi-D product** — 2-D / Implosion material under `experimental/` is not part of the supported API.
5. **No “world’s fastest” claim** — Wins are pattern-specific and measured against pdqsort and ska on the listed shapes.

---

## Upgrade from v2.6.x

- Put **both** `GeblomiSort.hpp` and `residual_automaton.hpp` on the include path.
- API for `geblomi::sort` is intentionally unchanged.
- Expect much faster organpipe-class inputs; random and sorted behavior should remain in the same class as before.

---

## License / attribution

- **pdqsort** — Orson Peters (zlib-style notice retained in the amalgam)
- **ska_sort** — Malte Skarupke lineage as vendored (notice retained)
- **Geblomi** probe, routing, Verge policy, joint Merge gate, residual automaton — project team (Grok, Harper, Lucas, Benjamin, Heywood)

---

*Numbers are representative of the release-gate host, not a guarantee on every CPU or compiler.*
