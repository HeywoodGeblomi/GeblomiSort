# bench smoke (descriptive)

Same 210 trials as the kitchen tool. `geblomi_full` is the plain `std::less<>` route.
Digit reads are the radix cost. `geblomi_fallback` keeps the custom-comparator counts.
ns descriptive only (this runner, includes the bench meter). No necessity claim. No gap column.

GOLDEN.sha256 = f9a4902360a954c30a37d6e9dfd55df7811f5cb5011bec7ac8ab150957a500db

| route | mean_cmps | mean_digits | mean_ns | win_rate |
|-------|----------:|------------:|--------:|---------:|
| pdq_full | 7735.44 | 0.00 | 11823 | 0.1429 |
| std_sort_full | 12146.99 | 0.00 | 15692 | 0.0000 |
| geblomi_full | 4348.68 | 5283.83 | 19576 | 0.8571 |
| geblomi_fallback | 9258.30 | 0.00 | 12167 | n/a |

Comparisons on the radix route are not the comparison model (digit reads are the cost there, and the log2 n! bound applies only to comparison routes).

| generator | geb_route | mean_cmps | mean_digits | mean_fallback_cmps |
|-----------|-----------|----------:|------------:|-------------------:|
| random | random | 3839.23 | 6143.60 | 13765.07 |
| already-sorted | sorted | 1278.00 | 0.00 | 1278.00 |
| reverse | reverse | 1278.00 | 0.00 | 3336.00 |
| organ-pipe | patterned | 17795.00 | 0.00 | 17795.00 |
| two-value | random | 2046.00 | 9215.37 | 4996.77 |
| noisy_ramp | random | 2060.53 | 11220.13 | 12968.17 |
| biased_walk | random | 2143.97 | 10407.73 | 10669.10 |

Identity: PASS.
