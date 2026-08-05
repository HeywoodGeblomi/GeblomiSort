# Residual Sublation Layer (RSL)

**Status:** Theoretical core + minimal implementation + empirical separation locked (2026-08-05).

## 1. Motivation

Adaptive residual systems (Geblomi residual_automaton, CycleGuard, Photonic residual menus, classic confidence-gated hybrids) treat residual disorder and diagnostic competence as ordinary product signals. When the disorder *detector itself* degrades in a correlated way — the senility paradox — ordinary residual + confidence + hysteresis either abort too early or ignore the degradation too long.

The Residual Sublation Layer supplies a non-reducible intermediate state that holds both “level-1 unreliable” and “level-2 still competent” for a non-trivial interval and rewrites disposition accordingly.

The construction is the engineering realisation of the level-dependent dual-actuating synthesis (*Aufhebung*) that emerged from the epistemological discussion of residual instability.

## 2. Core Object

### State
```
Σ = D × C × Λ × {0,1}
```
- `d` ∈ [0,1] — residual disorder  
- `c` ∈ [0,1] — diagnostic competence of the residual estimator  
- `λ` ∈ [0,1] — elevation tension  
- `p` ∈ {0,1} — hidden parity (invisible to any finite-window product monitor)

Observable dual signal remains the pair `(d, c)`. The full quadruple drives elevation and rewrite.

### Elevation operator

```
ρ ← d · c
r  ← corr(d,c)_window

Rig(λ, r, p) = 
    +λ(1-λ)   if p = 0 ∧ λ > ½ ∧ r > τ
    -λ(1-λ)   if p = 1 ∧ λ > ½ ∧ r > τ
    0         otherwise

λ' = λ + α ρ (1-λ) - β (1-c) λ + δ Rig(λ,r,p) + γ r λ (1-λ)

d' = d · (1-η λ') + (1-η λ') · r_fresh
c' = c · (1-η λ') + (1-η λ') · k_fresh

p  ← p ⊕ 1{λ' > ½ ∧ r > τ}
```

The hidden parity makes the polarity of the subsequent shared attenuation invisible to any residual + confidence + hysteresis construction. After an odd number of joint crossings the attenuation polarity reverses permanently until the next crossing; product-space monitors cannot recover the correct polarity sequence.

### Rewrite dynamics

When `λ > θ`:
- switch to restricted policy `π_λ` (elevated residual acceptance thresholds + increased diagnostic probing),
- apply correlation-strengthened discharge `- μ (λ-θ) (1 + κ r)`.

## 3. Guarantees (locked)

**Non-reducibility.**  
Any pure residual + confidence + hysteresis (or finite-window product) system maintains state that is a function of a finite history of the observable pair `(d, c)`. The polarity controlled by the hidden bit `p` is not such a function. Consequently the future joint trajectory of `(d', c')` diverges from every product-space simulation once the first polarity flip has occurred.

**Senility hard-case.**  
Under the explicit correlated-degradation + partial-recovery schedule the locked object maintains a strictly longer dual-active window than early-abort baselines and produces materially lower cumulative residual error against true residual than late-ignore and hysteresis-product baselines during the recovery phase.

**Residual potential (sketch).**  
```
V(σ) = d + ½ λ^{2}
```
Under the elevated regime the one-step change satisfies a decrease driven by the strengthened discharge and polarity-aware attenuation, yielding a residual bound that ordinary product monitors lack.

## 4. Implementation

Standalone header: [`include/residual_sublation.h`](../include/residual_sublation.h)  
Empirical harness: [`experiments/rsl_senility_harness.py`](../experiments/rsl_senility_harness.py)

Zero dependence on existing residual_automaton / CycleGuard code. Integration is a one-line include plus two call sites.

## 5. Lineage

McCulloch’s reciprocal question → conditions of reliable knowing under fallible components → realisation as spontaneous observation–measurement loop → level-dependent dual-actuating synthesis (*Aufhebung*) of residual instability and residual diagnostic competence → Residual Sublation Layer.

---

*Locked under the Residual Sublation constitution, 2026-08-05.*
