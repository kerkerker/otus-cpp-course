#include "print_ip.h"

#include <vector>
#include <string>
#include <list>

int main()
{
  std::tuple ip_addresses =
      {
          char(-1),
          short(0),
          int(2130706433),
          long(8875824491850138409),
          std::string("trololo"),
          std::vector{0, 1, 2, 3},
          std::list{4, 5, 6, 7, 8},
          std::tuple{9, 10, 11}
      };

  std::apply([&ip_addresses](const auto& ... ip) { ((print_ip(ip), std::cout << std::endl), ...); }, ip_addresses);

  return 0;
}