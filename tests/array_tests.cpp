#include <array/my_array.hpp>
#include <complex>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(MyArrayIntTest, FillConstructorAndAccess) {
  my_array<int, 5> arr(7);
  EXPECT_EQ(arr.size(), 5u);
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], 7);
    EXPECT_EQ(arr.at(i), 7);
  }
}

TEST(MyArrayIntTest, InitializerConstructor) {
  my_array<int, 3> arr(1, 2, 3);
  EXPECT_EQ(arr.size(), 3u);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

TEST(MyArrayIntTest, AtOutOfRangeThrows) {
  my_array<int, 3> arr(0);
  EXPECT_THROW(arr.at(3), std::out_of_range);
  EXPECT_THROW(arr.at(100), std::out_of_range);
}

TEST(MyArrayIntTest, FillMethod) {
  my_array<int, 4> arr;
  arr.fill(42);
  for (auto x : arr) {
    EXPECT_EQ(x, 42);
  }
}

TEST(MyArrayIntTest, SwapAndComparison) {
  my_array<int, 3> a(1, 2, 3);
  my_array<int, 3> b(4, 5, 6);
  a.swap(b);
  EXPECT_EQ(a[0], 4);
  EXPECT_EQ(b[2], 3);
  EXPECT_TRUE(a > b);
  EXPECT_FALSE(a < b);
  EXPECT_TRUE(a >= b);
  EXPECT_TRUE(b < a);
  EXPECT_TRUE(b <= a);
  EXPECT_NE(a, b);
}

TEST(MyArrayComplexTest, FillConstructor) {
  using cd = std::complex<double>;
  cd val(1.5, -2.5);
  my_array<cd, 4> arr(val);
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], val);
  }
}

TEST(MyArrayStringTest, DefaultAndFill) {
  my_array<std::string, 3> arr;
  EXPECT_EQ(arr.size(), 3u);
  for (auto &s : arr) {
    EXPECT_EQ(s, "");
  }
  arr.fill("hello");
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], "hello");
  }
}

TEST(MyArrayVectorTest, FillConstructorAndSwap) {
  my_array<std::vector<int>, 2> arr(std::vector<int>{1, 2, 3});
  EXPECT_EQ(arr.size(), 2u);
  for (auto &v : arr) {
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
  }
  my_array<std::vector<int>, 2> other(std::vector<int>{});
  arr.swap(other);
  EXPECT_TRUE(other[0].size() == 3);
  EXPECT_TRUE(arr[0].empty());
}

TEST(MyArrayNestedTest, NestedFillConstructor) {
  my_array<int, 5> inner(8);
  my_array<my_array<int, 5>, 5> arr(inner);
  for (size_t i = 0; i < arr.size(); ++i) {
    for (size_t j = 0; j < arr[i].size(); ++j) {
      EXPECT_EQ(arr[i][j], 8);
    }
  }
}

TEST(MyArrayIteratorTest, BeginEndFrontBackData) {
  my_array<int, 3> arr(5, 6, 7);
  EXPECT_EQ(*arr.begin(), 5);
  EXPECT_EQ(*(arr.end() - 1), 7);
  EXPECT_EQ(arr.front(), 5);
  EXPECT_EQ(arr.back(), 7);
  int *p = arr.data();
  EXPECT_EQ(p[1], 6);
}

TEST(MyArrayConstexprTest, Constexpr) {
  constexpr my_array<int, 3> arr(1, 2, 3);
  static_assert(arr[0] == 1, "constexpr operator[] broken");
  static_assert(arr[1] == 2, "constexpr operator[] broken");
  static_assert(arr[2] == 3, "constexpr operator[] broken");
}
