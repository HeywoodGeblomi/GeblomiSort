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

Confirm the install and Docker image work as expected. All smoke tests include **error handling**, **timeouts**, and **retry logic** (2–3 attempts) for transient failures.

### 1. Host compile smoke test (error handling + timeout + retry)

```bash
cd public/geblomi-sort || { echo "ERROR: cannot cd to public/geblomi-sort"; exit 1; }

command -v g++ >/dev/null 2>&1 || { echo "ERROR: g++ not found in PATH"; exit 1; }
command -v timeout >/dev/null 2>&1 || { echo "ERROR: timeout command not found"; exit 1; }

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

# Retry compile up to 3 times
COMPILE_OK=0
for attempt in 1 2 3; do
  if timeout 30s g++ -O3 -std=c++20 -I. /tmp/verify_geblomi.cpp -o /tmp/verify_geblomi; then
    COMPILE_OK=1
    break
  fi
  echo "WARN: compile attempt $attempt failed or timed out; retrying..."
  sleep 2
done
if [ "$COMPILE_OK" -ne 1 ]; then
  echo "ERROR: compilation failed after 3 attempts"
  exit 1
fi

# Retry run up to 3 times
RUN_OK=0
for attempt in 1 2 3; do
  if timeout 10s /tmp/verify_geblomi; then
    RUN_OK=1
    break
  fi
  echo "WARN: run attempt $attempt failed or timed out; retrying..."
  sleep 1
done
if [ "$RUN_OK" -ne 1 ]; then
  echo "ERROR: smoke test binary failed after 3 attempts"
  exit 1
fi

echo "Host smoke test passed."
```

**Expected output:**
```text
HOST_OK
Host smoke test passed.
```
Any final ERROR or non-zero exit indicates a real problem (retries exhausted).

### 2. Docker image smoke test (error handling + timeout + retry)

```bash
command -v docker >/dev/null 2>&1 || { echo "ERROR: docker not found in PATH"; exit 1; }
command -v timeout >/dev/null 2>&1 || { echo "ERROR: timeout command not found"; exit 1; }

# Retry docker build up to 3 times
BUILD_OK=0
for attempt in 1 2 3; do
  if timeout 180s docker build -t geblomisort -f public/geblomi-sort/Dockerfile public/geblomi-sort; then
    BUILD_OK=1
    break
  fi
  echo "WARN: docker build attempt $attempt failed or timed out; retrying..."
  sleep 5
done
if [ "$BUILD_OK" -ne 1 ]; then
  echo "ERROR: docker build failed after 3 attempts"
  exit 1
fi

# Retry docker run up to 3 times
RUN_OK=0
for attempt in 1 2 3; do
  if timeout 30s docker run --rm geblomisort; then
    RUN_OK=1
    break
  fi
  echo "WARN: docker run attempt $attempt failed or timed out; retrying..."
  sleep 2
done
if [ "$RUN_OK" -ne 1 ]; then
  echo "ERROR: docker run (demo) failed after 3 attempts"
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
Typical size **~80–150 MB**. Retries absorb transient daemon/network hiccups; final ERROR means a real failure.

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

Compile/run with the same timeout + retry pattern as the host smoke test. **Expected:** `PACKAGE_SWITCH_OK` and exit code 0.

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
