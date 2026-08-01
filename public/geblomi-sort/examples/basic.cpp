#include "GeblomiSort.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    geblomi::sort(v.begin(), v.end());
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
