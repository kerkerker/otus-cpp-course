#include <iostream>

uint32_t calc_sulfid(uint32_t N, uint32_t A, uint32_t B)
{
  return N * A * B * 2;
}

int main()
{
  uint32_t N, A, B;
  std::cin >> N >> A >> B;
  std::cout << calc_sulfid(N, A, B) << std::endl;

  return 0;
}
