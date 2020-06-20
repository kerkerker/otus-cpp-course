#pragma once

#include "async.h"

#include <boost/asio.hpp>

#include <memory>
#include <iostream>
#include <array>

namespace async {
namespace ba = boost::asio;
namespace ip = boost::asio::ip;

class Session
    : public std::enable_shared_from_this<Session>
{
public:
  Session(ip::tcp::socket socket, handle_t handle);

  void Start();

private:
  void Read();

  static constexpr size_t MAX_LENGTH_ = 1024;
  ip::tcp::socket socket_;
  handle_t handle_;
  std::array<char, MAX_LENGTH_> read_buffer_;
};

class BulkServer {
public:
  BulkServer(ba::io_context &io_context, uint16_t port, size_t bulk_size);
  void Run();
  ~BulkServer();

private:
  void Accept();

  handle_t handle_;
  ip::tcp::acceptor acceptor_;
};
}
