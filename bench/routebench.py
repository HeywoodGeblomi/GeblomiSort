#!/usr/bin/env python3
"""GeblomiSort/bench: descriptive table for three frozen routes."""

from __future__ import annotations

import argparse
import csv
import hashlib
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from policies.a_only import POLICIES
from tripwire.identity import check as identity_check

STABLE = [
    "generator",
    "seed",
    *[f"a{i}" for i in range(32)],
    "c_pdq",
    "c_std",
    "c_geb",
    "r_star",
]


def load_oracle(path):
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


def stable_hash(rows):
    h = hashlib.sha256()
    for r in rows:
        h.update((",".join(r[k] for k in STABLE) + "\n").encode())
    return h.hexdigest()


def mean(xs):
    return sum(xs) / len(xs) if xs else float("nan")


def describe(rows):
    n = len(rows)
    print("route,mean_cmps,mean_ns,win_rate")
    for key, ccol, ncol in (
        ("pdq_full", "c_pdq", "ns_pdq"),
        ("std_sort_full", "c_std", "ns_std"),
        ("geblomi_full", "c_geb", "ns_geb"),
    ):
        cmps = [int(r[ccol]) for r in rows]
        ns = [int(r[ncol]) for r in rows if ncol in r]
        wins = sum(1 for r in rows if r["r_star"] == key)
        print(f"{key},{mean(cmps):.2f},{mean(ns):.0f},{wins / n:.4f}")


def policy_losses(rows):
    losses = {}
    for name, fn in POLICIES.items():
        miss = 0
        for r in rows:
            a = [int(r[f"a{i}"]) for i in range(32)]
            if fn(a) != r["r_star"]:
                miss += 1
        losses[name] = miss / len(rows)
    return losses


def ensure_oracle(path: Path) -> None:
    if path.exists():
        return
    binary = Path("bench/count_routes")
    if not binary.exists():
        print("REFUSE: missing oracle and bench/count_routes; compile from repo root")
        raise SystemExit(2)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w") as f:
        subprocess.check_call([str(binary)], stdout=f)


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--oracle", default="bench/oracle.csv")
    p.add_argument("--expect-hash", default="")
    args = p.parse_args()
    oracle = Path(args.oracle)
    ensure_oracle(oracle)
    rows = load_oracle(oracle)
    if len(rows) != 210:
        print(f"REFUSE: expected 210 scored trials, got {len(rows)}")
        return 2
    digest = stable_hash(rows)
    print(f"stable_sha256,{digest}")
    if args.expect_hash and digest != args.expect_hash:
        print("REFUSE: stable hash mismatch")
        return 3
    describe(rows)
    tw = identity_check(policy_losses(rows))
    print("identity_tripwire," + ("PASS" if tw["pass"] else "FAIL"))
    if tw["identity_hits"]:
        print("identity_hits," + " ".join(tw["identity_hits"]))
        print("identity loss==0 fails; no necessity claim")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
