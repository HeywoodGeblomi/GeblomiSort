#!/usr/bin/env python3
"""
Residual Sublation Layer (RSL) – Senility Stress-Test Harness
Self-contained Python implementation of the locked object + baselines.
"""

import numpy as np
from dataclasses import dataclass, field
from typing import List, Tuple

# -------------------------------------------------------------
# Locked RSL parameters (defaults from residual_sublation.h)
# -------------------------------------------------------------
@dataclass
class RSLParams:
    alpha: float = 0.15
    beta: float = 0.10
    gamma: float = 0.20
    delta: float = 0.25
    eta: float = 0.08
    mu: float = 0.12
    kappa: float = 1.5
    tau: float = 0.30
    theta: float = 0.45

@dataclass
class RSLState:
    d: float = 0.0
    c: float = 1.0
    lambda_: float = 0.0
    p: int = 0

    def step(self, par: RSLParams, r_fresh: float, k_fresh: float, r_corr: float):
        rho = self.d * self.c
        rig = 0.0
        if self.lambda_ > 0.5 and r_corr > par.tau:
            rig = (1.0 if self.p == 0 else -1.0) * self.lambda_ * (1.0 - self.lambda_)

        lambda_new = (
            self.lambda_
            + par.alpha * rho * (1.0 - self.lambda_)
            - par.beta * (1.0 - self.c) * self.lambda_
            + par.delta * rig
            + par.gamma * r_corr * self.lambda_ * (1.0 - self.lambda_)
        )
        lambda_new = max(0.0, min(1.0, lambda_new))

        att = 1.0 - par.eta * lambda_new
        self.d = self.d * att + (1.0 - att) * r_fresh
        self.c = self.c * att + (1.0 - att) * k_fresh

        if lambda_new > 0.5 and r_corr > par.tau:
            self.p ^= 1

        self.lambda_ = lambda_new

    def rewrite(self, par: RSLParams, r_corr: float) -> bool:
        if self.lambda_ <= par.theta:
            return False
        discharge = par.mu * (self.lambda_ - par.theta) * (1.0 + par.kappa * r_corr)
        self.lambda_ = max(0.0, self.lambda_ - discharge)
        return True

# -------------------------------------------------------------
# Ordinary baselines (residual + confidence + hysteresis)
# -------------------------------------------------------------
@dataclass
class BaselineState:
    d: float = 0.0
    c: float = 1.0
    mode: str = "ordinary"  # ordinary | restricted | aborted
    hist_d: List[float] = field(default_factory=list)
    hist_c: List[float] = field(default_factory=list)

class EarlyAbortBaseline:
    """High confidence threshold → aborts early when c drops."""
    def __init__(self, conf_thresh: float = 0.60):
        self.conf_thresh = conf_thresh
        self.st = BaselineState()

    def step(self, r_fresh: float, k_fresh: float, r_corr: float):
        self.st.d = 0.7 * self.st.d + 0.3 * r_fresh
        self.st.c = 0.7 * self.st.c + 0.3 * k_fresh
        self.st.hist_d.append(self.st.d)
        self.st.hist_c.append(self.st.c)
        if self.st.c < self.conf_thresh:
            self.st.mode = "aborted"

class LateIgnoreBaseline:
    """Pure residual gating – ignores competence until residual saturates."""
    def __init__(self, res_thresh: float = 0.75):
        self.res_thresh = res_thresh
        self.st = BaselineState()

    def step(self, r_fresh: float, k_fresh: float, r_corr: float):
        self.st.d = 0.7 * self.st.d + 0.3 * r_fresh
        self.st.c = 0.7 * self.st.c + 0.3 * k_fresh
        self.st.hist_d.append(self.st.d)
        self.st.hist_c.append(self.st.c)
        if self.st.d > self.res_thresh:
            self.st.mode = "restricted"  # too late

