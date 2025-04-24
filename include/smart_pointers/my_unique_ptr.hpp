#ifndef MY_UNIQUE_PTR_HPP
#define MY_UNIQUE_PTR_HPP
#include <memory>
#include <utility>

template <typename T, typename Dp = std::default_delete<T>> class my_unique_ptr {
public:
  my_unique_ptr() noexcept : ptr_(nullptr) {}

  explicit my_unique_ptr(T *ptr) noexcept : ptr_(ptr) {}

  explicit my_unique_ptr(T *ptr, Dp deleter) noexcept : ptr_(ptr), deleter_(std::move(deleter)) {}

  my_unique_ptr(my_unique_ptr &&other) noexcept
      : ptr_(std::exchange(other.ptr_, nullptr)), deleter_(std::move(other.deleter_)) {}

  my_unique_ptr &operator=(my_unique_ptr &&other) noexcept {
    if (this != &other) {
      reset();
      ptr_ = std::exchange(other.ptr_, nullptr);
      deleter_ = std::move(other.deleter_);
    }
    return *this;
  }

  my_unique_ptr(const my_unique_ptr &) = delete;
  my_unique_ptr &operator=(const my_unique_ptr &) = delete;

  T *get() const noexcept { return ptr_; }

  T &operator*() const noexcept { return *ptr_; }

  T *operator->() const noexcept { return ptr_; }

  explicit operator bool() const noexcept { return ptr_ != nullptr; }

  T* release() noexcept {
    return std::exchange(ptr_, nullptr);
  }

  void reset(T *ptr = nullptr) noexcept {
    if (ptr_ != ptr) {
      if (ptr_)
        deleter_(ptr_);
      ptr_ = ptr;
    }
  }

  ~my_unique_ptr() noexcept {
    if (ptr_)
      deleter_(ptr_);
  }

private:
  T *ptr_;
  Dp deleter_;
};

#endif // MY_UNIQUE_PTR_HPP
