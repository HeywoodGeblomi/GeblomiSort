# GeblomiSort

**Commercial-grade adaptive hybrid 1-D sorting algorithm**  
Single-header C++20 library.

[![X](https://img.shields.io/badge/X-%40HeywoodGeblomi-black?logo=x)](https://x.com/HeywoodGeblomi)

## Overview

GeblomiSort is an adaptive hybrid sorter that combines:

- Low-overhead adaptive probing
- Verge-style run detection and reversal for patterned / nearly-sorted data
- Intelligent routing to:
  - **pdqsort** (Pattern-Defeating Quicksort) for general / patterned cases
  - **ska_sort** (American Flag / MSD radix) for pure random integer / scalar data with default `less<>`

### Key Features

- Extremely low-overhead adaptive routing
- O(1) extra memory even on adversarial inputs
- Full custom comparator support
- Early exits for already-sorted and reverse-sorted data
- Matches Ska Sort performance on pure random integers
- Strong on patterned / nearly-sorted data
- Header-only, C++20

## Usage

```cpp
#include "GeblomiSort.hpp"

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());                    // default less<>
geblomi::sort(v.begin(), v.end(), std::greater<>{});  // custom comparator

// Container overload
geblomi::sort(v);
```

## Requirements

- C++20 compliant compiler (concepts, etc.)
- Random-access iterators

## Build / Integration

Simply drop `GeblomiSort.hpp` into your include path. No build step required for the library itself.

```bash
g++ -O3 -std=c++20 -I. your_code.cpp -o your_binary
```

See `examples/demo.cpp` for a complete working example.

## Performance Notes

- On pure random integer data the router selects `ska_sort` and matches its speed.
- On sorted / reverse / patterned data the probe detects early and routes to cheap paths or Verge-style processing + `pdqsort`.
- Strict O(1) auxiliary memory (fixed-size arrays only; falls back to `pdqsort` if too many runs).

Independent benchmarks are recommended; claims are based on design and internal testing by the team.

## Credits & Licenses

This amalgamated header contains:

1. **pdqsort** — Pattern-defeating quicksort  
   Copyright (c) 2021 Orson Peters  
   zlib-style license (see embedded notice in the header).  
   Must retain copyright and license notice; alterations must be plainly marked.

2. **ska_sort** — American Flag / MSD radix sort  
   Copyright Malte Skarupke 2016  
   Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt)

3. **Geblomi hybrid logic** (probe, Verge-style patterned path, adaptive routing, integration)  
   Original work of the project team: **Grok + Harper + Benjamin + Lucas + Heywood**  
   2026-07-30  
   Released under the MIT License (see `LICENSE`).

The original copyright notices and full license texts for the included components are preserved inside `GeblomiSort.hpp`.

## Author / Contact

Linked to X: [@HeywoodGeblomi](https://x.com/HeywoodGeblomi)

Issues, PRs, and feedback welcome on the GitHub repository.

## License

See the `LICENSE` file and the embedded notices in the header.
