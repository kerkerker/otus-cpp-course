#pragma once

#include "async.h"
#include "bulk.h"
#include "ConcurrentQueue.h"

#include <memory>
#include <sstream>
#include <unordered_map>
#include <string_view>
#include <string>
#include <thread>

namespace async {

class Handler {
public:
  Handler(size_t bulk);

  void Receive(std::string sv);

  ~Handler();

private:
  void Stop();

  void Run();

  ConcurrentQueue<std::stringstream> queue_;
  std::shared_ptr<CommandReader> cmd_reader_;
  std::shared_ptr<CommandAggregator> cmd_aggregator_;
  std::shared_ptr<BunchWriter> bunch_writer_;
  std::shared_ptr<BunchFileWriter> bunch_file_writer_;

  std::thread thread_;
};

using HandlerPtr = std::unique_ptr<Handler>;

class AsyncBulkManager {
public:
  static AsyncBulkManager &GetInstance();

  handle_t Connect(std::size_t bulk);

  void Receive(handle_t handle, const char *data, std::size_t size);

  void Disconnect(handle_t handle);

private:
  AsyncBulkManager() = default;
  ~AsyncBulkManager() = default;
  AsyncBulkManager(const AsyncBulkManager &) = delete;
  AsyncBulkManager &operator=(const AsyncBulkManager &) = delete;

  size_t next_id_ = 0;
  std::unordered_map<handle_t, HandlerPtr> handlers_;
};
}