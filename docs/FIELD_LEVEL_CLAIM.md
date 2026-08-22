# GeblomiSort — Charged Surface (GEB-ACE-001)

**Status:** measured evidence, not a universal-win claim.
**Header:** `public/geblomi-sort/GeblomiSort.hpp` (amalgam; geblomi::sort body unchanged).
**Harness:** `tests/bench.cpp` — N=1e6 `int`, best-of-3 wall ms, `g++ -O2 -std=c++20`.
**Reproduce:** see GitHub Action `ace` / `./bench` after build.

## Charged set

| dist | geblomi | std::sort | pdqsort | ska_sort | vs pdq | vs ska | vs std |
|------|--------:|----------:|--------:|---------:|--------|--------|--------|
| random | 17.4 | 79.0 | 29.6 | 17.5 | **win** | tie | **win** |
| sorted | 0.30 | 13.0 | 1.31 | 12.9 | **win** | **win** | **win** |
| reverse | 0.68 | 9.2 | 2.09 | 14.3 | **win** | **win** | **win** |
| patterned | 32.2 | 81.2 | 31.9 | 17.6 | tie | **loss** | **win** |
| sawtooth | 4.3 | 21.9 | 4.1 | 9.5 | tie | **win** | **win** |

**Verdict rules:** win = ≥1.20× faster; loss = ≥1.20× slower; else tie.
**soft@1.20 losses vs pdq on this host:** 0 / 5.

Numbers above are from the GEB-ACE-001 authoring host (Linux, g++). CI on `ubuntu-latest` will print its own table; absolute ms will differ by machine. Cell *verdicts* (win/tie/loss) are the locked surface language — not a guarantee of fixed wall times.

## What this does **not** claim

- Not "beats pdq everywhere." Patterned loses to ska here; other machines may flip cells.
- Not a multi-type / multi-arch Field-Level claim (that is a later, harder bar).
- Not PhotonicSort; not hardware photonics.
- Correctness is gated by Phase A (`tests/correctness.cpp`), not by this table.

See `NON_CLAIMS.md`.
