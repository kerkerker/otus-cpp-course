#include "bulk.h"

#include <gtest/gtest.h>

TEST(bulk, basic) {
  std::stringstream ss;
  ss << "cmd1\n"
        "cmd2\n"
        "cmd3\n"
        "cmd4\n"
        "cmd5\n";

  std::stringstream sout;

  auto cmd_reader = std::make_shared<CommandReader>();
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3);
  auto bunch_writer = std::make_shared<BunchWriter>(sout);
  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_reader->Read(ss);
  cmd_reader->Stop();

  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5\n");
}

TEST(bulk, braces) {
  std::stringstream ss;
  ss << "cmd1\n"
        "cmd2\n"
        "cmd3\n"
        "{\n"
        "cmd4\n"
        "cmd5\n"
        "cmd6\n"
        "cmd7\n"
        "}\n";

  std::stringstream sout;

  auto cmd_reader = std::make_shared<CommandReader>();
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3);
  auto bunch_writer = std::make_shared<BunchWriter>(sout);
  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_reader->Read(ss);
  cmd_reader->Stop();

  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5, cmd6, cmd7\n");
}

TEST(bulk, nested_braces) {
  std::stringstream ss;
  ss << "{\n"
        "cmd1\n"
        "cmd2\n"
        "{\n"
        "cmd3\n"
        "cmd4\n"
        "}\n"
        "cmd5\n"
        "cmd6\n"
        "}\n";

  std::stringstream sout;

  auto cmd_reader = std::make_shared<CommandReader>();
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3);
  auto bunch_writer = std::make_shared<BunchWriter>(sout);
  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_reader->Read(ss);
  cmd_reader->Stop();

  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n");
}

TEST(bulk, open_brace) {
  std::stringstream ss;
  ss << "cmd1\n"
        "cmd2\n"
        "cmd3\n"
        "{\n"
        "cmd4\n"
        "cmd5\n"
        "cmd6\n"
        "cmd7";

  std::stringstream sout;

  auto cmd_reader = std::make_shared<CommandReader>();
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3);
  auto bunch_writer = std::make_shared<BunchWriter>(sout);
  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_reader->Read(ss);
  cmd_reader->Stop();

  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3\n");
}