#include "bulk.h"

#include <experimental/iterator>

// Command

bool Command::isEmpty() const {
  return name.empty();
}

bool Command::isOpenBrace() const {
  return name == "{";
}

bool Command::isCloseBrace() const {
  return name == "}";
}

std::istream &operator>>(std::istream &is, Command &cmd) {
  cmd.timestamp = std::time(nullptr);
  is >> cmd.name;

  return is;
}

std::ostream &operator<<(std::ostream &os, Command const &cmd) {
  os << cmd.name;
  return os;
}

// Bunch

void Bunch::Add(Command const &cmd) {
  commands_.push_back(cmd);
}

size_t Bunch::Size() const {
  return commands_.size();
}

void Bunch::Clear() {
  is_last_ = false;
  commands_.clear();
}

bool Bunch::IsEmpty() const {
  return commands_.empty();
}

bool Bunch::IsLast() const {
  return is_last_;
}

void Bunch::SetLast() {
  is_last_ = true;
}

time_t Bunch::Timestamp() const {
  return commands_.front().timestamp;
}

auto Bunch::begin() const {
  return commands_.begin();
}

auto Bunch::end() const {
  return commands_.end();
}

std::ostream &operator<<(std::ostream &os, Bunch const &bunch) {
  os << "bulk: ";
  std::copy(bunch.begin(), bunch.end(), std::experimental::make_ostream_joiner(os, ", "));
  return os;
}

std::ostream& operator<<(std::ostream& os, LogStatistics const& log_statistics) {
  if (log_statistics.strings > 0) {
    os << log_statistics.strings << " strings, ";
  }

  os << log_statistics.commands << " commands, ";
  os << log_statistics.blocks << " blocks";
}

// CommandReader

CommandReader::CommandReader(std::istream &is) : is_{is} {}

void CommandReader::Read() {
  Command cmd;
  while (is_ >> cmd) {
    Notify(cmd);
    if (cmd.isEmpty()) {
      break;
    }
  }
  Notify(Command{});
}

// CommandAggregator

CommandAggregator::CommandAggregator(size_t bunch_size, std::ostream& log_os) :
    bunch_size_{bunch_size}, log_os_{log_os} {}

void CommandAggregator::Update(Command const &cmd) {
  ++log_statistics_.strings;
  if (cmd.isOpenBrace()) {
    Flush();
    ++open_braces_;
  } else if (cmd.isCloseBrace() && open_braces_ > 0) {
    --open_braces_;
    Flush();
  } else if (cmd.isEmpty()) {
    --log_statistics_.strings;
    if (open_braces_ > 0) {
      bunch_.Clear();
    }
    bunch_.SetLast();
    Flush();
  } else {
    ++log_statistics_.commands;
    bunch_.Add(cmd);
    if (bunch_.Size() == bunch_size_) {
      Flush();
    }
  }
}

void CommandAggregator::Flush() {
  if (open_braces_ == 0 && bunch_.Size() != 0 || bunch_.IsLast()) {
    if (!bunch_.IsEmpty()) {
      ++log_statistics_.blocks;
    }
    Notify(bunch_);
    bunch_.Clear();
  }
}

CommandAggregator::~CommandAggregator() {
  log_os_ << "main thread - " << log_statistics_ << std::endl;
}

// BunchWriter

BunchWriter::BunchWriter(std::ostream& os, std::ostream& log_os) :
    os_{os}, log_os_{log_os}, thread_{&BunchWriter::Write, this} {}

void BunchWriter::Update(Bunch const &bunch) {
  queue_.Push(bunch);

  if (!bunch.IsEmpty()) {
    ++log_statistics_.blocks;
    log_statistics_.commands += bunch.Size();
  }
}

void BunchWriter::Write()
{
  while (true) {
    Bunch bunch = queue_.Pop();
    if (!bunch.IsEmpty()) {
      os_ << bunch << std::endl;
    }
    if (bunch.IsLast()) {
      break;
    }
  }
}

BunchWriter::~BunchWriter() {
  thread_.join();
  log_os_ << "log thread - " << log_statistics_ << std::endl;
}

// BunchFileWriter

BunchFileWriter::BunchFileWriter(std::ostream &log_os) :
    log_os_{log_os},
    first_thread_{&BunchFileWriter::Write, this},
    second_thread_{&BunchFileWriter::Write, this} {}

void BunchFileWriter::Update(Bunch const &bunch) {
  queue_.Push(bunch);
}

void BunchFileWriter::Write()
{
  while (true) {
    Bunch bunch = queue_.Pop();
    if (!bunch.IsEmpty()) {
      ++log_statistics_[std::this_thread::get_id()].blocks;
      log_statistics_[std::this_thread::get_id()].commands += bunch.Size();

      std::stringstream filename;
      filename << "bulk" << bunch.Timestamp() << "_" << std::this_thread::get_id() << ".log";

      std::ofstream ofs{filename.str()};
      if (ofs) {
        ofs << bunch << std::endl;
      }
    }

    if (bunch.IsLast()) {
      // Send stop-command to another thread
      Bunch empty_bunch;
      empty_bunch.SetLast();
      queue_.Push(empty_bunch);
      break;
    }
  }
}

BunchFileWriter::~BunchFileWriter() {
  auto first_thread_id = first_thread_.get_id();
  auto second_thread_id = second_thread_.get_id();

  first_thread_.join();
  second_thread_.join();

  log_os_ << "file1 thread - " << log_statistics_[first_thread_id] << std::endl;
  log_os_ << "file2 thread - " << log_statistics_[second_thread_id] << std::endl;
}

