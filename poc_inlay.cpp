// POC: CLion Nova inlay hint test for aspartame-style patterns
// Open this file in CLion and check if inlay hints appear on the auto variables.
//
// Build: g++ -std=c++17 -o poc_inlay poc_inlay.cpp && ./poc_inlay

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================
// Part 0: Normal code (baseline for CLion inlay comparison)
// ============================================================

std::vector<int> normal_filter(const std::vector<int> &v) {
  std::vector<int> out;
  for (auto &x : v)
    if (x > 2) out.push_back(x);
  return out;
}

int normal_reduce(const std::vector<int> &v) {
  int sum = 0;
  for (auto &x : v) sum += x;
  return sum;
}

void test_normal() {
  auto xs = std::vector<int>{1, 2, 3, 4, 5};  // CLion should show: vector<int>
  auto filtered = normal_filter(xs);             // CLion should show: vector<int>
  auto sum = normal_reduce(filtered);            // CLion should show: int
  std::cout << "normal: " << sum << "\n";
}

// ============================================================
// Part 1: Mimics aspartame WITHOUT trailing return types
//         (what the code looks like today)
// ============================================================
namespace no_trailing {

struct tag {};

// Free functions that do the real work
template <typename Predicate>
std::vector<int> filter(const std::vector<int> &v, Predicate &&pred, tag) {
  std::vector<int> out;
  for (auto &x : v)
    if (pred(x)) out.push_back(x);
  return out;
}

template <typename Function>
auto map(const std::vector<int> &v, Function &&fn, tag) {
  using R = decltype(fn(std::declval<int>()));
  std::vector<R> out;
  for (auto &x : v) out.push_back(fn(x));
  return out;
}

template <typename Function>
auto reduce(const std::vector<int> &v, Function &&fn, tag) {
  auto acc = v[0];
  for (size_t i = 1; i < v.size(); ++i) acc = fn(acc, v[i]);
  return acc;
}

// Fluent wrappers — NO trailing return type on inner lambda
template <typename Predicate>
constexpr auto filter(Predicate &&pred) {
  return [&](auto &&o, tag) { return filter(o, pred, tag{}); };
}

template <typename Function>
constexpr auto map(Function &&fn) {
  return [&](auto &&o, tag) { return map(o, fn, tag{}); };
}

template <typename Function>
constexpr auto reduce(Function &&fn) {
  return [&](auto &&o, tag) { return reduce(o, fn, tag{}); };
}

// Pipe operator
template <typename T, typename Op>
auto operator^(T &&lhs, const Op &op) {
  return op(std::forward<T>(lhs), tag{});
}

void test() {
  auto xs = std::vector<int>{1, 2, 3, 4, 5};

  // CLion Nova probably shows: auto (unknown) for all of these
  auto filtered = xs ^ filter([](int x) { return x > 2; });
  auto mapped = filtered ^ map([](int x) { return x * 10; });
  auto sum = mapped ^ reduce([](int a, int b) { return a + b; });

  std::cout << "no_trailing: " << sum << "\n";
}

} // namespace no_trailing

// ============================================================
// Part 2: Mimics aspartame WITH trailing return types
//         (proposed fix)
// ============================================================
namespace with_trailing {

struct tag {};

// Free functions (same as above)
template <typename Predicate>
std::vector<int> filter(const std::vector<int> &v, Predicate &&pred, tag) {
  std::vector<int> out;
  for (auto &x : v)
    if (pred(x)) out.push_back(x);
  return out;
}

template <typename Function>
auto map(const std::vector<int> &v, Function &&fn, tag) {
  using R = decltype(fn(std::declval<int>()));
  std::vector<R> out;
  for (auto &x : v) out.push_back(fn(x));
  return out;
}

template <typename Function>
auto reduce(const std::vector<int> &v, Function &&fn, tag) {
  auto acc = v[0];
  for (size_t i = 1; i < v.size(); ++i) acc = fn(acc, v[i]);
  return acc;
}

// Fluent wrappers — WITH trailing return type on inner lambda
template <typename Predicate>
constexpr auto filter(Predicate &&pred) {
  return [&](auto &&o, tag) -> decltype(filter(o, pred, tag{})) {
    return filter(o, pred, tag{});
  };
}

template <typename Function>
constexpr auto map(Function &&fn) {
  return [&](auto &&o, tag) -> decltype(map(o, fn, tag{})) {
    return map(o, fn, tag{});
  };
}

template <typename Function>
constexpr auto reduce(Function &&fn) {
  return [&](auto &&o, tag) -> decltype(reduce(o, fn, tag{})) {
    return reduce(o, fn, tag{});
  };
}

// Pipe operator — WITH trailing return type
template <typename T, typename Op>
auto operator^(T &&lhs, const Op &op) -> decltype(op(std::forward<T>(lhs), tag{})) {
  return op(std::forward<T>(lhs), tag{});
}

void test() {
  auto xs = std::vector<int>{1, 2, 3, 4, 5};

  // CLion Nova should now show: vector<int>, vector<int>, int
  auto filtered = xs ^ filter([](int x) { return x > 2; });
  auto mapped = filtered ^ map([](int x) { return x * 10; });
  auto sum = mapped ^ reduce([](int a, int b) { return a + b; });

  std::cout << "with_trailing: " << sum << "\n";
}

} // namespace with_trailing

// ============================================================
// Part 3: Same but with operator| (mimics view pipeline)
// ============================================================
namespace pipe_style {

struct tag {};

template <typename Predicate>
std::vector<int> filter(const std::vector<int> &v, Predicate &&pred, tag) {
  std::vector<int> out;
  for (auto &x : v)
    if (pred(x)) out.push_back(x);
  return out;
}

template <typename Predicate>
constexpr auto filter(Predicate &&pred) {
  return [&](auto &&o, tag) -> decltype(filter(o, pred, tag{})) {
    return filter(o, pred, tag{});
  };
}

template <typename T, typename Op>
auto operator|(T &&lhs, const Op &op) -> decltype(op(std::forward<T>(lhs), tag{})) {
  return op(std::forward<T>(lhs), tag{});
}

void test() {
  auto xs = std::vector<int>{1, 2, 3, 4, 5};
  auto filtered = xs | filter([](int x) { return x > 2; }); // should show vector<int>
  std::cout << "pipe_style: " << filtered.size() << "\n";
}

} // namespace pipe_style

int main() {
  test_normal();
  no_trailing::test();
  with_trailing::test();
  pipe_style::test();
  return 0;
}
