#include "matrix.h"

bool operator==(const Key& lhs, const Key& rhs)
{
  return std::tie(lhs.x, rhs.y) == std::tie(rhs.x, rhs.y);
}