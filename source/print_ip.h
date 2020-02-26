#include <tuple>
#include <type_traits>
#include <string>
#include <iostream>
#include <string_view>

template<typename, typename = std::void_t<>>
struct is_iterable : std::false_type {
};

template<typename T>
struct is_iterable<T,
                   std::void_t<decltype(++std::begin(std::declval<T>()) != std::end(std::declval<T>())),
                               typename std::enable_if_t<std::is_integral_v<
                                   typename std::remove_reference_t<decltype(*std::declval<decltype(std::begin(
                                       std::declval<T>()))>())>>  >>>
    : std::true_type {
};

template<class T>
constexpr bool is_iterable_v = is_iterable<T>::value;

template<class T>
struct remove_cvref {
  typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;

template<typename, typename=std::void_t<>>
struct is_string_like : std::false_type {
};

template<typename T>
struct is_string_like<T,
                      std::void_t<
                          typename std::enable_if_t<std::is_same_v<remove_cvref_t<T>, std::string> ||
                              std::is_same_v<remove_cvref_t<T>, char*> ||
                              std::is_same_v<remove_cvref<T>, std::string_view >> >>
    : std::true_type {
};

template<class T>
constexpr bool is_string_like_v = is_string_like<T>::value;

template<typename T, typename = typename std::enable_if_t<is_iterable_v<T>>,
    typename = typename std::enable_if_t<!is_string_like_v<T>>>
void print_ip(const T& container, std::ostream& out = std::cout)
{
  for (auto it = std::begin(container); it != std::end(container); ++it) {
    if (it != std::begin(container)) {
      out << '.';
    }
    out << static_cast<uint32_t>(*it);
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