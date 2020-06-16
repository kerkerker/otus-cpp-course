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
  commands_.clear();
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

// CommandReader

void CommandReader::Read(std::istream &is) {
  Command cmd;
  while (is >> cmd) {
    Notify(cmd);
    if (cmd.isEmpty()) {
      break;
    }
  }
}

void CommandReader::Stop() {
  Notify(Command{});
}

// CommandAggregator

CommandAggregator::CommandAggregator(size_t bunch_size) : bunch_size_{bunch_size} {}

void CommandAggregator::Update(Command const &cmd) {
  if (cmd.isOpenBrace()) {
    Flush();
    ++open_braces_;
  } else if (cmd.isCloseBrace() && open_braces_ > 0) {
    --open_braces_;
    Flush();
  } else if (cmd.isEmpty()) {
    Flush();
  } else {
    bunch_.Add(cmd);
    if (bunch_.Size() == bunch_size_) {
      Flush();
    }
  }
}

void CommandAggregator::Flush() {
  if (open_braces_ == 0 && bunch_.Size() != 0) {
    Notify(bunch_);
    bunch_.Clear();
  }
}

// BunchWriter

BunchWriter::BunchWriter(std::ostream &os) : os_{os} {}

void BunchWriter::Update(Bunch const &bunch) {
  os_ << bunch << std::endl;
}

// BunchFileWriter

void BunchFileWriter::Update(Bunch const &bunch) {
  std::stringstream filename;
  filename << "bulk" << bunch.Timestamp() << ".log";

  std::ofstream ofs{filename.str()};
  if (ofs) {
    ofs << bunch << std::endl;
  }
}
