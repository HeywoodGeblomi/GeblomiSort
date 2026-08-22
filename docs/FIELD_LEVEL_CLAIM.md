# GeblomiSort — Charged Surface (GEB-ACE-002 B+ / A)

**Status:** B+ on main; Phase A candidate.
**Header:** `public/geblomi-sort/GeblomiSort.hpp` (H0 — body unchanged).
**Harness:** `tests/bench.cpp` (int), `tests/bench_i64.cpp` (int64) — N=1e6, seed=42, 3 trials.
**CI:** Action run [32541185749](https://github.com/HeywoodGeblomi/GeblomiSort/actions/runs/32541185749) — all green.

## Verdict rules

win = ≥1.20× faster; loss = ≥1.20× slower; else tie.

## Locked table — int (B+)

| dist | vs pdq | vs ska |
|------|--------|--------|
| random | **win** | tie |
| sorted | **win** | **win** |
| reverse | **win** | **win** |
| patterned | tie | **loss** |
| sawtooth | tie | *(UNSTABLE — dropped)* |

## Locked table — int64 (Phase A)

Only cells where **CI O2 and CI O3 agree** (3-trial stable). Authoring-only wins that flip on CI are not locked.

| dist | vs pdq | vs ska |
|------|--------|--------|
| random | *(UNSTABLE O2 win / O3 tie)* | tie |
| sorted | **win** | **win** |
| reverse | **win** | **win** |
| patterned | tie | *(UNSTABLE O2 loss / O3 tie)* |
| sawtooth | tie | *(UNSTABLE O2 tie / O3 win)* |

## Appendix — UNSTABLE history

| cell | evidence | disposition |
|------|----------|-------------|
| int sawtooth vs ska | authoring win; CI loss | dropped (B+) |
| int64 random vs pdq | CI O2 **win**, CI O3 **tie** | dropped |
| int64 patterned vs ska | CI O2 **loss**, CI O3 **tie** | dropped |
| int64 sawtooth vs ska | authoring **win**; CI O2 **tie**, CI O3 **win** | dropped |

No probe retune. H0 held. A++ off.

## What this does **not** claim

- Not “beats pdq everywhere.”
- Not multi-arch Field-Level.
- Not photonic hardware.
- Correctness gated by `tests/correctness.cpp` (incl. N=1e6).

See `NON_CLAIMS.md`.
