#ifndef MY_ARRAY_HPP
#define MY_ARRAY_HPP
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T, std::size_t N> class my_array {
public:
  my_array() = default;

  explicit constexpr my_array(const T &value) {
    if constexpr (std::is_trivially_copyable_v<T>) {
      fill_trivial(value);
    } else {
      fill_nontrivial(value, std::make_index_sequence<N>{});
    }
  }

  template <typename... U, typename = std::enable_if_t<sizeof...(U) == N>>
  explicit constexpr my_array(U&&... elems)
    : _data{  std::forward<U>(elems)...  } {}

  constexpr T &operator[](std::size_t index) noexcept { return _data[index]; }

  constexpr const T &operator[](std::size_t i) const noexcept { return _data[i]; }

  T &at(std::size_t i) {
    if (i >= N)
      throw std::out_of_range("my_array::at");
    return _data[i];
  }

  constexpr const T &at(std::size_t i) const {
    if (i >= N)
      throw std::out_of_range("my_array::at");
    return _data[i];
  }

  T *begin() noexcept { return _data; }

  constexpr const T *begin() const noexcept { return _data; }

  T *end() noexcept { return _data + N; }

  constexpr const T *end() const noexcept { return _data + N; }

  T &front() noexcept { return _data[0]; }

  constexpr const T &front() const noexcept { return _data[0]; }

  T &back() noexcept { return _data[N - 1]; }

  constexpr const T &back() const noexcept { return _data[N - 1]; }

  T *data() noexcept { return _data; }

  constexpr const T *data() const noexcept { return _data; }

  void fill(const T &value) {
    if constexpr (std::is_trivially_copyable_v<T>) {
      fill_trivial(value);
    } else {
      for (std::size_t i = 0; i < N; ++i)
        _data[i] = value;
    }
  }

  void swap(my_array &other) noexcept(noexcept(std::swap(std::declval<T &>(), std::declval<T &>()))) {
    for (std::size_t i = 0; i < N; ++i)
      std::swap(_data[i], other._data[i]);
  }

  constexpr bool operator==(const my_array<T, N> &other) const noexcept {
    for (std::size_t i = 0; i < N; ++i) {
      if (_data[i] != other._data[i])
        return false;
    }
    return true;
  }

  constexpr bool operator!=(const my_array<T, N> &other) const noexcept {
    return !(*this == other);
  }

  constexpr bool operator<(const my_array<T, N> &other) const noexcept {
    for (std::size_t i = 0; i < N; ++i) {
      if (_data[i] < other._data[i])
        return true;
      if (_data[i] > other._data[i])
        return false;
    }
    return false;
  }

  constexpr bool operator>(const my_array<T, N> &other) const noexcept {
    return other < *this;
  }

  constexpr bool operator<=(const my_array<T, N> &other) const noexcept {
    return !(other < *this);
  }

  constexpr bool operator>=(const my_array<T, N> &other) const noexcept {
    return !(*this < other);
  }

  constexpr std::size_t size() const noexcept { return N; }

private:
  void fill_trivial(const T &value) {
    std::memcpy(&_data[0], &value, sizeof(T));
    std::size_t done = 1;
    while (done < N) {
      const std::size_t to_copy = std::min(done, N - done);
      std::memcpy(&_data[done], &_data[0], to_copy * sizeof(T));
      done += to_copy;
    }
  }

  template <std::size_t... I> void fill_nontrivial(const T &value, std::index_sequence<I...>) {
    (new (&_data[I]) T(value), ...);
  }

  T _data[N];
};

#endif // MY_ARRAY_HPP
