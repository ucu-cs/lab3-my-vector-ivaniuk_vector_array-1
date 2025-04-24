// // This is a personal academic project. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <vector/my_vector.hpp>
#include <array/my_array.hpp>
#include <cassert>
#include <iostream>
#include <vector>
#include <array>
#include <chrono>

my_vector<int> my_vector_benchmark_push(int n) {
  my_vector<int> v;
  v.reserve(n);
  for (int i = 0; i < n; ++i) {
    v.push_back(i);
  }
  return v;
}

std::vector<int> vector_benchmark_push(int n) {
  std::vector<int> v;
  v.reserve(n);
  for (int i = 0; i < n; ++i) {
    v.push_back(i);
  }
  return v;
}

int main() {
  const int N = 1'000'000;


  auto start = std::chrono::high_resolution_clock::now();
  auto my_v = my_vector_benchmark_push(N);
  auto end = std::chrono::high_resolution_clock::now();
  auto my_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();


  start = std::chrono::high_resolution_clock::now();
  auto std_v = vector_benchmark_push(N);
  end = std::chrono::high_resolution_clock::now();
  auto std_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

  int my_sum = 0;
  for (const auto &i : my_v) {
    my_sum += i;
  }
  int std_sum = 0;
  for (const auto &i : std_v) {
    std_sum += i;
  }
  assert(my_sum == std_sum);

  std::cout
      << "my_vector push took " << my_ms  << " ns (sum=" << my_sum << ")\n"
      << "std::vector push took " << std_ms  << " ns (sum=" << std_sum << ")\n";
  std::unique_ptr<int> p = std::make_unique<int>(42);
  std::shared_ptr<int> sp = std::make_shared<int>(42);

  return 0;
}