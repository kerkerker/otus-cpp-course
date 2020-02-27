#include <gtest/gtest.h>
#include "custom_allocator.h"

#include <vector>

TEST(basic, allocator)
{
  const size_t vec_size = 100;

  std::vector<int>  expected_vec;
  std::vector<int, CustomAllocator<int, 10>> vec;

  expected_vec.reserve(vec_size);
  vec.reserve(vec_size);

  for (size_t i = 0; i < vec.capacity(); ++i) {
    vec[i] = i;
    expected_vec[i] = i;
  }

  ASSERT_TRUE(std::equal(expected_vec.begin(), expected_vec.end(), vec.begin()));
}