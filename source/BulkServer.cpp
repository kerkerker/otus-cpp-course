#include "BulkServer.h"

namespace async {
using ip::tcp;

Session::Session(ip::tcp::socket socket, handle_t handle)
    : socket_{std::move(socket)}, handle_{handle} {}

void Session::Read() {
  auto self(shared_from_this());

  socket_.async_read_some(ba::buffer(read_buffer_),
                          [this, self](boost::system::error_code ec, std::size_t length) {
                            if (!ec) {
                              async::receive(handle_, read_buffer_.data(), length);
                              Read();
                            }
                          });
}

void Session::Start() {
  Read();
}

BulkServer::BulkServer(ba::io_context &io_context, uint16_t port, size_t bulk_size) :
    handle_{connect(bulk_size)}, acceptor_{io_context, tcp::endpoint(tcp::v4(), port)} {

}

void BulkServer::Run() {
  Accept();
}

void BulkServer::Accept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          std::make_shared<Session>(std::move(socket), handle_)->Start();
        }

        Accept();
      });
}

BulkServer::~BulkServer() {
  async::disconnect(handle_);
}
}