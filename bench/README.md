# bench

Package copy of the routebench tool. Third frozen route: `GeblomiSort.hpp`.
Not a filed title. `promote_ready=false`. No necessity claim.
Identity: `loss == 0` fails.

## compile

```
g++ -O2 -std=c++20 -o bench/count_routes bench/count_routes.cpp
./bench/count_routes > bench/oracle.csv
python3 bench/routebench.py --oracle bench/oracle.csv
```

Run from repo root. Output stays under `bench/`.

## oracle.csv

`generator,seed,a0..a31,c_pdq,c_std,c_geb,ns_pdq,ns_std,ns_geb,r_star`

210 rows. Seeds 1..30. Same gens as the kitchen tool. n=1024. A=S[:32].
Meter: comparator count. ns descriptive only.

## routes

- `pdq_full` — Orson Peters pdqsort (amalgamated)
- `std_sort_full` — libstdc++ std::sort
- `geblomi_full` — `geblomi::sort` + same CountComp

Tie-break prefers pdq, then std, then geblomi.
