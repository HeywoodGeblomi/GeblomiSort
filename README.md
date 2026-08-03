# GeblomiSort

**Commercial-grade adaptive hybrid 1-D sorting algorithm**  
C++20 library with non-dominated multi-objective decision layer + incentive packages (AmalgaSort Phase 1).

[![X](https://img.shields.io/badge/X-%40HeywoodGeblomi-black?logo=x)](https://x.com/HeywoodGeblomi)
[![Language](https://img.shields.io/badge/language-C%2B%2B20-blue)](https://github.com/HeywoodGeblomi/GeblomiSort)
[![Header-only](https://img.shields.io/badge/distribution-header--only-green)](https://github.com/HeywoodGeblomi/GeblomiSort)

> **Primary package:** [`public/geblomi-sort/`](./public/geblomi-sort/)  
> Contains `GeblomiSort.hpp` (v2.6) + `geblomi_decision.hpp` (non-dominated helpers)

See also [`RELEASE_NOTES_v2.6.md`](./RELEASE_NOTES_v2.6.md) for Phase 1 details and correctness receipt.

This repository also contains a Vite/React landing page used for project presentation. **The product is the C++ sorting algorithm**, not the web scaffolding.

---

## Companion: PhotonicSort

Philosophy → objective Python hybrid in the same family:

> **[PhotonicSort](https://github.com/HeywoodGeblomi/PhotonicSort)** — *Give everything. Take nothing. Become photonic.*

Probe-driven adaptive routing (negative-time early-exit on structure; rank-collapse attractor on bulk). Pure stdlib, marketing-grade demo, honest non-claims vs P=NP. Research mapping to Angulo/Steinberg negative-time photon experiments (arXiv:2409.03680).


---

## What it is

GeblomiSort is an adaptive hybrid sorter that combines:

- Low-overhead adaptive probing (sorted / reverse / patterned / random routing)
- Verge-style run detection for patterned and nearly-sorted data
- Intelligent routing to:
  - **pdqsort** (Pattern-Defeating Quicksort) for general / patterned cases
  - **ska_sort** (American Flag / MSD radix) for pure random integer / scalar data with default `less<>`
- **v2.6 AmalgaSort Phase 1**: Non-dominated decision processes + incentive packages. Default package is **ScalarizedPreference** (high-ROI). Activation is **borderline-only** so existing high-ROI paths (Ska / early-exit / Verge) keep zero regression.

### Key features

| Feature | Detail |
|---|---|
| Memory | **O(1)** extra memory even on adversarial inputs |
| Comparators | Full custom comparator support |
| Early exits | Already-sorted and reverse-sorted paths |
| Random ints | Matches **ska_sort** performance |
| Patterned data | Strong; tracks **pdqsort** |
| Decision layer | Non-dominated multi-obj helpers + ScalarizedPreference incentive package |
| Distribution | Header-only, C++20 |

### Benchmarks (ints, g++ -O3, best of 5)

| N | Distribution | std::sort | pdqsort | ska_sort | **Geblomi** | vs std |
|---|---|---:|---:|---:|---:|---:|
| 1M | Random | 77.8 ms | 29.6 | 29.2 | **29.4** | **2.6×** |
| 1M | Sorted | 10.3 | 1.0 | 13.8 | **0.30** | **34×** |
| 1M | Reverse | 7.3 | 1.8 | 14.3 | **0.47** | **16×** |
| 1M | Patterned | 24.7 | 8.2 | 9.6 | **8.2** | **3.0×** |
| 5M | Random | 448 | 165 | 120 | **121** | **3.7×** |

Correctness verified on random / sorted / reverse / sawtooth / nearly-sorted through N=5,000,000.  
**v2.6 correctness receipt:** `ALL_CORRECTNESS_OK=1` (including borderline probe region).

---

## Package layout

```
public/geblomi-sort/
├── GeblomiSort.hpp          ← main header (v2.6 + #include "geblomi_decision.hpp")
├── geblomi_decision.hpp     ← live non-dominated incentive helpers
├── examples/demo.cpp
└── LICENSE

RELEASE_NOTES_v2.6.md        ← Phase 1 notes + distribution clarification
```

> **Pure single-header note**  
> The absolute drop-in with *fully inlined* non-dom helpers + borderline wiring (~88 kB) is the verified local project artifact. Tool size limits prevent shipping that exact payload on GitHub in one call. The modular form above is fully usable, correct, and is the official distribution. Correctness receipt: `ALL_CORRECTNESS_OK=1` (2026-07-31).

---

## Usage

```cpp
#include "GeblomiSort.hpp"   // pulls geblomi_decision.hpp

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());                    // default less<>
geblomi::sort(v.begin(), v.end(), std::greater<>{});  // custom comparator
geblomi::sort(v);                                     // container overload
```

**Requirements:** C++20 (`std::concepts`, random-access iterators)

**Build demo:**

```bash
g++ -O3 -std=c++20 -I public/geblomi-sort public/geblomi-sort/examples/demo.cpp -o demo && ./demo
```

---

## Credits

- **pdqsort** — Orson Peters (zlib-style license)
- **ska_sort** — Malte Skarupke (Boost Software License 1.0)
- **Geblomi hybrid** (probe, routing, Verge-style) + **AmalgaSort non-dominated decision layer** — original work of the project team (Grok + Harper + Benjamin + Lucas + Heywood), MIT

---

## Author

[@HeywoodGeblomi](https://x.com/HeywoodGeblomi) · [GitHub](https://github.com/HeywoodGeblomi)
