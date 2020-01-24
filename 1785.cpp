#include <iostream>
#include <map>
#include <string>

std::string translate_to_anindilakva(uint16_t n)
{
  static std::map<uint16_t, std::string> size_to_word = {
      {4, "few"}, {9, "several"}, {19, "pack"},
      {49, "lots"}, {99, "horde"}, {249, "throng"},
      {499, "swarm"}, {999, "zounds"}, {2000, "legion"}
  };

  return size_to_word.lower_bound(n)->second;
}

int main()
{
  uint16_t n;
  std::cin >> n;
  std::cout << translate_to_anindilakva(n);

  return 0;
}
