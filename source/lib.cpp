#include "lib.h"

#include <sstream>
#include <algorithm>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d) {
  std::vector<std::string> result;
  std::string token;

  std::istringstream ss(str);
  while (std::getline(ss, token, d)) {
    result.push_back(token);
  }

  return result;
}

IpAddress::IpAddress(std::string const &ip, char delimiter) {
  auto const &ip_parts = split(ip, delimiter);
  if (ip_parts.size() != ip_address_.size()) {
    throw std::invalid_argument("incorrect number of parts in ip");
  }

  std::transform(ip_parts.begin(),
                 ip_parts.end(),
                 ip_address_.begin(),
                 [](const std::string &str) { return std::stoul(str); });
}

bool IpAddress::operator>(const IpAddress &rhs) const {
  for (auto first = ip_address_.begin(), first_rhs = rhs.ip_address_.begin();
       (first != ip_address_.end()) && (first_rhs != rhs.ip_address_.end());
       ++first, ++first_rhs) {
    if (*first > *first_rhs) {
      return true;
    }
    if (*first_rhs > *first) {
      return false;
    }
  }
  return false;
}

uint8_t &IpAddress::operator[](size_t idx) {
  return ip_address_[idx];
}

const uint8_t &IpAddress::operator[](size_t idx) const {
  return ip_address_[idx];
}

const auto IpAddress::begin() const {
  return ip_address_.begin();
}

const auto IpAddress::end() const {
  return ip_address_.end();
}

std::ostream &operator<<(std::ostream &os, const IpAddress &ip) {
  const auto &first_part = *ip.begin();
  for (auto const &ip_part : ip) {
    if (&ip_part != &first_part) {
      os << ".";
    }
    os << static_cast<uint16_t>(ip_part);
  }
  return os;
}

std::ostream &filter(
    std::ostream &os, std::vector<IpAddress> const &ip_pool,
    std::function<bool(const IpAddress &)> predicate) {
  for (const auto &ip : ip_pool) {
    if (!predicate(ip)) {
      continue;
    }
    os << ip << std::endl;
  }

  return os;
}

std::ostream &filter_one(std::ostream &os, std::vector<IpAddress> const &ip_pool) {
  return filter(os, ip_pool, [](const IpAddress &ip) { return ip[0] == 1; });
}

std::ostream &filter_two(std::ostream &os, std::vector<IpAddress> const &ip_pool) {
  return filter(os,
                ip_pool,
                [](const IpAddress &ip) { return ip[0] == 46 && ip[1] == 70; });
}

std::ostream &filter_any(std::ostream &os, std::vector<IpAddress> const &ip_pool) {
  return filter(os,
                ip_pool,
                [](const IpAddress &ip) {
                  return std::any_of(
                      ip.begin(), ip.end(), [](auto part) { return part == 46; });
                });
}

std::vector<IpAddress> read_ip_addresses(std::istream &is) {
  std::vector<IpAddress> ip_pool;

  for (std::string line; std::getline(is, line);) {
    if (line.empty()) {
      continue;
    }

    auto v = split(line, '\t');
    ip_pool.emplace_back(v.at(0));
  }

  return ip_pool;
}
