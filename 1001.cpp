#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <iomanip>

int main()
{
  std::vector<uint64_t> numbers(
      (std::istream_iterator<uint64_t>(std::cin)),
      (std::istream_iterator<uint64_t>()));

  std::cout << std::fixed;
  std::cout << std::setprecision(4);
  for (auto it = numbers.rbegin(); it != numbers.rend(); ++it) {
    std::cout << std::sqrt(*it) << std::endl;
  }

  return 0;
}
