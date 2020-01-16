#include "lib.h"

#include <gtest/gtest.h>

TEST(test_major_version, version)
{
  ASSERT_TRUE(major_version() == 0);
}