class HysteresisProductBaseline:
    """Filtered product d*c with simple hysteresis."""
    def __init__(self, high: float = 0.35, low: float = 0.15):
        self.high = high
        self.low = low
        self.st = BaselineState()
        self.elevated = False

    def step(self, r_fresh: float, k_fresh: float, r_corr: float):
        self.st.d = 0.7 * self.st.d + 0.3 * r_fresh
        self.st.c = 0.7 * self.st.c + 0.3 * k_fresh
        prod = self.st.d * self.st.c
        if not self.elevated and prod > self.high:
            self.elevated = True
            self.st.mode = "restricted"
        elif self.elevated and prod < self.low:
            self.elevated = False
            self.st.mode = "ordinary"
        self.st.hist_d.append(self.st.d)
        self.st.hist_c.append(self.st.c)

# -------------------------------------------------------------
# Senility noise schedule (exact)
# -------------------------------------------------------------
def senility_schedule(t: int) -> Tuple[float, float, float]:
    """Returns (d_true, c_true, r_corr) at step t."""
    if t < 20:
        d = np.random.uniform(0.05, 0.15)
        c = 0.95
        r = 0.0
    elif t <= 40:
        d = 0.15 + 0.02 * (t - 20)
        c = 0.95 - 0.025 * (t - 20)
        r = min(0.7, 0.035 * (t - 20))
    else:
        d = 0.9
        c = 0.2
        r = 0.75
    return d, c, r

# -------------------------------------------------------------
# Harness
# -------------------------------------------------------------
def run_harness(T: int = 80, seed: int = 42):
    np.random.seed(seed)
    par = RSLParams()
    rsl = RSLState()
    early = EarlyAbortBaseline()
    late = LateIgnoreBaseline()
    hyst = HysteresisProductBaseline()

    rsl_d_hist = []
    dual_active = []
    elevated_flags = []

    for t in range(T):
        d_true, c_true, r_corr = senility_schedule(t)

        # RSL step + rewrite
        rsl.step(par, d_true, c_true, r_corr)
        elevated = rsl.rewrite(par, r_corr)
        elevated_flags.append(elevated)
        rsl_d_hist.append(rsl.d)

        # dual-active: level-1 unreliable (d high) AND level-2 still competent (c moderate) AND elevated
        dual = (rsl.d > 0.4) and (rsl.c > 0.3) and elevated
        dual_active.append(dual)

        # baselines
        early.step(d_true, c_true, r_corr)
        late.step(d_true, c_true, r_corr)
        hyst.step(d_true, c_true, r_corr)

    # Metrics
    dual_window = sum(dual_active)
    residual_after_40 = rsl_d_hist[40] if len(rsl_d_hist) > 40 else rsl_d_hist[-1]
    residual_final = rsl_d_hist[-1]
    bounded = residual_final < 0.85  # heuristic under the schedule

    print("=== Residual Sublation Layer – Senility Stress Test ===")
    print(f"Total steps               : {T}")
    print(f"Dual-active window length : {dual_window} steps")
    print(f"Residual at t=40          : {residual_after_40:.4f}")
    print(f"Residual at end           : {residual_final:.4f}")
    print(f"Residual bounded (<0.85)  : {bounded}")
    print()
    print("Baseline final modes:")
    print(f"  Early-abort  : {early.st.mode}  (final d={early.st.d:.3f}, c={early.st.c:.3f})")
    print(f"  Late-ignore  : {late.st.mode}  (final d={late.st.d:.3f}, c={late.st.c:.3f})")
    print(f"  Hysteresis   : {hyst.st.mode}  (final d={hyst.st.d:.3f}, c={hyst.st.c:.3f})")
    print()
    print("RSL final state:")
    print(f"  d={rsl.d:.4f}  c={rsl.c:.4f}  λ={rsl.lambda_:.4f}  p={rsl.p}")

    return {
        "dual_window": dual_window,
        "residual_t40": residual_after_40,
        "residual_final": residual_final,
        "bounded": bounded,
        "rsl_hist": rsl_d_hist,
        "elevated_flags": elevated_flags,
    }

if __name__ == "__main__":
    run_harness()
