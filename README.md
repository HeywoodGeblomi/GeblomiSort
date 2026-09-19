# GeblomiSort

C++20 header-only adaptive hybrid 1-D sort.

**Include:** `#include "GeblomiSort.hpp"` with `-I public/geblomi-sort`

**Routes on:** a short probe then early-exit / Verge-style / pdqsort / ska_sort.

**Does not claim:** not photonic, no χ, not “beats pdq everywhere.”

[![ace](https://github.com/HeywoodGeblomi/GeblomiSort/actions/workflows/ace.yml/badge.svg)](https://github.com/HeywoodGeblomi/GeblomiSort/actions/workflows/ace.yml)

**Product file:** [`public/geblomi-sort/GeblomiSort.hpp`](./public/geblomi-sort/GeblomiSort.hpp) — one-file drop-in.

This repository is the header, tests, and CI.

## Reproduce

```bash
# Correctness (oracle = std::sort; includes N=1e6)
g++ -O2 -std=c++20 -I public/geblomi-sort tests/correctness.cpp -o correctness && ./correctness

# Charged bench int / int64, N=1e6, 3 trials
g++ -O2 -std=c++20 -I public/geblomi-sort tests/bench.cpp -o bench && ./bench
g++ -O2 -std=c++20 -I public/geblomi-sort tests/bench_i64.cpp -o bench_i64 && ./bench_i64

# Descriptive 210-trial tape (n=1024). From repo root (see bench/README.md)
g++ -O2 -std=c++20 -o bench/count_routes bench/count_routes.cpp
./bench/count_routes > bench/oracle.csv
python3 bench/routebench.py --oracle bench/oracle.csv
```

CI: `.github/workflows/ace.yml` (correctness, n=1024 routebench, charged int/int64 × O2/O3).

## Bench (descriptive, n=1024, 210 trials)

```
route          mean_cmps    mean_ns    win_rate
pdq_full         7735.44      14081     0.8095
geblomi_full     9258.30      15190     0.1429
std_sort_full   12146.99      20234     0.0476
```

Descriptive only. Same machine class as bench/ smoke. geblomi is between pdq and std::sort on this tape (more cmps than pdq). Not the N=1e6 charged surface. promote_ready=false for any “beats pdq” sentence.

mean_ns is this runner, not portable. ska not on this tape. win_rate = fraction of trials with strictly fewest cmps (ties: pdq, then std, then geblomi — see bench/README.md).

## Locked charged surface

N=1e6 locked cells — different suite than the table above.

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
g++ -O3 -std=c++20 -I public/geblomi-sort examples/demo.cpp -o demo && ./demo
```

Requirements: C++20, random-access iterators.

## Honesty

See [`NON_CLAIMS.md`](./NON_CLAIMS.md).

- Not a replacement for pdq/ska on all inputs.
- Not “beats pdq everywhere.”
- Not photonic hardware.
- `promote_ready=false` for any universal-win sentence.
- A++ not claimed.

## License

**Dual licensed.**

- Non-commercial / research / evaluation / non-production → **AGPLv3** (see [LICENSE](LICENSE) and [LICENSE-AGPL](LICENSE-AGPL))
- Any commercial use, production deployment, embedding, SaaS, or redistribution as product → **requires a commercial license** from the copyright holder (see [LICENSE-COMMERCIAL](LICENSE-COMMERCIAL) and [COMMERCIAL.md](COMMERCIAL.md)).

Copyright (c) 2026 Heywood Geblomi.

Third-party components (pdqsort, ska_sort) retain their original licenses — see [NOTICE](NOTICE).

## Credits

- pdqsort — Orson Peters
- ska_sort — Malte Skarupke
- Geblomi probe / routing / Verge-style — project team
