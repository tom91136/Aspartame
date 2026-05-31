// Eager-side single-pass reductions over 1M ints: aspartame ^, the std
// equivalent, and a hand loop. Each TEST_CASE asserts agreement before
// timing, in the same style as numeric.cpp.

#include "aspartame/all.hpp"

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

namespace {
std::vector<int> make_data(size_t n, unsigned seed = 42) {
  std::mt19937 g(seed);
  std::uniform_int_distribution<int> d(-1000, 1000);
  std::vector<int> v(n);
  for (auto &x : v)
    x = d(g);
  return v;
}
} // namespace

int eager_sum(const std::vector<int> &data) {
  using namespace aspartame;
  return data ^ sum();
}
int std_sum(const std::vector<int> &data) { return std::accumulate(data.begin(), data.end(), 0); }
int loop_sum(const std::vector<int> &data) {
  int s = 0;
  for (auto x : data)
    s += x;
  return s;
}

TEST_CASE("sum 1M") {
  auto data = make_data(1'000'000);
  auto expected = std_sum(data);
  CHECK(eager_sum(data) == expected);
  CHECK(loop_sum(data) == expected);
  BENCHMARK("aspartame ^") { return eager_sum(data); };
  BENCHMARK("std::accumulate") { return std_sum(data); };
  BENCHMARK("raw loop") { return loop_sum(data); };
}

int eager_min(const std::vector<int> &data) {
  using namespace aspartame;
  return (data ^ min()).value();
}
int std_min(const std::vector<int> &data) { return *std::min_element(data.begin(), data.end()); }
int loop_min(const std::vector<int> &data) {
  int m = data[0];
  for (auto x : data)
    if (x < m) m = x;
  return m;
}

TEST_CASE("min 1M") {
  auto data = make_data(1'000'000);
  auto expected = std_min(data);
  CHECK(eager_min(data) == expected);
  CHECK(loop_min(data) == expected);
  BENCHMARK("aspartame ^") { return eager_min(data); };
  BENCHMARK("std::min_element") { return std_min(data); };
  BENCHMARK("raw loop") { return loop_min(data); };
}

size_t eager_count_pos(const std::vector<int> &data) {
  using namespace aspartame;
  return data ^ count([](int x) { return x > 0; });
}
size_t std_count_pos(const std::vector<int> &data) {
  return std::count_if(data.begin(), data.end(), [](int x) { return x > 0; });
}
size_t loop_count_pos(const std::vector<int> &data) {
  size_t c = 0;
  for (auto x : data)
    if (x > 0) ++c;
  return c;
}

TEST_CASE("count(x > 0) 1M") {
  auto data = make_data(1'000'000);
  auto expected = std_count_pos(data);
  CHECK(eager_count_pos(data) == expected);
  CHECK(loop_count_pos(data) == expected);
  BENCHMARK("aspartame ^") { return eager_count_pos(data); };
  BENCHMARK("std::count_if") { return std_count_pos(data); };
  BENCHMARK("raw loop") { return loop_count_pos(data); };
}
