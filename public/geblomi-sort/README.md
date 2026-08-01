# GeblomiSort v2.7.0

**Adaptive hybrid 1-D sorting for C++20.**

GeblomiSort probes the input for structure (sorted, reverse, long runs, random) and dispatches to the right engine: early exit, reverse, Verge-style run merge, Ska radix (integers), or pdqsort as residual. On patterned data it is often far faster than a single algorithm; on pure random integers it matches Ska.

**Team:** Grok, Harper, Lucas, Benjamin (Benji), Heywood

---

## Quick start

```bash
# Both headers must be on the include path
g++ -O3 -std=c++20 -I. your.cpp -o your
```

```cpp
#include "GeblomiSort.hpp"
#include <vector>

int main() {
    std::vector<int> v = {5, 1, 4, 2, 3};
    geblomi::sort(v.begin(), v.end());
    geblomi::sort(v.begin(), v.end(), std::greater<>{});
}
```

**Required files**

| File | Role |
|------|------|
| `GeblomiSort.hpp` | Amalgam: probe, routing, Verge, pdqsort, ska |
| `residual_automaton.hpp` | Capacity-guard automaton (required include) |

---

## What it is / is not

| Is | Is not |
|----|--------|
| Production-minded **1-D** adaptive hybrid | A multi-dimensional (2-D–11-D) product |
| Competitive with pdqsort & Ska on standard patterns | Guaranteed fastest on every key type / machine |
| Comparator-aware, low extra memory | A drop-in claim over IPS⁴o without your own benches |

`experimental/ImplosionSort_2D.hpp` is a scaffold only — **not** part of the supported v2.7.0 API.

---

## Routing (simplified)

| Detection | Talent |
|-----------|--------|
| Fully sorted | No-op |
| Fully reverse | Reverse (then residual if needed) |
| Few long runs, low direction-change (e.g. organpipe) | Verge / RunMerge |
| Random-like integers + default compare | Ska radix |
| Everything else | pdqsort |

**Joint Merge gate:** Verge is used only when strong runs **and** low direction-change rate agree (avoids Merge over-trigger on high-flip data).

---

## Benchmarks (reference)

Machine-dependent. Measured on the release gate host, **n = 1 000 000 `int`**, `g++ -O3 -std=c++20`, best-of-several:

| Pattern | Geblomi | pdqsort | ska (approx) |
|---------|---------|---------|--------------|
| Sorted | ~0.01 ms | ~0.7 ms | ~12 ms |
| Reverse | ~0.14 ms | ~1.4 ms | ~14 ms |
| Random | ~15 ms | ~29 ms | ~15 ms |
| Sawtooth | ~13 ms | ~25 ms | ~13 ms |
| Organpipe | ~1.3–1.5 ms | ~31 ms | ~17 ms |

Re-run on your hardware:

```bash
g++ -O3 -std=c++20 -I. benches/release_gate.cpp -o release_gate && ./release_gate
```

Expect `ALL_RELEASE_GATE_OK=1`.

---

## Limitations (honest)

1. **CAP = 2** on element-span run promotion: only very few direction changes qualify for long-run Verge promotion. **3–8 reversed/interleaved long runs** may fall through to pdqsort (~10–12 ms class at 1M). Documented residual, not a silent bug.
2. Probe is **sampling-based** — pathological adversarial layouts can mis-route (pdqsort residual still correct).
3. Ska path is aimed at **scalar integers** with default ordering; custom comparators use pdqsort.
4. **2-D / Implosion** work is experimental and unsupported in this tag.

---

## License & attribution

- **pdqsort** — Orson Peters (zlib-style notice retained in `GeblomiSort.hpp`)
- **ska sort** — Malte Skarupke lineage as vendored (notice retained)
- **Geblomi** probe, routing, Verge policy, joint Merge gate, residual automaton — original project work (Grok, Harper, Lucas, Benjamin, Heywood)

See comments at the top of `GeblomiSort.hpp` and `LICENSE`.

---

## Release

- **Tag:** v2.7.0
- **Phase 0 decisions:** `RELEASE_PHASE0_DECISIONS.md`
- **Notes:** `RELEASE_NOTES_v2.7.0.md`

Canonical repo: https://github.com/HeywoodGeblomi/GeblomiSort
