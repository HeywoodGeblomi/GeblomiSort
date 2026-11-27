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
python3 bench/meter_header.py
g++ -O2 -std=c++20 -o bench/count_routes bench/count_routes.cpp
./bench/count_routes > bench/oracle.csv
python3 bench/routebench.py --oracle bench/oracle.csv
```

CI: `.github/workflows/ace.yml` (correctness, n=1024 routebench, charged int/int64 × O2/O3).

## Bench (descriptive, n=1024, 210 trials)

`geblomi_full` counts the plain `std::less<>` call (`geblomi::sort(begin, end)`). `geblomi_fallback` is a custom comparator on the same inputs: random scalars then take pdqsort, and a reversed input is re-sorted after `std::reverse`. The previous geblomi row was that fallback (mean_cmps 9258.30).

```
route               mean_cmps    mean_digits    mean_ns    win_rate
pdq_full              7735.44           0.00      11823      0.1429
std_sort_full        12146.99           0.00      15692      0.0000
geblomi_full          4348.68        5283.83      19576      0.8571
geblomi_fallback      9258.30           0.00      12167         n/a
```

Comparisons on the radix route are not the comparison model (digit reads are the cost there, and the log2 n! bound applies only to comparison routes).

Descriptive only. Same machine class as bench/ smoke. Not the N=1e6 charged surface. promote_ready=false for any “beats pdq” sentence. mean_ns is this runner, not portable, and includes the bench meter. mean_digits averages all 210 trials; 120 of them (random, two-value, noisy_ramp, biased_walk) take ska_sort and the other 90 read no digits. win_rate = fraction of trials with strictly fewest comparator calls among pdq_full, std_sort_full, and geblomi_full (ties: pdq, then std, then geblomi — see bench/README.md). It does not rank digit reads. Those 120 radix trials are where geblomi_full records the fewest comparator calls; the other geblomi_full wins are the 30 sorted and 30 reverse trials, and pdq_full wins the 30 organ-pipe trials.

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

## Where this sits: duplicate detection and lower bounds

Checking whether a list has any repeats ("element distinctness") has a standard baseline: **sort, then compare neighbors**. A sorter like this one is the engine inside that baseline.

In November 2026 OpenAI posted a preprint, [The matrix multiplication exponent is nine fourths](https://github.com/openai/math/tree/main/preprints/The-matrix-multiplication-exponent-is-nine-fourths-November-24-2026) ([PDF](https://raw.githubusercontent.com/openai/math/main/preprints/The-matrix-multiplication-exponent-is-nine-fourths-November-24-2026/paper.pdf)). Its proof starts from this exact baseline. It builds a sort-then-compare-neighbors network for distinctness (Section 2.2), then argues that a faster matrix multiplication would compress such networks below a proven lower bound for testing many lists at once.

What that does and does not say about this project:

- The bound lives in an algebraic and tensor-network model: arithmetic circuits over a field, and networks of small bit tables. It is not a bound on a CPU running integer or string keys.
- So it **neither confirms nor rules out** any bit-level sorter speed. Radix-style and adaptive routes are outside what it measures. GeblomiSort routes on a probe, then early-exit, Verge-style, pdqsort or ska_sort (see "Routes on" above), and its claims stay the measured cells in the tables above.
- The lower bound it uses for distinctness is small (a power of log n above linear). It does not say sorting must cost n log n in this setting.
- The preprint is **not peer reviewed**. Its Lean check covers the division-free exponent statement, not every step. One outside audit of the Lean statement was run by AI agents, not people. Treat the result as a strong claim, not settled fact.
- Nothing here changes the claims or non-claims of this repo. Measured results stay measured results.

Author: Heywood Geblomi

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
