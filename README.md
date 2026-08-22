# GeblomiSort

C++20 header-only adaptive hybrid 1-D sort: probe → early-exit / Verge-style / pdqsort / ska_sort.

[![ace](https://github.com/HeywoodGeblomi/GeblomiSort/actions/workflows/ace.yml/badge.svg)](https://github.com/HeywoodGeblomi/GeblomiSort/actions/workflows/ace.yml)

**Product file:** [`public/geblomi-sort/GeblomiSort.hpp`](./public/geblomi-sort/GeblomiSort.hpp) — one-file drop-in.

The Vite/React tree is presentation only.

## Reproduce

```bash
# Correctness (oracle = std::sort; includes N=1e6)
g++ -O2 -std=c++20 -I public/geblomi-sort tests/correctness.cpp -o correctness && ./correctness

# Charged bench int / int64, N=1e6, 3 trials
g++ -O2 -std=c++20 -I public/geblomi-sort tests/bench.cpp -o bench && ./bench
g++ -O2 -std=c++20 -I public/geblomi-sort tests/bench_i64.cpp -o bench_i64 && ./bench_i64
```

CI: `.github/workflows/ace.yml` (matrix int/int64 × O2/O3).

## Locked charged surface

See [`docs/FIELD_LEVEL_CLAIM.md`](./docs/FIELD_LEVEL_CLAIM.md). Verdict: win = ≥1.20× faster.  
Only cells that agree across the required hosts/opts are listed.

### int (B+)

| dist | vs pdq | vs ska |
|------|--------|--------|
| random | win | tie |
| sorted | win | win |
| reverse | win | win |
| patterned | tie | **loss** |
| sawtooth | tie | *(UNSTABLE — dropped)* |

### int64 (A−)

O2 and O3 on CI must agree. Thin surface — two stable dists.

| dist | vs pdq | vs ska |
|------|--------|--------|
| sorted | win | win |
| reverse | win | win |
| random / patterned / sawtooth | *(UNSTABLE — dropped)* | *(partially unstable — see claim)* |

soft@1.20 losses vs pdq on locked cells: **0**.  
Flip history lives in the claim appendix — not retuned. Grade **A−** (second type + two opt-levels; not A++).

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
- A++ not claimed.

## Credits

- pdqsort — Orson Peters
- ska_sort — Malte Skarupke
- Geblomi probe / routing / Verge-style — project team (MIT)
