# GeblomiSort — Charged Surface (GEB-ACE-001)

**Status:** measured evidence, not a universal-win claim. Grade **B** (not B+): one cell flipped on CI.
**Header:** `public/geblomi-sort/GeblomiSort.hpp` (amalgam; geblomi::sort body unchanged).
**Harness:** `tests/bench.cpp` — N=1e6 `int`, best-of-3 wall ms, `g++ -O2 -std=c++20`.
**Reproduce:** GitHub Action `ace` / `./bench` after build.

## Verdict rules

win = ≥1.20× faster; loss = ≥1.20× slower; else tie.

## Authoring host (local)

| dist | geblomi | std::sort | pdqsort | ska_sort | vs pdq | vs ska | vs std |
|------|--------:|----------:|--------:|---------:|--------|--------|--------|
| random | 17.4 | 79.0 | 29.6 | 17.5 | **win** | tie | **win** |
| sorted | 0.30 | 13.0 | 1.31 | 12.9 | **win** | **win** | **win** |
| reverse | 0.68 | 9.2 | 2.09 | 14.3 | **win** | **win** | **win** |
| patterned | 32.2 | 81.2 | 31.9 | 17.6 | tie | **loss** | **win** |
| sawtooth | 4.3 | 21.9 | 4.1 | 9.5 | tie | **win** | **win** |

soft@1.20 losses vs pdq: **0 / 5**.

## CI `ubuntu-latest` (Action run 32539582721)

| dist | geblomi | std::sort | pdqsort | ska_sort | vs pdq | vs ska | vs std |
|------|--------:|----------:|--------:|---------:|--------|--------|--------|
| random | 14.66 | 70.68 | 29.18 | 14.66 | **win** | tie | **win** |
| sorted | 0.12 | 13.29 | 1.22 | 7.51 | **win** | **win** | **win** |
| reverse | 0.48 | 9.52 | 2.10 | 7.83 | **win** | **win** | **win** |
| patterned | 32.42 | 75.85 | 32.00 | 9.34 | tie | **loss** | **win** |
| sawtooth | 5.01 | 20.81 | 4.70 | 3.88 | tie | **loss** | **win** |

soft@1.20 losses vs pdq: **0 / 5**.

## Cell flip (not hidden)

| cell | authoring | CI ubuntu |
|------|-----------|-----------|
| **sawtooth vs ska** | **win** | **loss** |

Absolute ms differ by machine (expected). The locked language is the *verdict*. Because sawtooth vs ska flipped, authoring and CI do **not** match cell-for-cell → grade stays **B**, not B+.

## What this does **not** claim

- Not "beats pdq everywhere."
- Not "beats ska on sawtooth everywhere" — CI says otherwise.
- Not a multi-type / multi-arch Field-Level claim.
- Not PhotonicSort; not hardware photonics.
- Correctness is gated by Phase A (`tests/correctness.cpp`), not by this table.

See `NON_CLAIMS.md`.
