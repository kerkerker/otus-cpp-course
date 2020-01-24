#include <iostream>

#include <bits/stdc++.h>

// Реализовать constexpr функцию bin_id - определения ближайшей большей степени двойки
namespace {

constexpr size_t pow(int base, uint32_t n)
{
  return n == 0 ? 1 : base * pow(base, n - 1);
}

constexpr size_t bin_id(size_t x, uint32_t n = 0)
{
  return x <= pow(2, n) ? n : bin_id(x, n + 1);
}

static_assert(bin_id(0) == 0);
static_assert(bin_id(1) == 0);
static_assert(bin_id(2) == 1);
static_assert(bin_id(4) == 2);
static_assert(bin_id(7) == 3);
static_assert(bin_id(8) == 3);
static_assert(bin_id(9) == 4);
static_assert(bin_id(1023) == 10);
static_assert(bin_id(1024) == 10);
static_assert(bin_id(1025) == 11);
static_assert(bin_id(1024 * 1024 - 33) == 20);
static_assert(bin_id(1024 * 1024) == 20);
static_assert(bin_id(1024 * 1024 + 33) == 21);
static_assert(bin_id(1024 * 1024 * 1024 - 127) == 30);
static_assert(bin_id(1024 * 1024 * 1024) == 30);
static_assert(bin_id(1024 * 1024 * 1024 + 127) == 31);


static_assert(pow(2, 3) == 8);
static_assert(pow(2, 0) == 1);
}

int main()
{
  size_t n;
  while (std::cin >> n)
    std::cout << bin_id(n) << std::endl;
  return 0;
}
