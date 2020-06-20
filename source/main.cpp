#include "async.h"
#include "BulkServer.h"

#include <iostream>

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
      return 1;
    }

    int port = std::stoi(argv[1]);
    if (port < 0) {
      std::cout << "port must be 0 or greater" << std::endl;
      return 1;
    }

    int bulk_size = std::stoll(argv[2]);
    if (bulk_size < 1) {
      std::cout << "bulk_size must be 1 or greater" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;
    async::BulkServer bulk_server{io_context, static_cast<uint16_t>(port), static_cast<size_t>(bulk_size)};
    bulk_server.Run();
    io_context.run();
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << "\n";
  }

  return 0;
}