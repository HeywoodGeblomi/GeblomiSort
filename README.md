# GeblomiSort

**Commercial-grade adaptive hybrid 1-D sorting algorithm**  
Single-header **C++20** library — not a web app.

[![X](https://img.shields.io/badge/X-%40HeywoodGeblomi-black?logo=x)](https://x.com/HeywoodGeblomi)
[![Language](https://img.shields.io/badge/language-C%2B%2B20-blue)](https://github.com/HeywoodGeblomi/GeblomiSort)
[![Header-only](https://img.shields.io/badge/distribution-header--only-green)](https://github.com/HeywoodGeblomi/GeblomiSort/blob/main/GeblomiSort.hpp)

> **Primary artifact:** [`GeblomiSort.hpp`](./GeblomiSort.hpp) (also at [`public/geblomi-sort/GeblomiSort.hpp`](./public/geblomi-sort/GeblomiSort.hpp))

This repository also contains a Vite/React landing page used for project presentation. **The product is the C++ sorting algorithm**, not the web scaffolding.

---

## What it is

GeblomiSort is an adaptive hybrid sorter that combines:

- Low-overhead adaptive probing (sorted / reverse / patterned / random routing)
- Verge-style run detection for patterned and nearly-sorted data
- Intelligent routing to:
  - **pdqsort** (Pattern-Defeating Quicksort) for general / patterned cases
  - **ska_sort** (American Flag / MSD radix) for pure random integer / scalar data with default `less<>`

### Key features

| Feature | Detail |
|---|---|
| Memory | **O(1)** extra memory even on adversarial inputs |
| Comparators | Full custom comparator support |
| Early exits | Already-sorted and reverse-sorted paths |
| Random ints | Matches **ska_sort** performance |
| Patterned data | Strong; tracks **pdqsort** |
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

---

## Usage

```cpp
#include "GeblomiSort.hpp"

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());                    // default less<>
geblomi::sort(v.begin(), v.end(), std::greater<>{});  // custom comparator
geblomi::sort(v);                                     // container overload
```

**Requirements:** C++20 (`std::concepts`, random-access iterators)

**Build demo:**

```bash
g++ -O3 -std=c++20 examples/demo.cpp -o demo && ./demo
```

---

## Package layout

```
GeblomiSort.hpp          ← primary single-header (root)
LICENSE                  ← hybrid MIT + pdqsort zlib + ska Boost notices
examples/demo.cpp        ← working demo
public/geblomi-sort/     ← same package mirrored for the site download
```

---

## Credits

- **pdqsort** — Orson Peters (zlib-style license)
- **ska_sort** — Malte Skarupke (Boost Software License 1.0)
- **Geblomi hybrid** (probe, routing, Verge-style patterned path) — original work, MIT

---

## Author

[@HeywoodGeblomi](https://x.com/HeywoodGeblomi) · [GitHub](https://github.com/HeywoodGeblomi)
