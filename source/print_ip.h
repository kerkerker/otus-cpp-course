#include <tuple>
#include <type_traits>
#include <string>
#include <iostream>
#include <string_view>

template<typename T,
    typename Iter = decltype(std::begin(std::declval<T>())),
    typename = decltype(std::begin(std::declval<T>()) != std::end(std::declval<T>())),
    typename = typename std::enable_if_t<std::is_integral_v<
        typename std::remove_reference_t<decltype(*std::declval<Iter>())>>>>
struct is_iterable
{
  static constexpr bool value = true;
};

// TODO:
template<typename T,
    typename = typename std::enable_if_t<std::is_same_v<typename std::remove_reference<T>, std::string>>,
    typename = typename std::enable_if_t<std::is_same_v<typename std::remove_reference<T>, char*>>>
struct is_string_like
{
  static constexpr bool value = true;
};

template<typename T, typename = typename std::enable_if<is_iterable<T>::value>,
    typename = typename std::enable_if_t<!std::is_same_v<T, std::string>>>
void print_ip(const T& container, std::ostream& out = std::cout)
{
  for (auto it = std::begin(container); it != std::end(container); ++it) {
    if (it != std::begin(container)) {
      out << '.';
    }
    out << *it;
  }
}

template<typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
void print_ip(T ip, std::ostream& out = std::cout)
{
  const size_t bytes_count = sizeof(T);
  const auto& byte_array = reinterpret_cast<std::array<uint8_t, bytes_count>&>(ip);
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
  for (auto it = std::begin(byte_array); it != std::end(byte_array); ++it) {
        if (it != std::begin(byte_array)) {
            out << '.';
        }
        out << static_cast<uint32_t>(*it);
    }
#else
  for (auto it = std::rbegin(byte_array); it != std::rend(byte_array); ++it) {
    if (it != std::rbegin(byte_array)) {
      out << '.';
    }
    out << static_cast<uint32_t>(*it);
  }
#endif
}

void print_ip(std::string_view ip, std::ostream& out = std::cout)
{
  out << ip;
}

template<typename T, typename... Types>
void print_ip(const std::tuple<T, Types...>& ip, std::ostream& os = std::cout)
{
  static_assert(std::is_integral_v<T>);
  static_assert((std::is_same_v<T, Types> && ...));

  std::apply([&os](const auto& ... args) {
    size_t n{0};
    constexpr size_t tuple_size = sizeof...(Types);
    ((os << args << (n++ == tuple_size ? "" : ".")), ...);
  }, ip);
}