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
geblomi::g_active_package = geblomi::IncentivePackage::ResourceAware;
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

Confirm the install and Docker image work as expected. Smoke tests include:
- Error handling + timeouts
- **AWS Full Jitter** retry (sub-second)
- **Token-bucket retry quota** (AWS Standard-style circuit-breaking)

| Step | Timeout | Max attempts | Backoff | Quota |
|------|---------|--------------|---------|-------|
| Host compile | 30s | 3 | Full Jitter 1s/8s | shared bucket |
| Host run | 10s | 2 | Full Jitter 1s/8s | shared bucket |
| Docker build | 180s | 3 | Full Jitter 1s/8s | shared bucket |
| Docker run | 30s | 2 | Full Jitter 1s/8s | shared bucket |

> **Note:** `timeout` is GNU coreutils (Linux). On macOS install `coreutils` or use `gtimeout`.

### Implementation snippet — Bash

```bash
#!/usr/bin/env bash
# GeblomiSort smoke-test helpers
# - AWS Full Jitter (sub-second): sleep = Uniform(0, min(cap, base*2^(attempt-1)))
# - Token-bucket retry quota (AWS Standard-style circuit breaker)

RETRY_BUCKET_CAPACITY=20
RETRY_BUCKET_TOKENS=20
RETRY_TOKEN_COST=5
RETRY_TOKEN_REFILL=2

retry_quota_available() { (( RETRY_BUCKET_TOKENS >= RETRY_TOKEN_COST )); }

retry_quota_consume() {
  RETRY_BUCKET_TOKENS=$(( RETRY_BUCKET_TOKENS - RETRY_TOKEN_COST ))
  echo "retry quota: spent ${RETRY_TOKEN_COST}, remaining ${RETRY_BUCKET_TOKENS}/${RETRY_BUCKET_CAPACITY}"
}

retry_quota_refill() {
  RETRY_BUCKET_TOKENS=$(( RETRY_BUCKET_TOKENS + RETRY_TOKEN_REFILL ))
  if (( RETRY_BUCKET_TOKENS > RETRY_BUCKET_CAPACITY )); then
    RETRY_BUCKET_TOKENS=$RETRY_BUCKET_CAPACITY
  fi
}

sleep_full_jitter() {
  local attempt=$1 base=${2:-1} cap=${3:-8}
  local exp=$(( base * (1 << (attempt - 1)) )) temp=$exp
  (( temp > cap )) && temp=$cap
  local delay
  if command -v awk >/dev/null 2>&1; then
    delay=$(awk -v t="$temp" 'BEGIN { srand(); printf "%.3f", rand() * t }')
  else
    local ms=0; (( temp > 0 )) && ms=$(( RANDOM % (temp * 1000 + 1) ))
    printf -v delay "%d.%03d" $((ms / 1000)) $((ms % 1000))
  fi
  echo "Full Jitter backoff ${delay}s (attempt=$attempt, temp=$temp, cap=$cap)"
  sleep "$delay"
}

retry() {
  local tries=$1; shift; local n=1
  until "$@"; do
    if (( n >= tries )); then
      echo "ERROR: failed after ${tries} attempt(s): $*"; return 1
    fi
    if ! retry_quota_available; then
      echo "ERROR: retry quota exhausted (tokens=${RETRY_BUCKET_TOKENS}, cost=${RETRY_TOKEN_COST}) — circuit open"
      return 1
    fi
    retry_quota_consume
    echo -n "WARN: attempt $n failed — "
    sleep_full_jitter "$n" 1 8
    ((n++)) || true
  done
  retry_quota_refill
  return 0
}
```

### Implementation snippet — Python Token Bucket

Equivalent helpers in pure Python 3 (stdlib only):

