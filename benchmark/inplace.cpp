// In-place ^= vs rebuild-and-assign ^. BENCHMARK_ADVANCED pre-allocates one
// copy of the input per run so the reset cost is excluded from the timing.

#include "aspartame/all.hpp"

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

#include <map>
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
std::map<int, int> make_map(size_t n) {
  std::map<int, int> m;
  for (size_t i = 0; i < n; ++i)
    m.emplace(static_cast<int>(i), static_cast<int>(i * 3));
  return m;
}
auto even = [](int x) { return x % 2 == 0; };
auto plus_one = [](int v) { return v + 1; };
} // namespace

TEST_CASE("filter even, vector<int> 1M") {
  using namespace aspartame;
  auto base = make_data(1'000'000);
  auto check = base;
  check ^= filter(even);
  CHECK(check.size() < base.size());

  BENCHMARK_ADVANCED("in-place vec ^= filter(p)")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::vector<int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] ^= filter(even);
      return copies[i].size();
    });
  };
  BENCHMARK_ADVANCED("out-of-place vec = vec ^ filter(p)")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::vector<int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] = copies[i] ^ filter(even);
      return copies[i].size();
    });
  };
}

TEST_CASE("sort vector<int> 100K") {
  using namespace aspartame;
  auto base = make_data(100'000);
  auto check = base;
  check ^= sort();
  CHECK(std::is_sorted(check.begin(), check.end()));

  BENCHMARK_ADVANCED("in-place vec ^= sort()")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::vector<int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] ^= sort();
      return copies[i].size();
    });
  };
  BENCHMARK_ADVANCED("out-of-place vec = vec ^ sort()")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::vector<int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] = copies[i] ^ sort();
      return copies[i].size();
    });
  };
}

TEST_CASE("self-append, vector<int> 100K") {
  using namespace aspartame;
  auto base = make_data(100'000);
  auto check = base;
  check ^= concat(check);
  CHECK(check.size() == 2 * base.size());

  BENCHMARK_ADVANCED("in-place xs ^= concat(xs)")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::vector<int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] ^= concat(copies[i]);
      return copies[i].size();
    });
  };
  BENCHMARK_ADVANCED("out-of-place xs = xs ^ concat(xs)")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::vector<int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] = copies[i] ^ concat(copies[i]);
      return copies[i].size();
    });
  };
}

TEST_CASE("map_values, std::map<int,int> 100K") {
  using namespace aspartame;
  auto base = make_map(100'000);
  auto check = base;
  check ^= map_values(plus_one);
  CHECK(check.size() == base.size());

  BENCHMARK_ADVANCED("in-place m ^= map_values(f)")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::map<int, int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] ^= map_values(plus_one);
      return copies[i].size();
    });
  };
  BENCHMARK_ADVANCED("out-of-place m = m ^ map_values(f)")(Catch::Benchmark::Chronometer meter) {
    std::vector<std::map<int, int>> copies(meter.runs(), base);
    meter.measure([&](int i) {
      copies[i] = copies[i] ^ map_values(plus_one);
      return copies[i].size();
    });
  };
}
