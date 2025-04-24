// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>

template <typename T> class my_vector {
public:
  my_vector() {
    _start = static_cast<T *>(::operator new(sizeof(T)));
    _finish = _start;
    _end_of_storage = _start + 1;
  }

  explicit my_vector(const size_t n) {
    allocate_and_default_construct(n);
  }

  explicit my_vector(const size_t n, const T &value) { allocate_and_fill(n, value); }

  template <std::forward_iterator II>
    requires std::constructible_from<T, std::iter_reference_t<II>>
  my_vector(II first, II last) {
    size_t n = std::distance(first, last);
    allocate_storage(n);
    std::uninitialized_copy(first, last, _start);
    _finish = _start + n;
  }

  my_vector(std::initializer_list<T> init) {
    size_t n = init.size();
    allocate_storage(n);
    std::uninitialized_copy(init.begin(), init.end(), _start);
    _finish = _start + n;
  }

  my_vector(const my_vector &other) {
    size_t n = other.capacity();
    T *raw = static_cast<T *>(::operator new(n * sizeof(T)));
    std::unique_ptr<T, void (*)(void *)> guard(raw, ::operator delete);
    T *m = std::uninitialized_copy(other._start, other._finish, raw);
    guard.release();
    _start = raw;
    _finish = m;
    _end_of_storage = _start + n;
  }

  my_vector(my_vector &&other) noexcept
      : _start(other._start), _finish(other._finish), _end_of_storage(other._end_of_storage) {
    other._start = nullptr;
    other._finish = nullptr;
    other._end_of_storage = nullptr;
  }

  my_vector &operator=(const my_vector &other) {
    if (this != &other) {
      my_vector tmp(other);
      swap(tmp);
    }
    return *this;
  }

  my_vector &operator=(my_vector &&other) noexcept {
    if (this != &other) {
      clean_up();
      _start = other._start;
      _finish = other._finish;
      _end_of_storage = other._end_of_storage;
      other._start = nullptr;
      other._finish = nullptr;
      other._end_of_storage = nullptr;
    }
    return *this;
  }

  ~my_vector() noexcept { clean_up(); }

  T &operator[](size_t index) noexcept { return _start[index]; }

  const T &operator[](size_t index) const noexcept { return _start[index]; }

  T &at(size_t index) {
    if (index >= size())
      throw std::out_of_range("Index out of range");
    return _start[index];
  }

  const T &at(size_t index) const {
    if (index >= size())
      throw std::out_of_range("Index out of range");
    return _start[index];
  }

  T &front() { return *_start; }

  const T &front() const { return *_start; }

  T &back() { return *(_finish - 1); }

  const T &back() const { return *(_finish - 1); }

  T *begin() noexcept { return _start; }

  const T *begin() const noexcept { return _start; }

  T *end() noexcept { return _finish; }

  const T *end() const noexcept { return _finish; }

  const T *cbegin() const noexcept { return _start; }

  const T *cend() const noexcept { return _finish; }

  std::reverse_iterator<T *> rbegin() noexcept { return std::reverse_iterator<T *>(end()); }

  std::reverse_iterator<const T *> rbegin() const noexcept { return std::reverse_iterator<const T *>(end()); }

  std::reverse_iterator<T *> rend() noexcept { return std::reverse_iterator<T *>(begin()); }

  std::reverse_iterator<const T *> rend() const noexcept { return std::reverse_iterator<const T *>(begin()); }

  std::reverse_iterator<const T *> crbegin() const noexcept { return std::reverse_iterator<const T *>(end()); }

  std::reverse_iterator<const T *> crend() const noexcept { return std::reverse_iterator<const T *>(begin()); }

  size_t capacity() const { return _end_of_storage - _start; }

  size_t size() const { return _finish - _start; }

  bool is_empty() const noexcept { return _start == _finish; }

  void reserve(size_t n) {
    if (n <= capacity())
      return;
    T *raw = static_cast<T *>(::operator new(n * sizeof(T)));
    std::unique_ptr<T, void (*)(void *)> guard(raw, ::operator delete);
    T *mid = std::uninitialized_move(_start, _finish, raw);
    guard.release();
    std::destroy(_start, _finish);
    ::operator delete(_start);
    _start = raw;
    _finish = mid;
    _end_of_storage = _start + n;
  }

  void resize(size_t n) {
    if (n > capacity()) {
      reserve(n);
    }
    if (n > size()) {
      std::uninitialized_value_construct_n(_finish, n - size());
      _finish += n - size();
    } else {
      std::destroy(_start + n, _finish);
      _finish = _start + n;
    }
  }

  void shrink_to_fit() {
    if (_finish < _end_of_storage) {
      size_t new_capacity = size();
      T *new_start = static_cast<T *>(::operator new(new_capacity * sizeof(T)));
      T *new_finish = std::uninitialized_copy(_start, _finish, new_start);
      clean_up();
      _start = new_start;
      _finish = new_finish;
      _end_of_storage = _start + new_capacity;
    }
  }

  void swap(my_vector &other) noexcept {
    std::swap(_start, other._start);
    std::swap(_finish, other._finish);
    std::swap(_end_of_storage, other._end_of_storage);
  }

  T *insert(const T *pos, const T &value) {
    const size_t idx = pos - _start;
    if (_start == _finish) {
      std::construct_at(_start, value);
      _finish = _start + 1;
      return _start;
    }
    if (_finish < _end_of_storage) {
      std::uninitialized_move(_finish - 1, _finish, _finish);
      std::move_backward(_start + idx, _finish - 1, _finish);
      std::construct_at(_start + idx, value);
      ++_finish;
      return _start + idx;
    }
    const size_t old_size = size();
    const size_t new_cap = old_size ? old_size * 2 : 1;
    T *new_start = static_cast<T *>(::operator new(new_cap * sizeof(T)));
    T *new_finish = new_start;
    new_finish = std::uninitialized_move(_start, _start + idx, new_start);
    std::construct_at(new_finish, value);
    ++new_finish;
    new_finish = std::uninitialized_move(_start + idx, _finish, new_finish);
    std::destroy(_start, _finish);
    ::operator delete(_start);
    _start = new_start;
    _finish = new_finish;
    _end_of_storage = new_start + new_cap;

    return _start + idx;
  }

  template <typename II,
            typename = std::enable_if_t<std::is_constructible_v<T, typename std::iterator_traits<II>::reference>>>
  T *insert(const T *pos, II first, II last) {
    size_t idx = pos - _start;
    size_t count = std::distance(first, last);
    if (count == 0)
      return _start + idx;
    if (size() + count <= capacity()) {
      T *old_end = _finish;
      std::uninitialized_move(_finish - count, _finish, _finish);
      std::move_backward(_start + idx, old_end - count, old_end);
      std::uninitialized_copy(first, last, _start + idx);
      _finish += count;
      return _start + idx;
    }
    size_t new_cap = std::max(capacity() * 2, size() + count);
    T *new_start = static_cast<T *>(::operator new(new_cap * sizeof(T)));
    T *new_finish = new_start;
    new_finish = std::uninitialized_copy(_start, _start + idx, new_start);
    new_finish = std::uninitialized_copy(first, last, new_finish);
    new_finish = std::uninitialized_copy(_start + idx, _finish, new_finish);
    clean_up();
    _start = new_start;
    _finish = new_finish;
    _end_of_storage = _start + new_cap;
    return _start + idx;
  }

  T *erase(const T *pos) {
    T *erase_pos = const_cast<T *>(pos);
    if (erase_pos + 1 != _finish) {
      std::move(erase_pos + 1, _finish, erase_pos);
    }
    --_finish;
    std::destroy_at(_finish);
    return erase_pos;
  }

  T *erase(const T *first, const T *last) {
    T *erase_first = const_cast<T *>(first);
    T *erase_last = const_cast<T *>(last);
    if (erase_last != _finish) {
      std::move(erase_last, _finish, erase_first);
    }
    size_t count = erase_last - erase_first;
    _finish -= count;
    std::destroy(erase_first, _finish);
    return erase_first;
  }

  void pop_back() {
    if (_finish != _start) {
      --_finish;
      std::destroy_at(_finish);
    }
  }

  void push_back(const T &value) {
    if (_finish != _end_of_storage) {
      std::construct_at(_finish, value);
      ++_finish;
      return;
    }
    const size_t old_cap = capacity();
    const size_t new_cap = old_cap ? old_cap * 2 : 1;
    T *new_start = static_cast<T *>(::operator new(new_cap * sizeof(T)));
    std::unique_ptr<T, void (*)(void *)> guard(new_start, ::operator delete);
    T *dest = std::uninitialized_move(_start, _finish, new_start);
    std::construct_at(dest, value);
    ++dest;
    guard.release();
    std::destroy(_start, _finish);
    ::operator delete(_start);
    _start = new_start;
    _finish = dest;
    _end_of_storage = new_start + new_cap;
  }

  void push_back(T &&value) {
    if (_finish == _end_of_storage) {
      reallocate(capacity() ? capacity() * 2 : 1);
    }
    std::construct_at(_finish, std::move(value));
    ++_finish;
  }

  template <typename... Args> void emplace_back(Args &&...args) {
    if (_finish == _end_of_storage) {
      reallocate(capacity() ? capacity() * 2 : 1);
    }
    std::construct_at(_finish, std::forward<Args>(args)...);
    ++_finish;
  }

  void clear() noexcept {
    std::destroy(_start, _finish);
    _finish = _start;
  }

  bool operator==(const my_vector &other) const {
    if (size() != other.size()) {
      return false;
    }
    return std::equal(_start, _finish, other._start);
  }

  bool operator!=(const my_vector &other) const { return !(*this == other); }

  bool operator<(const my_vector &other) const {
    return std::lexicographical_compare(_start, _finish, other._start, other._finish);
  }

  bool operator>(const my_vector &other) const { return other < *this; }

  bool operator<=(const my_vector &other) const { return !(other < *this); }

  bool operator>=(const my_vector &other) const { return !(*this < other); }

private:
  void allocate_storage(const size_t n) {
    _start = static_cast<T *>(::operator new(n * sizeof(T)));
    _end_of_storage = _start + n;
    _finish = _start;
  }

  void allocate_and_default_construct(const size_t n) {
    allocate_storage(n);
    std::uninitialized_value_construct_n(_start, n);
    _finish = _end_of_storage;
  }

  void allocate_and_fill(const size_t n, const T &value) {
    allocate_storage(n);
    std::uninitialized_fill_n(_start, n, value);
    _finish = _end_of_storage;
  }

  void reallocate(size_t new_cap) {
    T *new_start = static_cast<T *>(::operator new(new_cap * sizeof(T)));
    T *new_finish = std::uninitialized_move(_start, _finish, new_start);
    clean_up();
    _start = new_start;
    _finish = new_finish;
    _end_of_storage = _start + new_cap;
  }

  void clean_up() {
    if (_start) {
      std::destroy(_start, _finish);
      ::operator delete(_start);
    }
  }

  T *_start;
  T *_finish;
  T *_end_of_storage;
};

#endif // MY_VECTOR_HPP
