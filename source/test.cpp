#include "matrix.h"

#include <gtest/gtest.h>

TEST(matrix, zero_size)
{
  Matrix<int, 0> matrix;
  ASSERT_TRUE(matrix.size() == 0);
}

TEST(matrix, assign_operator)
{
  Matrix<int, 0> matrix;
  ((matrix[100][100] = 314) = 0) = 217;
  ASSERT_TRUE(matrix[100][100] == 217);
  ASSERT_TRUE(matrix.size() == 1);
}

TEST(matrix, example)
{
  Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
  ASSERT_TRUE(matrix.size() == 0); // все ячейки свободны
  auto            a = matrix[0][0];
  ASSERT_TRUE(a == -1);
  ASSERT_TRUE(matrix.size() == 0);
  matrix[100][100] = 314;
  ASSERT_TRUE(matrix[100][100] == 314);

  ASSERT_TRUE(matrix.size() == 1);

  std::ostringstream oss;
  for (auto[x, y, v]: matrix) {
    oss << x << y << v;
  }

  auto str = oss.str();
  ASSERT_TRUE(oss.str() == "100100314");
}