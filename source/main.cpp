#include "matrix.h"

#include <iostream>

int main()
{
  Matrix<int, 0> matrix;

  for (size_t i = 0; i < 10; ++i) {
    matrix[i][i] = i;
    matrix[i][10 - i - 1] = 10 - i - 1;
  }

  for (size_t i = 1; i < 9; ++i) {
    for (size_t j = 1; j < 9; ++j) {
      std::cout << matrix[i][j];
      if (j != 8) {
        std::cout << ' ';
      }
    }
    std::cout << std::endl;
  }

  std::cout << matrix.size() << std::endl;

  for (auto[x, y, v]: matrix) {
    std::cout << '[' << x << ", " << y << "] = " << v << std::endl;
  }

  return 0;
}