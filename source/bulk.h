#pragma once

#include "ConcurrentQueue.h"

#include <memory>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <thread>

template<typename T>
class Observer {
public:
  virtual void Update(T const &source) = 0;
  virtual ~Observer() = default;
};

template<typename T>
using ObserverWeakPtr = std::weak_ptr<Observer<T>>;

template<typename T>
class Observable {
public:
  virtual void Subscribe(ObserverWeakPtr<T> observer) = 0;
  virtual void Notify(T const &source) = 0;
  virtual ~Observable() = default;
};

template<typename T>
class BasicObservable : public Observable<T> {
public:
  void Subscribe(ObserverWeakPtr<T> observer) override {
    subscribers_.push_back(observer);
  }

  void Notify(T const &source) override {
    for (auto &subscriber: subscribers_) {
      if (auto ptr = subscriber.lock(); ptr) {
        ptr->Update(source);
      }
    }
  }

private:
  std::vector<ObserverWeakPtr<T>> subscribers_;
};

struct Command {
  std::string name;
  std::time_t timestamp;

  [[nodiscard]] bool isEmpty() const;

  [[nodiscard]] bool isOpenBrace() const;

  [[nodiscard]] bool isCloseBrace() const;
};

std::istream &operator>>(std::istream &is, Command &cmd);
std::ostream &operator<<(std::ostream &os, Command const &cmd);

struct Bunch {
public:
  void Add(Command const &cmd);

  [[nodiscard]] size_t Size() const;

  void Clear();

  bool IsEmpty() const;

  bool IsLast() const;

  void SetLast();

  [[nodiscard]] time_t Timestamp() const;

  [[nodiscard]] auto begin() const;

  [[nodiscard]] auto end() const;

private:
  bool is_last_ = false;
  std::vector<Command> commands_;
};

std::ostream &operator<<(std::ostream &os, Bunch const &bunch);

class CommandReader : public BasicObservable<Command> {
public:
  explicit CommandReader(std::istream &is);
  void Read();

private:
  std::istream &is_;
};

struct LogStatistics {
  size_t strings = 0;
  size_t commands = 0;
  size_t blocks = 0;
};

std::ostream& operator<<(std::ostream& os, LogStatistics const& log_statistics);

class CommandAggregator : public BasicObservable<Bunch>, public Observer<Command> {
public:
  explicit CommandAggregator(size_t bunch_size, std::ostream& log_os = std::cout);
  void Update(Command const &cmd) override;
  ~CommandAggregator();

private:
  void Flush();

  Bunch bunch_;
  size_t bunch_size_;
  size_t open_braces_ = 0;
  LogStatistics log_statistics_;
  std::ostream& log_os_;
};

class BunchWriter : public Observer<Bunch> {
public:
  explicit BunchWriter(std::ostream &os, std::ostream &log_os = std::cout);
  void Update(Bunch const &bunch) override;
  ~BunchWriter();

private:
  void Write();

  std::ostream &os_;
  ConcurrentQueue<Bunch> queue_;
  mutable LogStatistics log_statistics_;
  std::ostream &log_os_;

  std::thread thread_;
};

class BunchFileWriter : public Observer<Bunch> {
 public:
  explicit BunchFileWriter(std::ostream &log_os = std::cout);
  void Update(Bunch const &bunch) override;
  ~BunchFileWriter();

 private:
  void Write();

  ConcurrentQueue<Bunch> queue_;
  std::unordered_map<std::thread::id, LogStatistics> log_statistics_;
  std::ostream &log_os_;

  std::thread first_thread_;
  std::thread second_thread_;
};


