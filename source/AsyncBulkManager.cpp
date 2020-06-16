#include "AsyncBulkManager.h"

// Handler

async::Handler::Handler(size_t bulk) :
    cmd_reader_{new CommandReader},
    cmd_aggregator_{new CommandAggregator{bulk}},
    bunch_writer_{new BunchWriter{std::cout}},
    bunch_file_writer_{new BunchFileWriter},
    thread_{&Handler::Run, this} {
  cmd_reader_->Subscribe(cmd_aggregator_);
  cmd_aggregator_->Subscribe(bunch_writer_);
  cmd_aggregator_->Subscribe(bunch_file_writer_);
}

void async::Handler::Receive(std::string sv) {
  queue_.Push(std::move(std::stringstream {sv}));
}

async::Handler::~Handler() {
  Stop();
  thread_.join();
}

void async::Handler::Stop() {
  queue_.Push(std::stringstream{});
}

void async::Handler::Run() {
  while (true) {
    auto ss = queue_.Pop();
    if (ss.rdbuf()->in_avail() != 0) {
      cmd_reader_->Read(ss);
    } else {
      cmd_reader_->Stop();
      break;
    }
  }
}

// AsyncBulkManager

async::AsyncBulkManager &async::AsyncBulkManager::GetInstance() {
  static AsyncBulkManager instance;
  return instance;
}

async::handle_t async::AsyncBulkManager::Connect(std::size_t bulk) {
  handlers_.emplace(next_id_, std::make_unique<Handler>(bulk));
  return next_id_++;
}

void async::AsyncBulkManager::Receive(async::handle_t handle, const char *data, std::size_t size) {
  handlers_[handle]->Receive({data, size});
}

void async::AsyncBulkManager::Disconnect(async::handle_t handle) {
  handlers_.erase(handle);
}
