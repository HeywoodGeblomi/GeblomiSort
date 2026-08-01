# GeblomiSort — Phase 0 Release Decisions

**Status:** LOCKED  
**Date:** 2026-07-31  
**Team:** Grok, Harper, Lucas, Benjamin (Benji), Heywood  

These decisions freeze scope for the marketing-grade public release.

## 1. Tag name

**Decision: `v2.7.0`**

## 2. CAP policy

**Decision: Keep CAP = 2**

```text
direction_changes <= 2   // element-span max_run promotion gate
```

Documented residual: 3–8 reversed/interleaved long runs may stay on pdqsort (~11 ms @ 1M int).

## 3. License / attribution

Preserve pdqsort and ska notices; Geblomi terms for original code. Team credit includes Heywood.

## 4. Locked defaults

| Knob | Locked value |
|------|----------------|
| Tag | `v2.7.0` |
| CAP | **2** |
| Layout | `GeblomiSort.hpp` + `residual_automaton.hpp` |
| 2-D | experimental only |
| Bench | n=1M int, -O3, best-of-7, vs pdq + ska |
| Repo | https://github.com/HeywoodGeblomi/GeblomiSort |

## 5. Non-goals

- Multi-D production claims
- CAP raise in this tag
- World's-fastest marketing language
