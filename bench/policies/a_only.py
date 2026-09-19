"""A-only policies. Input is S[:32]. No suffix. No realized costs."""

PDQ = "pdq_full"
STD = "std_sort_full"
GEB = "geblomi_full"
T = 8


def inv_latch(a):
    latch = sum(1 for i in range(len(a) - 1) if a[i] > a[i + 1])
    return STD if latch >= T else PDQ


def always_pdq(_a):
    return PDQ


def always_std(_a):
    return STD


def always_geb(_a):
    return GEB


def majority_descents(a):
    latch = sum(1 for i in range(len(a) - 1) if a[i] > a[i + 1])
    return STD if latch > (len(a) - 1) / 2 else PDQ


POLICIES = {
    "inv_latch": inv_latch,
    "always_pdq": always_pdq,
    "always_std": always_std,
    "always_geb": always_geb,
    "majority_descents": majority_descents,
}