```python
#!/usr/bin/env python3
"""GeblomiSort smoke-test helpers — Full Jitter + token-bucket retry quota."""

from __future__ import annotations

import random
import subprocess
import time
from dataclasses import dataclass, field
from typing import Callable, Sequence


@dataclass
class TokenBucket:
    """AWS Standard-style retry quota (circuit breaker)."""
    capacity: int = 20
    tokens: int = 20
    cost: int = 5
    refill: int = 2

    def available(self) -> bool:
        return self.tokens >= self.cost

    def consume(self) -> None:
        if not self.available():
            raise RuntimeError(
                f"retry quota exhausted (tokens={self.tokens}, cost={self.cost}) — circuit open"
            )
        self.tokens -= self.cost
        print(f"retry quota: spent {self.cost}, remaining {self.tokens}/{self.capacity}")

    def on_success(self) -> None:
        self.tokens = min(self.capacity, self.tokens + self.refill)


def sleep_full_jitter(attempt: int, base: float = 1.0, cap: float = 8.0) -> None:
    """AWS Full Jitter: sleep ~ Uniform(0, min(cap, base * 2**(attempt-1)))."""
    temp = min(cap, base * (2 ** (attempt - 1)))
    delay = random.uniform(0.0, temp)
    print(f"Full Jitter backoff {delay:.3f}s (attempt={attempt}, temp={temp}, cap={cap})")
    time.sleep(delay)


def retry(
    tries: int,
    fn: Callable[[], None],
    *,
    bucket: TokenBucket | None = None,
    label: str = "command",
) -> None:
    """Run fn up to `tries` times with token-bucket + Full Jitter between failures."""
    bucket = bucket or TokenBucket()
    last_exc: BaseException | None = None

    for attempt in range(1, tries + 1):
        try:
            fn()
            bucket.on_success()
            return
        except BaseException as exc:  # noqa: BLE001 — surface any failure as retryable
            last_exc = exc
            if attempt >= tries:
                break
            if not bucket.available():
                raise RuntimeError(
                    f"retry quota exhausted (tokens={bucket.tokens}, cost={bucket.cost}) — circuit open"
                ) from exc
            bucket.consume()
            print(f"WARN: attempt {attempt} failed ({label}): {exc} — ", end="")
            sleep_full_jitter(attempt)

    raise RuntimeError(f"failed after {tries} attempt(s): {label}") from last_exc


def run(cmd: Sequence[str], timeout: float | None = None) -> None:
    """Run a subprocess; raise on non-zero exit."""
    subprocess.run(list(cmd), check=True, timeout=timeout)


# --- example usage ---
if __name__ == "__main__":
    quota = TokenBucket(capacity=20, tokens=20, cost=5, refill=2)

    # Host compile smoke test
    retry(
        3,
        lambda: run(
            ["g++", "-O3", "-std=c++20", "-I.", "/tmp/verify_geblomi.cpp", "-o", "/tmp/verify_geblomi"],
            timeout=30,
        ),
        bucket=quota,
        label="compile",
    )
    retry(2, lambda: run(["/tmp/verify_geblomi"], timeout=10), bucket=quota, label="host-run")
    print("Host smoke test passed.")

    # Docker smoke test
    retry(
        3,
        lambda: run(
            [
                "docker", "build", "-t", "geblomisort",
                "-f", "public/geblomi-sort/Dockerfile",
                "public/geblomi-sort",
            ],
            timeout=180,
        ),
        bucket=quota,
        label="docker-build",
    )
    retry(2, lambda: run(["docker", "run", "--rm", "geblomisort"], timeout=30), bucket=quota, label="docker-run")
    print("Docker smoke test passed.")
```

**Quota behavior (same as Bash)**

| Event | Tokens |
|-------|--------|
| Start | 20 / 20 |
| Each retry | −5 |
| Success | +2 (capped at 20) |
| Tokens < 5 | circuit open — fail fast |

### 1. Host compile smoke test (Bash)

```bash
cd public/geblomi-sort || { echo "ERROR: cannot cd to public/geblomi-sort"; exit 1; }
command -v g++ >/dev/null 2>&1 || { echo "ERROR: g++ not found"; exit 1; }
command -v timeout >/dev/null 2>&1 || { echo "ERROR: timeout not found"; exit 1; }

# (define helpers from the Bash snippet above)

cat > /tmp/verify_geblomi.cpp << 'EOF'
#include "GeblomiSort.hpp"
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
int main() {
    std::vector<int> v(1000);
    std::iota(v.rbegin(), v.rend(), 0);
    geblomi::sort(v.begin(), v.end());
    if (!std::is_sorted(v.begin(), v.end())) { std::cerr << "FAIL: ascending\n"; return 1; }
    geblomi::sort(v.begin(), v.end(), std::greater<>{});
    if (!std::is_sorted(v.begin(), v.end(), std::greater<>{})) { std::cerr << "FAIL: descending\n"; return 2; }
    std::cout << "HOST_OK\n";
    return 0;
}
EOF

retry 3 timeout 30s g++ -O3 -std=c++20 -I. /tmp/verify_geblomi.cpp -o /tmp/verify_geblomi \
  || { echo "ERROR: compilation failed after retries / quota"; exit 1; }
retry 2 timeout 10s /tmp/verify_geblomi \
  || { echo "ERROR: smoke test failed after retries / quota"; exit 1; }
echo "Host smoke test passed."
```

**Expected:** `HOST_OK` then `Host smoke test passed.`

### 2. Docker image smoke test (Bash)

```bash
command -v docker >/dev/null 2>&1 || { echo "ERROR: docker not found"; exit 1; }

retry 3 timeout 180s docker build -t geblomisort -f public/geblomi-sort/Dockerfile public/geblomi-sort \
  || { echo "ERROR: docker build failed after retries / quota"; exit 1; }
retry 2 timeout 30s docker run --rm geblomisort \
  || { echo "ERROR: docker run failed after retries / quota"; exit 1; }

echo "Image size: $(docker images geblomisort --format '{{.Size}}')"
echo "Docker smoke test passed."
```

**Expected:** demo runs (exit 0); image size typically ~80–150 MB.

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
    geblomi::g_active_package = geblomi::IncentivePackage::ResourceAware;
    geblomi::sort(v.begin(), v.end());
    if (!std::is_sorted(v.begin(), v.end())) {
        std::cerr << "FAIL: ResourceAware produced unsorted output\n";
        return 1;
    }
    std::cout << "PACKAGE_SWITCH_OK\n";
    return 0;
}
```

**Expected:** `PACKAGE_SWITCH_OK` (exit 0).

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
