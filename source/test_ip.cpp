#include "lib.h"

#include <gtest/gtest.h>

TEST(basic, ip)
{
  std::stringstream sin("194.106.108.214\t0\t3\n"
                       "24.210.1.46\t1\t0\n"
                       "166.88.193.116\t9\t0\n"
                       "1.205.202.214\t0\t2\n"
                       "188.166.87.191\t0\t50\n"
                       "46.70.147.2\t1\t6\n"
                       "1.181.177.197\t1\t0\n"
                       "95.181.177.197\t1\t0\n"
                       "149.107.0.27\t3\t0\n"
                       "182.186.69.18\t96\t0\n"
                       "73.21.168.251\t2\t0");

  std::vector<IpAddress> ip_vector = read_ip_addresses(sin);
  std::stringstream sout;

  filter_one(sout, ip_vector);
  ASSERT_EQ(sout.str(), "1.205.202.214\n"
                             "1.181.177.197\n");
  sout.str("");
  sout.clear();

  filter_two(sout, ip_vector);
  ASSERT_EQ(sout.str(), "46.70.147.2\n");
  sout.str("");
  sout.clear();

  filter_any(sout, ip_vector);
  ASSERT_EQ(sout.str(), "24.210.1.46\n"
                             "46.70.147.2\n");
}

TEST(equal_ip, ip)
{
  std::stringstream ss("68.202.216.62\t1\t0\n"
                       "68.202.216.62\t1\t0\n"
                       "68.202.216.62\t1\t0");

  std::vector<IpAddress> ip_vector = read_ip_addresses(ss);
  std::stringstream sout;

  filter_one(sout, ip_vector);

  ASSERT_TRUE(sout.str().empty());
}

TEST(incorrect_input, ip) {
  std::stringstream ss("68.202.216.62\t1\t0\n"
                       "1.216.62\t1\t0\n"
                       "68.202.216.62\t1\t0");

  ASSERT_THROW(read_ip_addresses(ss), std::invalid_argument);
}