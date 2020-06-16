#include "async.h"
#include "AsyncBulkManager.h"

namespace async {

handle_t connect(std::size_t bulk) {
  return AsyncBulkManager::GetInstance().Connect(bulk);
}

void receive(handle_t handle, const char *data, std::size_t size) {
  AsyncBulkManager::GetInstance().Receive(handle, data, size);
}

void disconnect(handle_t handle) {
  AsyncBulkManager::GetInstance().Disconnect(handle);
}
}
