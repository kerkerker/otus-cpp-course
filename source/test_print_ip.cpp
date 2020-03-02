#include "print_ip.h"

#include <gtest/gtest.h>
#include <sstream>

TEST(basic, test_print_ip)
{
  std::stringstream out;
  print_ip(int{1234}, out);

  if constexpr (sizeof(int) == 4) {
    ASSERT_EQ(out.str(), "0.0.4.210");
  } else if (sizeof(int) == 2) {
    ASSERT_EQ(out.str(), "4.210");
  }
}