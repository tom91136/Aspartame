#include "aspartame/all.hpp"

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

#include <algorithm>
#include <numeric>
#include <random>
#include <ranges>
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
auto positive = [](int x) { return x > 0; };
auto doubled = [](int x) { return x * 2; };
} // namespace

long long eager_pipeline(const std::vector<int> &data) {
  using namespace aspartame;
  return data                  //
         ^ filter(positive)    //
         ^ map(doubled)        //
         ^ fold_left(0LL, std::plus<>());
}
long long view_pipeline(const std::vector<int> &data) {
  using namespace aspartame;
  return data                  //
         | filter(positive)    //
         | map(doubled)        //
         | fold_left(0LL, std::plus<>());
}
long long ranges_pipeline(const std::vector<int> &data) {
  auto r = data                                 //
           | std::views::filter(positive)       //
           | std::views::transform(doubled)     //
           | std::views::common;
  return std::accumulate(r.begin(), r.end(), 0LL, std::plus<>());
}
long long loop_pipeline(const std::vector<int> &data) {
  long long s = 0;
  for (auto x : data)
    if (positive(x)) s += static_cast<long long>(doubled(x));
  return s;
}

TEST_CASE("filter | map | fold_left 1M") {
  auto data = make_data(1'000'000);
  auto expected = loop_pipeline(data);
  CHECK(eager_pipeline(data) == expected);
  CHECK(view_pipeline(data) == expected);
  CHECK(ranges_pipeline(data) == expected);
  BENCHMARK("aspartame ^ (eager, materialises each step)") { return eager_pipeline(data); };
  BENCHMARK("aspartame | (lazy view, single pass)") { return view_pipeline(data); };
  BENCHMARK("std::ranges (lazy)") { return ranges_pipeline(data); };
  BENCHMARK("raw loop") { return loop_pipeline(data); };
}

size_t eager_filter_count(const std::vector<int> &data) {
  using namespace aspartame;
  return (data ^ filter(positive)) ^ count([](int) { return true; });
}
size_t view_filter_count(const std::vector<int> &data) {
  using namespace aspartame;
  return data | filter(positive) | count([](int) { return true; });
}
size_t ranges_filter_count(const std::vector<int> &data) {
  auto r = data | std::views::filter(positive);
  return std::distance(r.begin(), r.end());
}
size_t loop_filter_count(const std::vector<int> &data) {
  size_t c = 0;
  for (auto x : data)
    if (positive(x)) ++c;
  return c;
}

TEST_CASE("filter | count 1M") {
  auto data = make_data(1'000'000);
  auto expected = loop_filter_count(data);
  CHECK(eager_filter_count(data) == expected);
  CHECK(view_filter_count(data) == expected);
  CHECK(ranges_filter_count(data) == expected);
  BENCHMARK("aspartame ^ (materialises the filtered vector)") { return eager_filter_count(data); };
  BENCHMARK("aspartame | (lazy)") { return view_filter_count(data); };
  BENCHMARK("std::ranges (lazy)") { return ranges_filter_count(data); };
  BENCHMARK("raw loop") { return loop_filter_count(data); };
}

std::vector<int> eager_top_k(const std::vector<int> &data, size_t k) {
  using namespace aspartame;
  return data ^ sort() ^ take(k);
}
std::vector<int> partial_sort_top_k(const std::vector<int> &data, size_t k) {
  std::vector<int> out(k);
  std::partial_sort_copy(data.begin(), data.end(), out.begin(), out.end());
  return out;
}
std::vector<int> nth_element_top_k(const std::vector<int> &data, size_t k) {
  std::vector<int> copy = data;
  std::nth_element(copy.begin(), copy.begin() + k, copy.end());
  std::sort(copy.begin(), copy.begin() + k);
  copy.resize(k);
  return copy;
}

TEST_CASE("top-k=10 from 100K") {
  auto data = make_data(100'000);
  constexpr size_t k = 10;
  auto expected = partial_sort_top_k(data, k);
  CHECK(eager_top_k(data, k) == expected);
  CHECK(nth_element_top_k(data, k) == expected);
  BENCHMARK("aspartame ^ sort ^ take (full O(n log n) sort)") { return eager_top_k(data, k); };
  BENCHMARK("std::partial_sort_copy (O(n log k))") { return partial_sort_top_k(data, k); };
  BENCHMARK("std::nth_element + sort (O(n + k log k))") { return nth_element_top_k(data, k); };
}
