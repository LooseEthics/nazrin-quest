
#pragma once

#include <array>
#include <cassert>
#include <cstddef>

template<typename T, std::size_t Capacity>
class RingBuffer
{
  static_assert(Capacity > 0);

public:
  void push(const T& value)
  {
    buffer_[head_] = value;
    head_ = (head_ + 1) % Capacity;

    if (size_ < Capacity) ++size_;
    else tail_ = (tail_ + 1) % Capacity;
  }

  T& operator[](std::size_t index)
  {
    assert(index < size_);
    return buffer_[(tail_ + index) % Capacity];
  }

  const T& operator[](std::size_t index) const
  {
    assert(index < size_);
    return buffer_[(tail_ + index) % Capacity];
  }

  T& back()
  {
    assert(!empty());
    return buffer_[(head_ + Capacity - 1) % Capacity];
  }

  const T& back() const
  {
    assert(!empty());
    return buffer_[(head_ + Capacity - 1) % Capacity];
  }

  T& front()
  {
    assert(!empty());
    return buffer_[(tail_)];
  }

  const T& front() const
  {
    assert(!empty());
    return buffer_[(tail_)];
  }

  std::size_t size() const { return size_; }
  constexpr std::size_t capacity() const { return Capacity; }
  bool empty() const { return size_ == 0; }
  bool full() const { return size_ == Capacity; }
  T sum() const
  {
    T out{};
    for (std::size_t i = 0; i < size_; ++i)
      out += buffer_[(tail_ + i) % Capacity];
    return out;
  }

private:
  std::array<T, Capacity> buffer_{};
  std::size_t head_ = 0;
  std::size_t tail_ = 0;
  std::size_t size_ = 0;
};
