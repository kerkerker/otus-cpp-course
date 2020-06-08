#pragma once

#include <unordered_map>
#include <utility>

struct Key
{
  size_t x;
  size_t y;
};

bool operator==(const Key& lhs, const Key& rhs);

namespace std {
template<>
struct hash<Key>
{
  size_t operator()(const Key& key) const
  {
    auto x_hash = std::hash<decltype(key.x)>{}(key.x);
    auto y_hash = std::hash<decltype(key.y)>{}(key.y);
    size_t x = 37; // prime
    return x_hash + y_hash * x;
  }
};
}

template<typename T, T DefaultValue>
class Matrix
{
 public:
  using DataType = std::unordered_map<Key, T>;

  template<size_t Dimension = 2>
  class AccessMatrix
  {
   public:
    AccessMatrix(std::unordered_map<Key, T>& data, size_t x, size_t y = 0) : data_{data}, x_{x}, y_{y}
    {
    }

    auto operator[](size_t column_idx)
    {
      return AccessMatrix<Dimension - 1>(data_, x_, column_idx);
    }

    auto operator=(T value)
    {
      static_assert(Dimension == 1);

      if (value == DefaultValue) {
        data_.erase({x_, y_});
      } else {
        data_[{x_, y_}] = value;
      }

      return *this;
    }

    operator T()
    {
      static_assert(Dimension == 1);
      auto val = DefaultValue;
      try {
        val = data_.at({x_, y_});
      }
      catch (std::exception const&) {

      }
      return val;
    }

   private:
    std::unordered_map<Key, T>& data_;
    size_t x_;
    size_t y_;
  };

  class MatrixIterator
  {
   public:
    using value_type = std::tuple<decltype(std::declval<Key>().x), decltype(std::declval<Key>().y), T>;

    explicit MatrixIterator(typename DataType::iterator it) noexcept:
        cur_it_{it}
    {}

    MatrixIterator& operator++()
    {
      ++cur_it_;
      return *this;
    }

    MatrixIterator operator++(int)
    {
      auto ret_val = *this;
      ++cur_it_;
      return ret_val;
    }

    value_type* operator->()
    {
      cur_value_ = {cur_it_->first.x, cur_it_->first.y, cur_it_->second};
      return &cur_value_;
    }

    value_type& operator*()
    {
      cur_value_ = {cur_it_->first.x, cur_it_->first.y, cur_it_->second};
      return cur_value_;
    }

    bool operator!=(const MatrixIterator& other)
    {
      return cur_it_ != other.cur_it_;
    }

   private:
    typename DataType::iterator cur_it_;
    value_type                  cur_value_;
  };

  AccessMatrix<2> operator[](std::size_t raw_idx)
  {
    return AccessMatrix(data_, raw_idx);
  }

  size_t size() const
  {
    return data_.size();
  }

  auto begin() noexcept
  {
    return MatrixIterator{data_.begin()};
  }

  auto end() noexcept
  {
    return MatrixIterator{data_.end()};
  }

 private:
  std::unordered_map<Key, T> data_;
};


