# Non-claims (GEB-ACE-001)

GeblomiSort is an adaptive hybrid 1-D sorter. The following are **not** claimed:

1. **Not a replacement for pdqsort / ska_sort on all inputs.**
   The charged bench shows wins, ties, and at least one loss (patterned vs ska on the authoring host). Soft losses vs pdq are reported, not hidden.

2. **O(1) extra memory** refers to the hybrid path as implemented (bounded run tracking, no full auxiliary buffer for the general residual). It is not a formal complexity theorem for every possible comparator specialization.

3. **Not photonic hardware.** The name family and any PhotonicSort companion are software branding / related research. This header is classical C++20.

4. **Not multi-dimensional.** 2-D through 11-D work is out of scope for this package.

5. **Not commercial-grade as a warranty.** That phrase is marketing tone; the locked evidence is Phase A correctness CI + Phase B charged table. Prefer measured on CI over slogan language.

6. **promote_ready = false** for any sentence of the form "beats pdq on every distribution."

Lying about a cell (claiming win where the harness shows loss or no measurement) is an automatic failure of honesty.
