#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "Allocator.h"
#include "CustomList.h"

constexpr size_t factorial(int n)
{
  return n == 0 ? 1 : (n * factorial(n - 1));
}

int main(int, char* [])
{
  std::cout << "map with default allocator" << std::endl;
  std::map<int, int> map_with_default_alloc;
  for (int i = 0; i < 10; ++i) {
    map_with_default_alloc[i] = i;
  }

  std::cout << "map with custom allocator" << std::endl;
  std::map<int, int, std::less<>, CustomAllocator<std::pair<int, int>, 10>> map_with_new_alloc{};
  for (int i = 0; i < 10; ++i) {
    map_with_new_alloc[i] = factorial(i);
  }

  for (const auto&[key, val]: map_with_new_alloc) {
    std::cout << key << ' ' << val << std::endl;
  }

  std::cout << "custom list with custom allocator" << std::endl;
  CustomList<int, CustomAllocator<int>> custom_list;
  for (int i = 0; i < 10; ++i) {
    custom_list.push_front(factorial(i));
  }

  for (auto elem: custom_list) {
    std::cout << elem << std::endl;
  }

  return 0;
}
