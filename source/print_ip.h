#pragma once

#include <tuple>
#include <type_traits>
#include <string>
#include <iostream>
#include <string_view>

/**
 * Checks if a type is iterable and consists of integral values
 */
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

/** Removes const or/and volatile specifiers from the given type
 */
template<class T>
struct remove_cvref {
  typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;

/**
 * Checks if a type is std::string_view or std::string or char*
 */
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

template<typename InputIt>
void print_container(InputIt first, InputIt last, std::ostream& out = std::cout) {
  for (auto it = first; it != last; ++it) {
    if (it != first) {
      out << '.';
    }
    out << static_cast<uint32_t>(*it);
  }
}

/** Outputs container to the out in the format of decimal numbers each separated by a single dot
 *
 * @tparam T - container type
 * @param container - IP address
 * @param out - output stream
 * @return
 */
template<typename T>
std::enable_if_t<is_iterable_v<T> && !is_string_like_v<T>> print_ip(const T& container, std::ostream& out = std::cout)
{
  print_container(std::begin(container), std::end(container), out);
}

/** Outputs integral value ip to the out in the format of decimal numbers (one byte in size) each separated
 * by a single dot
 *
 * @tparam T - ip type
 * @param ip - IP address
 * @param out - output stream
 * @return
 */
template<typename T>
std::enable_if_t<std::is_integral_v<T>> print_ip(T ip, std::ostream& out = std::cout)
{
  const size_t bytes_count = sizeof(T);
  const auto& byte_array = reinterpret_cast<std::array<uint8_t, bytes_count>&>(ip);
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
  print_container(std::begin(container), std::end(container), out);
#else
  print_container(std::rbegin(byte_array), std::rend(byte_array), out);
#endif
}

/** Outputs ip to the out in the string format
 *
 * @param ip - IP address
 * @param out - output stream
 */
void print_ip(std::string_view ip, std::ostream& out = std::cout)
{
  out << ip;
}

/** Outputs tuple ip to the out in the format of decimal numbers each separated by a single dot
 *
 * @tparam T - type of the first element in the tuple ip
 * @tparam Types - types of the other elements in the tuple ip
 * @param ip - IP address
 * @param os - output stream
 * @return
 */
template<typename T, typename... Types>
std::enable_if_t<std::is_integral_v<T> && (std::is_same_v<T, Types> && ...)>
print_ip(const std::tuple<T, Types...>& ip, std::ostream& os = std::cout)
{
  std::apply([&os](const auto& ... args) {
    size_t n{0};
    constexpr size_t tuple_size = sizeof...(Types);
    ((os << args << (n++ == tuple_size ? "" : ".")), ...);
  }, ip);
}