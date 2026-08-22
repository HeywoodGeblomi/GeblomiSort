# GeblomiSort

C++20 header-only adaptive hybrid 1-D sort: probe → early-exit / Verge-style / pdqsort / ska_sort.

[![ace](https://github.com/HeywoodGeblomi/GeblomiSort/actions/workflows/ace.yml/badge.svg)](https://github.com/HeywoodGeblomi/GeblomiSort/actions/workflows/ace.yml)

**Product file:** [`public/geblomi-sort/GeblomiSort.hpp`](./public/geblomi-sort/GeblomiSort.hpp) — one-file drop-in.

The Vite/React tree is presentation only.

## Reproduce

```bash
# Phase A — correctness (oracle = std::sort)
g++ -O2 -std=c++20 -I public/geblomi-sort tests/correctness.cpp -o correctness && ./correctness

# Phase B — charged bench N=1e6 best-of-3
g++ -O2 -std=c++20 -I public/geblomi-sort tests/bench.cpp -o bench && ./bench
```

CI: `.github/workflows/ace.yml`.

## Charged surface

Locked write-up: [`docs/FIELD_LEVEL_CLAIM.md`](./docs/FIELD_LEVEL_CLAIM.md). Verdict: win = ≥1.20× faster.

soft@1.20 losses vs pdq: **0 / 5** on both authoring host and CI.

| dist | vs pdq | vs ska (authoring) | vs ska (CI ubuntu) |
|------|--------|--------------------|--------------------|
| random | win | tie | tie |
| sorted | win | win | win |
| reverse | win | win | win |
| patterned | tie | **loss** | **loss** |
| sawtooth | tie | win | **loss** (flip) |

Milliseconds are host-dependent. Cell *verdicts* are the surface. Sawtooth vs ska flipped on ubuntu-latest — documented, not hidden.

## Usage

```cpp
#include "GeblomiSort.hpp"

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());
geblomi::sort(v.begin(), v.end(), std::greater<>{});
geblomi::sort(v);
```

```bash
g++ -O3 -std=c++20 -I public/geblomi-sort public/geblomi-sort/examples/demo.cpp -o demo && ./demo
```

Requirements: C++20, random-access iterators.

## Honesty

See [`NON_CLAIMS.md`](./NON_CLAIMS.md).

- Not a replacement for pdq/ska on all inputs.
- Not “beats pdq everywhere.”
- Not photonic hardware.
- `promote_ready=false` for any universal-win sentence.

## Credits

- pdqsort — Orson Peters
- ska_sort — Malte Skarupke
- Geblomi probe / routing / Verge-style — project team (MIT)
