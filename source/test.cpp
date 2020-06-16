#include "bulk.h"

#include <gtest/gtest.h>

TEST(bulkmt, basic) {
  std::stringstream ss;
  ss << "cmd1\n"
        "cmd2\n"
        "cmd3\n"
        "cmd4\n"
        "cmd5\n";

  std::stringstream sout;
  std::ostringstream log_ss;
  std::ostringstream log_ss_log;

  auto cmd_reader = std::make_shared<CommandReader>(ss);
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3, log_ss);
  auto bunch_writer = std::make_shared<BunchWriter>(sout, log_ss_log);
  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_reader->Read();

  cmd_aggregator.reset();
  bunch_writer.reset();
  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5\n");
  ASSERT_EQ(log_ss.str(), "main thread - 5 strings, 5 commands, 2 blocks\n");
  ASSERT_EQ(log_ss_log.str(), "log thread - 5 commands, 2 blocks\n");
}

TEST(bulkmt, braces) {
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
  std::ostringstream log_ss;
  std::ostringstream log_ss_log;

  auto cmd_reader = std::make_shared<CommandReader>(ss);
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3, log_ss);
  auto bunch_writer = std::make_shared<BunchWriter>(sout, log_ss_log);
  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_reader->Read();

  cmd_aggregator.reset();
  bunch_writer.reset();
  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5, cmd6, cmd7\n");
  ASSERT_EQ(log_ss.str(), "main thread - 9 strings, 7 commands, 2 blocks\n");
  ASSERT_EQ(log_ss_log.str(), "log thread - 7 commands, 2 blocks\n");
}

TEST(bulkmt, nested_braces) {
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
  std::ostringstream log_ss;
  std::ostringstream log_ss_log;
  std::ostringstream log_ss_file;

  auto cmd_reader = std::make_shared<CommandReader>(ss);
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3, log_ss);
  auto bunch_writer = std::make_shared<BunchWriter>(sout, log_ss_log);
  auto bunch_file_writer = std::make_shared<BunchFileWriter>(log_ss_file);

  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_aggregator->Subscribe(bunch_file_writer);
  cmd_reader->Read();

  cmd_aggregator.reset();
  bunch_writer.reset();
  bunch_file_writer.reset();

  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n");
  ASSERT_EQ(log_ss.str(), "main thread - 10 strings, 6 commands, 1 blocks\n");
  ASSERT_EQ(log_ss_log.str(), "log thread - 6 commands, 1 blocks\n");
  ASSERT_TRUE(log_ss_file.str() == "file1 thread - 0 commands, 0 blocks\nfile2 thread - 6 commands, 1 blocks\n" ||
      log_ss_file.str() == "file1 thread - 6 commands, 1 blocks\nfile2 thread - 0 commands, 0 blocks\n");
}

TEST(bulkmt, open_brace) {
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
  std::ostringstream log_ss;
  std::ostringstream log_ss_log;

  auto cmd_reader = std::make_shared<CommandReader>(ss);
  auto cmd_aggregator = std::make_shared<CommandAggregator>(3, log_ss);
  auto bunch_writer = std::make_shared<BunchWriter>(sout, log_ss_log);
  cmd_reader->Subscribe(cmd_aggregator);
  cmd_aggregator->Subscribe(bunch_writer);
  cmd_reader->Read();

  cmd_aggregator.reset();
  bunch_writer.reset();
  ASSERT_EQ(sout.str(), "bulk: cmd1, cmd2, cmd3\n");
  ASSERT_EQ(log_ss.str(), "main thread - 8 strings, 7 commands, 1 blocks\n");
  ASSERT_EQ(log_ss_log.str(), "log thread - 3 commands, 1 blocks\n");
}


