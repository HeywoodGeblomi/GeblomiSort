# GeblomiSort v2.6.2 — AmalgaSort Phase 2B

**Commercial-grade adaptive hybrid 1-D sorting algorithm**  
C++20 header-only library with non-dominated multi-objective decision layer + full incentive package menu.

[![X](https://img.shields.io/badge/X-%40HeywoodGeblomi-black?logo=x)](https://x.com/HeywoodGeblomi)

## Installation

GeblomiSort is **header-only**. No build step or external dependencies beyond a C++20 compiler.

### 1. Get the headers

```bash
git clone https://github.com/HeywoodGeblomi/GeblomiSort.git
# or download the two files from public/geblomi-sort/
```

Required files (this directory):

| File | Role |
|------|------|
| `GeblomiSort.hpp` | Main header |
| `geblomi_decision.hpp` | 6-package non-dominated incentive layer |

### 2. Add to your include path

```bash
# Example: compile with the package directory on the include path
g++ -O3 -std=c++20 -I path/to/public/geblomi-sort your_code.cpp -o your_binary
```

Or simply copy both headers next to your source and `#include "GeblomiSort.hpp"`.

### 3. Use it

```cpp
#include "GeblomiSort.hpp"   // automatically pulls geblomi_decision.hpp

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());                    // default less<>
geblomi::sort(v.begin(), v.end(), std::greater<>{});  // custom comparator
geblomi::sort(v);                                     // container overload
```

### 4. (Optional) Select an incentive package

```cpp
// Runtime
geblomi::g_active_package = geblomi::IncentivePackage::ResourceAware;

// Or compile-time default
// g++ ... -DGEBLMI_DEFAULT_PACKAGE=geblomi::IncentivePackage::HypervolumeProxy
```

Available packages: `ScalarizedPreference` (default), `ParetoDominance`, `ConfidenceWeighted`, `Lexicographic`, `ResourceAware`, `HypervolumeProxy`.

### Requirements
- C++20 compiler (`std::concepts`, random-access iterators)
- Tested with g++ 13+ / clang 15+

### Quick demo

```bash
g++ -O3 -std=c++20 -I. examples/demo.cpp -o demo && ./demo
```

See also [`RELEASE_NOTES_v2.6.2.md`](../../RELEASE_NOTES_v2.6.2.md) for the full package menu and speed/space comparison.

---

## Overview

GeblomiSort is an adaptive hybrid sorter that combines:

- Low-overhead adaptive probing
- Verge-style run detection and reversal for patterned / nearly-sorted data
- Intelligent routing to **pdqsort** and **ska_sort**
- **AmalgaSort decision layer**: Non-dominated multi-objective selection + incentive packages. Default = **ScalarizedPreference**. Activation is **borderline-only** (zero regression on clear high-ROI paths).

### Key Features

- O(1) extra memory even on adversarial inputs
- Full custom comparator support
- Early exits for sorted / reverse data
- Matches Ska Sort on pure random integers
- Strong on patterned / nearly-sorted data
- Header-only, C++20
- Runtime / compile-time package selection + activation counters

## Performance Notes

- Clear high-ROI paths (random / sorted / reverse) are identical in speed and space to prior versions by design.
- Non-dom layer activates only on borderline probe results.
- Strict O(1) auxiliary memory.

## Credits & Licenses

1. **pdqsort** — Orson Peters (zlib-style)
2. **ska_sort** — Malte Skarupke (Boost Software License 1.0)
3. **Geblomi hybrid + AmalgaSort decision layer** — Grok + Harper + Benjamin + Lucas + Heywood (MIT)

## Author

[@HeywoodGeblomi](https://x.com/HeywoodGeblomi)
