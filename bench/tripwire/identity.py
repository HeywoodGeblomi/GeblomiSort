"""Identity tripwire: loss == 0 fails the run. Do not quote a claim."""


def check(loss_by_policy):
    hits = [name for name, loss in loss_by_policy.items() if loss == 0.0]
    return {
        "pass": len(hits) == 0,
        "identity_hits": hits,
    }
