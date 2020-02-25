#include <vector>
#include <string>
#include <array>
#include <functional>

std::vector<std::string> split(const std::string &str, char d);

class IpAddress {
public:
  explicit IpAddress(std::string const &ip, char delimiter = '.');
  bool operator>(const IpAddress &rhs) const;
  uint8_t &operator[](size_t idx);
  const uint8_t &operator[](size_t idx) const;
  const auto begin() const;
  const auto end() const;

  friend std::ostream &operator<<(std::ostream &os, const IpAddress &ip_address);

private:
  std::array<uint8_t, 4> ip_address_;
};

std::ostream &operator<<(std::ostream &os, const IpAddress &ip);

std::vector<IpAddress> read_ip_addresses(std::istream &is);

std::ostream &filter(
    std::ostream &os, std::vector<IpAddress> const &ip_pool,
    std::function<bool(const IpAddress &)> predicate = [](const IpAddress &) { return true; });

std::ostream &filter_one(std::ostream &os, std::vector<IpAddress> const &ip_pool);

std::ostream &filter_two(std::ostream &os, std::vector<IpAddress> const &ip_pool);

std::ostream &filter_any(std::ostream &os, std::vector<IpAddress> const &ip_pool);