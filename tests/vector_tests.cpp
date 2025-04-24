#include <vector/my_vector.hpp>
#include <algorithm>
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

TEST(MyVectorTest, DefaultConstructor) {
  my_vector<int> v;
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 1);
  EXPECT_TRUE(v.is_empty());
}

TEST(MyVectorTest, SizeConstructor) {
  my_vector<int> v(5);
  EXPECT_EQ(v.size(), 5);
  EXPECT_GE(v.capacity(), 5);
  for (size_t i = 0; i < 5; ++i)
    EXPECT_EQ(v[i], 0);
}

TEST(MyVectorTest, SizeValueConstructor) {
  my_vector<int> v(3, 42);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.capacity(), 3);
  for (size_t i = 0; i < 3; ++i)
    EXPECT_EQ(v[i], 42);
}

TEST(MyVectorTest, InitializerListConstructor) {
  my_vector<int> v = {1, 2, 3};
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(MyVectorTest, CopyConstructor) {
  my_vector<int> a = {5, 6, 7};
  my_vector<int> b(a);
  EXPECT_EQ(b, a);
  EXPECT_EQ(b.size(), 3);
}

TEST(MyVectorTest, MoveConstructor) {
  my_vector<int> a = {8, 9};
  my_vector<int> b(std::move(a));
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], 8);
  EXPECT_EQ(b[1], 9);
  EXPECT_TRUE(a.is_empty());
}

TEST(MyVectorTest, CopyAssignment) {
  my_vector<int> a = {1, 2};
  my_vector<int> b;
  b = a;
  EXPECT_EQ(b, a);
}

TEST(MyVectorTest, MoveAssignment) {
  my_vector<int> a = {3, 4};
  my_vector<int> b;
  b = std::move(a);
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], 3);
  EXPECT_EQ(b[1], 4);
  EXPECT_TRUE(a.is_empty());
}

TEST(MyVectorTest, ElementAccess) {
  my_vector<int> v = {10, 20, 30};
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v.at(2), 30);
  EXPECT_THROW(v.at(5), std::out_of_range);
}

TEST(MyVectorTest, FrontBack) {
  my_vector<int> v = {7, 8, 9};
  EXPECT_EQ(v.front(), 7);
  EXPECT_EQ(v.back(), 9);
}

TEST(MyVectorTest, Iterators) {
  my_vector<int> v = {1, 2, 3};
  int sum = 0;
  for (auto it = v.begin(); it != v.end(); ++it) {
    sum += *it;
  }
  EXPECT_EQ(sum, 6);
}

TEST(MyVectorTest, ReverseIterators) {
  my_vector<int> v = {1, 2, 3};
  std::vector<int> rev(v.rbegin(), v.rend());
  EXPECT_EQ(rev, std::vector<int>({3, 2, 1}));
}

TEST(MyVectorTest, Reserve) {
  my_vector<int> v(2);
  v.reserve(10);
  EXPECT_GE(v.capacity(), 10);
  EXPECT_EQ(v.size(), 2);
  v.reserve(5);
  EXPECT_GE(v.capacity(), 10);
}

TEST(MyVectorTest, ResizeGrowShrink) {
  my_vector<int> v = {1, 2, 3};
  v.resize(5);
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[3], 0);
  v.resize(2);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
}

TEST(MyVectorTest, ShrinkToFit) {
  my_vector<int> v;
  for (int i = 0; i < 8; ++i)
    v.push_back(i);
  size_t cap_before = v.capacity();
  v.resize(4);
  v.shrink_to_fit();
  EXPECT_LT(v.capacity(), cap_before);
  EXPECT_EQ(v.capacity(), v.size());
}

TEST(MyVectorTest, SwapVectors) {
  my_vector<int> a = {1, 2};
  my_vector<int> b = {3, 4, 5};
  a.swap(b);
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(a[0], 3);
  EXPECT_EQ(b[0], 1);
}

TEST(MyVectorTest, InsertSingle) {
  my_vector<int> v = {1, 3};
  auto it = v.insert(v.begin() + 1, 2);
  EXPECT_EQ(*it, 2);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v.size(), 3);
}

TEST(MyVectorTest, InsertRange) {
  my_vector<int> v = {1, 4};
  std::vector<int> insert_vals = {2, 3};
  auto it = v.insert(v.begin() + 1, insert_vals.begin(), insert_vals.end());
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(MyVectorTest, EraseSingleAndRange) {
  my_vector<int> v = {1, 2, 3, 4};
  auto it = v.erase(v.begin() + 1);
  EXPECT_EQ(*it, 3);
  EXPECT_EQ(v.size(), 3);
  it = v.erase(v.begin(), v.begin() + 2);
  EXPECT_EQ(*it, 4);
  EXPECT_EQ(v.size(), 1);
}

TEST(MyVectorTest, PopBack) {
  my_vector<int> v = {1, 2, 3};
  v.pop_back();
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.back(), 2);
}

TEST(MyVectorTest, PushAndEmplaceBack) {
  my_vector<int> v;
  v.push_back(5);
  v.push_back(6);
  EXPECT_EQ(v[1], 6);
  v.emplace_back(7);
  EXPECT_EQ(v.back(), 7);
}

TEST(MyVectorTest, Clear) {
  my_vector<int> v = {1, 2, 3};
  v.clear();
  EXPECT_EQ(v.size(), 0);
  EXPECT_TRUE(v.is_empty());
}

TEST(MyVectorTest, ComparisonOperators) {
  my_vector<int> a = {1, 2};
  my_vector<int> b = {1, 2};
  my_vector<int> c = {1, 3};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
  EXPECT_TRUE(a < c);
  EXPECT_TRUE(c > a);
  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(a >= b);
}