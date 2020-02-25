#include "lib.h"

#include <iostream>
#include <algorithm>

int main(int argc, char const *argv[]) {
  try {
    const std::vector<IpAddress> ip_pool = []() {
      auto ip_pool = read_ip_addresses(std::cin);
      std::sort(ip_pool.begin(), ip_pool.end(), std::greater<IpAddress>());
      return ip_pool;
    }();

    filter(std::cout, ip_pool);
    filter_one(std::cout, ip_pool);
    filter_two(std::cout, ip_pool);
    filter_any(std::cout, ip_pool);
  }
  catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
