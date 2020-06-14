#include "bulk.h"

#include <iostream>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <N>" << std::endl;
    return 1;
  }

  auto cmd_reader = std::make_shared<CommandReader>(std::cin);
  auto cmd_aggregator = std::make_shared<CommandAggregator>(std::stoul(argv[1]));
  auto bunch_writer = std::make_shared<BunchWriter>(std::cout);
  auto bunch_file_writer = std::make_shared<BunchFileWriter>();

  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_aggregator->Subscribe(bunch_file_writer);

  cmd_reader->Read();

  return 0;
}