# bench

Package copy of the routebench tool. Third frozen route: `GeblomiSort.hpp`.
Not a filed title. `promote_ready=false`. No necessity claim.
Identity: `loss == 0` fails.

## compile

```
python3 bench/meter_header.py
g++ -O2 -std=c++20 -o bench/count_routes bench/count_routes.cpp
./bench/count_routes > bench/oracle.csv
python3 bench/routebench.py --oracle bench/oracle.csv
```

Run from repo root. Output stays under `bench/`.
`meter_header.py` writes `bench/metered/GeblomiSort.hpp` (gitignored). That copy
counts comparisons and digit reads, and routes `geb_meter::CountLess` like
`std::less<>`. The product header is not modified.

## oracle.csv

`generator,seed,a0..a31,c_pdq,c_std,c_geb,d_geb,c_geb_fb,ns_pdq,ns_std,ns_geb,ns_geb_fb,geb_route,r_star`

210 rows. Seeds 1..30. Same gens as the kitchen tool. n=1024. A=S[:32].

- `c_geb` — comparator calls on the plain `std::less<>` route (probe, comparison engines, and ska_sort's `std::sort` fallback)
- `d_geb` — radix digit reads (`current_byte`) on that route. 0 unless `geb_route` is `random`
- `c_geb_fb` — comparator calls with a custom comparator (the previous `c_geb`)
- ns descriptive only, and includes the bench meter

Every sort is checked against `std::sort`. A random probe route with zero digit reads fails the run. A non-random route with any digit read fails the run. The custom-comparator pass must leave the radix meter at zero.

## routes

- `pdq_full` — Orson Peters pdqsort (amalgamated), counted with CountComp
- `std_sort_full` — libstdc++ `std::sort`, counted with CountComp
- `geblomi_full` — `geblomi::sort` with CountLess, routed as plain `std::less<>`
- `geblomi_fallback` — `geblomi::sort` with CountComp (not in `r_star`)

`r_star` is the strictly fewest comparator calls among pdq_full, std_sort_full, and geblomi_full. Ties prefer pdq, then std, then geblomi. Digit reads are not ranked.

Comparisons on the radix route are not the comparison model (digit reads are the cost there, and the log2 n! bound applies only to comparison routes).
