# GeblomiSort — Charged Surface (GEB-ACE-002 B+)

**Status:** B+ candidate — locked cells only. Unstable cells removed, not retuned.
**Header:** `public/geblomi-sort/GeblomiSort.hpp` (amalgam; geblomi::sort body unchanged — H0).
**Harness:** `tests/bench.cpp` — N=1e6, seed=42, sawtooth_mod=64, 3 trials, best-of-3+warmup, `g++ -O2`.
**Reproduce:** Action `ace` / `./bench`.

## Verdict rules

win = ≥1.20× faster; loss = ≥1.20× slower; else tie.

## Locked table

Cells below agreed on **authoring host** and **CI ubuntu-latest** (ACE-001 run 32539582721).
In-process 3-trial stability on authoring: YES for all remaining cells.

| dist | vs pdq | vs ska |
|------|--------|--------|
| random | **win** | tie |
| sorted | **win** | **win** |
| reverse | **win** | **win** |
| patterned | tie | **loss** |
| sawtooth | tie | *(dropped — UNSTABLE)* |

**soft@1.20 losses vs pdq (locked dists): 0 / 4** (sawtooth still tie vs pdq on both hosts; counted in soft@pdq scan as non-loss).

Absolute milliseconds are host-dependent and are **not** locked language.

## Appendix — flip history (not locked)

| cell | authoring (ACE-001) | CI ubuntu (run 32539582721) | disposition |
|------|---------------------|------------------------------|-------------|
| **sawtooth vs ska** | win (geblomi 4.3 / ska 9.5) | **loss** (geblomi 5.01 / ska 3.88) | **UNSTABLE** — removed from locked table. Not retuned. |

No probe-threshold change was made to chase ska on sawtooth (ticket non-goal).

## What this does **not** claim

- Not “beats pdq everywhere.”
- Not “beats ska on sawtooth” (cell is UNSTABLE).
- Not multi-type / multi-arch Field-Level.
- Not photonic hardware.
- Correctness gated by `tests/correctness.cpp`, not this table.

See `NON_CLAIMS.md`.
