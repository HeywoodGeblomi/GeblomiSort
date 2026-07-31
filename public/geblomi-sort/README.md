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
g++ -O3 -std=c++20 -I path/to/public/geblomi-sort your_code.cpp -o your_binary
```

Or copy both headers next to your source and `#include "GeblomiSort.hpp"`.

### 3. Use it

```cpp
#include "GeblomiSort.hpp"

std::vector<int> v = /* ... */;
geblomi::sort(v.begin(), v.end());
geblomi::sort(v.begin(), v.end(), std::greater<>{});
geblomi::sort(v);
```

### 4. (Optional) Select an incentive package

```cpp
geblomi::current_package() = geblomi::IncentivePackage::ResourceAware;
```

Available: `ScalarizedPreference` (default), `ParetoDominance`, `ConfidenceWeighted`, `Lexicographic`, `ResourceAware`, `HypervolumeProxy`.

### Requirements
- C++20 (`std::concepts`, random-access iterators)
- g++ 13+ / clang 15+

### Quick demo
```bash
g++ -O3 -std=c++20 -I. examples/demo.cpp -o demo && ./demo
```

---

## Docker Setup

An **optimized multi-stage Dockerfile** is included in this directory.

- **Builder stage**: `gcc:14` (compiles the demo with static-libstdc++)
- **Runtime stage**: `debian:bookworm-slim` (only the binary + headers → significantly smaller final image)

### Build the image
```bash
# From repository root
docker build -t geblomisort -f public/geblomi-sort/Dockerfile public/geblomi-sort

# Or from this directory
docker build -t geblomisort .
```

### Run the built-in demo
```bash
docker run --rm geblomisort
```

### Compile your own code (use a full gcc image)
```bash
docker run --rm -v "$PWD":/work -w /work gcc:14 \
  g++ -O3 -std=c++20 -I/path/to/headers your_code.cpp -o /tmp/a && /tmp/a
```

The multi-stage design keeps the final runtime image small while still shipping the headers.

---

## Verification

Confirm the install and Docker image work as expected. All smoke tests below include explicit error handling.

### 1. Host compile smoke test (with error handling)

```bash
cd public/geblomi-sort || { echo "ERROR: cannot cd to public/geblomi-sort"; exit 1; }

# Check compiler is present
command -v g++ >/dev/null 2>&1 || { echo "ERROR: g++ not found in PATH"; exit 1; }

cat > /tmp/verify_geblomi.cpp << 'EOF'
#include "GeblomiSort.hpp"
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
int main() {
    std::vector<int> v(1000);
    std::iota(v.rbegin(), v.rend(), 0);   // reverse-sorted
    geblomi::sort(v.begin(), v.end());
    if (!std::is_sorted(v.begin(), v.end())) {
        std::cerr << "FAIL: ascending sort incorrect\n";
        return 1;
    }
    geblomi::sort(v.begin(), v.end(), std::greater<>{});
    if (!std::is_sorted(v.begin(), v.end(), std::greater<>{})) {
        std::cerr << "FAIL: descending sort incorrect\n";
        return 2;
    }
    std::cout << "HOST_OK\n";
    return 0;
}
EOF

if ! g++ -O3 -std=c++20 -I. /tmp/verify_geblomi.cpp -o /tmp/verify_geblomi; then
  echo "ERROR: compilation failed"
  exit 1
fi

if ! /tmp/verify_geblomi; then
  echo "ERROR: smoke test binary failed"
  exit 1
fi

echo "Host smoke test passed."
```

**Expected output:**
```text
HOST_OK
Host smoke test passed.
```
Any other message or non-zero exit indicates a problem (missing headers, wrong include path, broken sort, etc.).

### 2. Docker image smoke test (with error handling)

```bash
command -v docker >/dev/null 2>&1 || { echo "ERROR: docker not found in PATH"; exit 1; }

if ! docker build -t geblomisort -f public/geblomi-sort/Dockerfile public/geblomi-sort; then
  echo "ERROR: docker build failed"
  exit 1
fi

if ! docker run --rm geblomisort; then
  echo "ERROR: docker run (demo) failed"
  exit 1
fi

SIZE=$(docker images geblomisort --format '{{.Size}}' 2>/dev/null || echo "unknown")
echo "Image size: $SIZE"
echo "Docker smoke test passed."
```

**Expected output (illustrative):**
```text
Successfully tagged geblomisort:latest
...demo output...
Image size: 120MB
Docker smoke test passed.
```
Typical size **~80–150 MB**. A ~1 GB+ image means the old single-stage Dockerfile was used. Non-zero exit at any step is a failure.

### 3. (Optional) Package switch check

```cpp
#include "GeblomiSort.hpp"
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

int main() {
    std::vector<int> v(500);
    std::iota(v.rbegin(), v.rend(), 0);

    geblomi::current_package() = geblomi::IncentivePackage::ResourceAware;
    geblomi::sort(v.begin(), v.end());
    if (!std::is_sorted(v.begin(), v.end())) {
        std::cerr << "FAIL: ResourceAware produced unsorted output\n";
        return 1;
    }
    std::cout << "PACKAGE_SWITCH_OK\n";
    return 0;
}
```

**Expected:** `PACKAGE_SWITCH_OK` and exit code 0. All six packages preserve correctness on clear data (borderline-only activation).

See [`RELEASE_NOTES_v2.6.2.md`](../../RELEASE_NOTES_v2.6.2.md) for the full package menu and speed/space comparison.

---

## Overview

GeblomiSort is an adaptive hybrid sorter combining low-overhead probing, Verge-style runs, pdqsort / ska_sort routing, and the AmalgaSort non-dominated decision layer (6 incentive packages). Default = ScalarizedPreference. Activation is borderline-only → zero regression on high-ROI paths.

### Key Features
- O(1) extra memory
- Full custom comparators
- Early exits for sorted / reverse
- Ska-caliber on random integers
- Header-only, C++20
- Runtime package selection + counters

## Performance Notes
Clear high-ROI paths are identical in speed and space to prior versions by design. Decision layer activates only on borderline probe results.

## Credits & Licenses
1. **pdqsort** — Orson Peters (zlib-style)
2. **ska_sort** — Malte Skarupke (Boost 1.0)
3. **Geblomi + AmalgaSort** — Grok + Harper + Benjamin + Lucas + Heywood (MIT)

## Author
[@HeywoodGeblomi](https://x.com/HeywoodGeblomi)
