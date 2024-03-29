#include "aspartame/optional.hpp"
#include "aspartame/view.hpp"

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <ranges>
#include <vector>
#include <numeric>

double view_pi(int iter) {
  using namespace aspartame;
  return 4 * (iota(0)                                                               //
              | map([](auto n) { return (n % 2 == 0 ? 1.0 : -1.0) / (2 * n + 1); }) //
              | take(iter)                                                          //
              | fold_left(0.0, std::plus<>()));
}

double range_pi(int iter) {
  using namespace std::views;
  auto r = iota(0)                                                                     //
           | transform([](auto n) { return (n % 2 == 0 ? 1.0 : -1.0) / (2 * n + 1); }) //
           | take(iter)                                                                //
           | common;                                                                   //
  return 4 * std::accumulate(r.begin(), r.end(), 0.0, std::plus<>());
}

TEST_CASE("pi") {
  volatile int iter = 1000000;

  auto Pi = std::atan(1) * 4;

  CHECK_THAT(view_pi(iter), Catch::Matchers::WithinRel(Pi, 0.00001));
  CHECK_THAT(range_pi(iter), Catch::Matchers::WithinRel(Pi, 0.00001));
  BENCHMARK("aspartame_view") { return view_pi(iter); };
  BENCHMARK("cxx20_ranges") { return range_pi(iter); };
}

bool is_prime(int n) {
  if (n <= 1) return false;
  for (int i = 2; i <= std::sqrt(n); ++i) {
    if (n % i == 0) return false;
  }
  return true;
}

int view_prime_gt_n(int n) {
  using namespace aspartame;
  return (iota(1)                                     //
          | filter(&is_prime)                         //
          | drop_while([&](auto x) { return x < n; }) //
          | head_maybe())                             //
         ^ get_or_else(1);
}

int range_prime_gt_n(int n) {
  using namespace std::views;
  auto r = iota(1)                                     //
           | filter(&is_prime)                         //
           | drop_while([&](auto x) { return x < n; }) //
           | common;                                   //
  return r.begin() != r.end() ? *r.begin() : 1;
}

TEST_CASE("prime") {
  volatile int iter = 30000;
  CHECK(view_prime_gt_n(iter) == 30011);
  CHECK(range_prime_gt_n(iter) == 30011);
  BENCHMARK("aspartame_view") { return view_prime_gt_n(iter); };
  BENCHMARK("cxx20_ranges") { return range_prime_gt_n(iter); };
}

long long view_fib_product(int lim) {
  using namespace aspartame;
  return iota(1L)        //
         | take(lim + 1) //
         | fold_left(1LL, std::multiplies<>());
}

long long range_fib_product(int lim) {
  using namespace std::views;
  auto r = iota(1L) | take(lim + 1) | common;
  return std::accumulate(r.begin(), r.end(), 1LL, std::multiplies<>());
}

TEST_CASE("fib") {
  volatile int iter = 48; // XXX yes, this is testing a very small range
  CHECK(view_fib_product(iter) == 8789267254022766592);
  CHECK(range_fib_product(iter) == 8789267254022766592);
  BENCHMARK("aspartame_view") { return view_fib_product(iter); };
  BENCHMARK("cxx20_ranges") { return range_fib_product(iter); };
}
