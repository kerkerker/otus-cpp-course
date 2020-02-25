#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "allocator.h"

constexpr size_t factorial(int n) {
    return n == 0 ? 1 : (n * factorial(n - 1));
}

int main(int, char *[]) {
    std::map<int, int> map_with_default_alloc;
    for (int i = 0; i < 10; ++i) {
        map_with_default_alloc[i] = i;
    }

    std::map<int, int, std::less<>, CustomAllocator<std::pair<int, int>, 10>> map_with_new_alloc{};
    for (int i = 0; i < 10; ++i) {
        map_with_new_alloc[i] = factorial(i);
    }

    for (const auto&[key, val]: map_with_new_alloc) {
        std::cout << key << ' ' << val << std::endl;
    }

    return 0;
}
