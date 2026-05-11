#include "test_base_includes.hpp"
#include "fixtures.hpp"

#include <catch2/catch_test_macros.hpp>

#include <random>

using namespace aspartame;

TEST_CASE("vector_concat_inplace_extends_in_place", "[inplace]") {
  std::vector<int> xs{1, 2, 3};
  const auto *original = xs.data();
  xs ^= concat_inplace(std::vector<int>{4, 5});
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4, 5});
  // small-buffer-free vector reallocates if it grows past capacity, so we only
  // check identity when capacity was already sufficient
  xs.reserve(16);
  const auto *retained = xs.data();
  xs ^= concat_inplace(std::vector<int>{6, 7});
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4, 5, 6, 7});
  REQUIRE(xs.data() == retained);
  (void)original;
}

TEST_CASE("vector_concat_inplace_returns_lhs_for_chaining", "[inplace]") {
  std::vector<int> xs{1};
  auto &same = (xs ^= concat_inplace(std::vector<int>{2, 3}));
  REQUIRE(&same == &xs);
  same ^= concat_inplace(std::vector<int>{4});
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("vector_concat_inplace_with_empty_other_is_noop", "[inplace]") {
  std::vector<int> xs{1, 2, 3};
  xs ^= concat_inplace(std::vector<int>{});
  REQUIRE(xs == std::vector<int>{1, 2, 3});
}

TEST_CASE("vector_concat_inplace_with_empty_self", "[inplace]") {
  std::vector<int> xs{};
  xs ^= concat_inplace(std::vector<int>{1, 2});
  REQUIRE(xs == std::vector<int>{1, 2});
}

TEST_CASE("vector_concat_inplace_self_concat", "[inplace]") {
  std::vector<int> xs{1, 2};
  xs.reserve(8);
  xs ^= concat_inplace(xs);
  REQUIRE(xs == std::vector<int>{1, 2, 1, 2});
}

TEST_CASE("vector_concat_inplace_with_strings", "[inplace]") {
  std::vector<string> xs{"a", "b"};
  xs ^= concat_inplace(std::vector<string>{"c", "d"});
  REQUIRE(xs == std::vector<string>{"a", "b", "c", "d"});
}

TEST_CASE("vector_concat_inplace_with_non_default_constructible", "[inplace]") {
  std::vector<Foo> xs{Foo(1), Foo(2)};
  xs ^= concat_inplace(std::vector<Foo>{Foo(3)});
  REQUIRE(xs == std::vector<Foo>{Foo(1), Foo(2), Foo(3)});
}

TEST_CASE("list_concat_inplace_extends_in_place", "[inplace]") {
  std::list<int> xs{1, 2, 3};
  xs ^= concat_inplace(std::list<int>{4, 5});
  REQUIRE(xs == std::list<int>{1, 2, 3, 4, 5});
}

TEST_CASE("deque_concat_inplace_extends_in_place", "[inplace]") {
  std::deque<int> xs{1, 2, 3};
  xs ^= concat_inplace(std::deque<int>{4, 5});
  REQUIRE(xs == std::deque<int>{1, 2, 3, 4, 5});
}

TEST_CASE("set_concat_inplace_unions", "[inplace]") {
  std::set<int> xs{1, 2, 3};
  xs ^= concat_inplace(std::set<int>{3, 4, 5});
  REQUIRE(xs == std::set<int>{1, 2, 3, 4, 5});
}

TEST_CASE("set_concat_inplace_with_empty", "[inplace]") {
  std::set<int> xs{};
  xs ^= concat_inplace(std::set<int>{1, 2});
  REQUIRE(xs == std::set<int>{1, 2});
}

TEST_CASE("unordered_set_concat_inplace_unions", "[inplace]") {
  std::unordered_set<int> xs{1, 2, 3};
  xs ^= concat_inplace(std::unordered_set<int>{3, 4, 5});
  REQUIRE(xs == std::unordered_set<int>{1, 2, 3, 4, 5});
}

TEST_CASE("vector_concat_inplace_accepts_other_iterable_types", "[inplace]") {
  // concat_inplace is templated on Container, so any range with .begin()/.end() works
  std::vector<int> xs{1, 2};
  std::list<int> ys{3, 4};
  xs ^= concat_inplace(ys);
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("non_inplace_concat_still_returns_fresh_container", "[inplace]") {
  // The pre-existing concat() / ^ operator is unaffected: it produces a new container
  // and does not touch the lhs.
  const std::vector<int> xs{1, 2};
  const auto ys = xs ^ concat(std::vector<int>{3, 4});
  REQUIRE(xs == std::vector<int>{1, 2});  // lhs unchanged
  REQUIRE(ys == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("vector_sort_inplace_default_comparator", "[inplace]") {
  std::vector<int> xs{3, 1, 4, 1, 5, 9, 2, 6};
  xs.reserve(16);
  const auto *retained = xs.data();
  xs ^= sort_inplace();
  REQUIRE(xs == std::vector<int>{1, 1, 2, 3, 4, 5, 6, 9});
  REQUIRE(xs.data() == retained);
}

TEST_CASE("vector_sort_inplace_with_comparator", "[inplace]") {
  std::vector<int> xs{1, 4, 2, 8, 5};
  xs ^= sort_inplace(std::greater<int>{});
  REQUIRE(xs == std::vector<int>{8, 5, 4, 2, 1});
}

TEST_CASE("vector_sort_inplace_empty_is_noop", "[inplace]") {
  std::vector<int> xs{};
  xs ^= sort_inplace();
  REQUIRE(xs.empty());
}

TEST_CASE("vector_sort_inplace_returns_lhs_for_chaining", "[inplace]") {
  std::vector<int> xs{3, 1, 2};
  auto &same = (xs ^= sort_inplace());
  REQUIRE(&same == &xs);
}

TEST_CASE("list_sort_inplace_uses_member_sort", "[inplace]") {
  std::list<int> xs{3, 1, 4, 1, 5};
  xs ^= sort_inplace();
  REQUIRE(xs == std::list<int>{1, 1, 3, 4, 5});
}

TEST_CASE("list_sort_inplace_with_comparator", "[inplace]") {
  std::list<int> xs{3, 1, 4};
  xs ^= sort_inplace(std::greater<int>{});
  REQUIRE(xs == std::list<int>{4, 3, 1});
}

TEST_CASE("deque_sort_inplace", "[inplace]") {
  std::deque<int> xs{3, 1, 4, 1, 5};
  xs ^= sort_inplace();
  REQUIRE(xs == std::deque<int>{1, 1, 3, 4, 5});
}

TEST_CASE("vector_stable_sort_inplace_preserves_relative_order", "[inplace]") {
  // pair sorted by first only; second order must be preserved among equal keys
  std::vector<std::pair<int, int>> xs{{1, 0}, {2, 0}, {1, 1}, {2, 1}, {1, 2}};
  xs ^= stable_sort_inplace([](auto a, auto b) { return a.first < b.first; });
  REQUIRE(xs == std::vector<std::pair<int, int>>{{1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}});
}

TEST_CASE("vector_stable_sort_inplace_default_comparator", "[inplace]") {
  std::vector<int> xs{3, 1, 4, 1, 5};
  xs ^= stable_sort_inplace();
  REQUIRE(xs == std::vector<int>{1, 1, 3, 4, 5});
}

TEST_CASE("list_stable_sort_inplace_uses_stable_member_sort", "[inplace]") {
  // std::list::sort is stable per the standard
  std::list<std::pair<int, int>> xs{{1, 0}, {2, 0}, {1, 1}, {2, 1}};
  xs ^= stable_sort_inplace([](auto a, auto b) { return a.first < b.first; });
  REQUIRE(xs == std::list<std::pair<int, int>>{{1, 0}, {1, 1}, {2, 0}, {2, 1}});
}

TEST_CASE("vector_sort_by_inplace", "[inplace]") {
  std::vector<std::pair<int, string>> xs{{3, "c"}, {1, "a"}, {2, "b"}};
  xs ^= sort_by_inplace([](auto x) { return x.first; });
  REQUIRE(xs == std::vector<std::pair<int, string>>{{1, "a"}, {2, "b"}, {3, "c"}});
}

TEST_CASE("vector_sort_by_inplace_with_foo", "[inplace]") {
  std::vector<Foo> xs{Foo(3), Foo(1), Foo(2)};
  xs ^= sort_by_inplace([](const Foo &x) { return x.value; });
  REQUIRE(xs == std::vector<Foo>{Foo(1), Foo(2), Foo(3)});
}

TEST_CASE("list_sort_by_inplace", "[inplace]") {
  std::list<std::pair<int, int>> xs{{3, 0}, {1, 0}, {2, 0}};
  xs ^= sort_by_inplace([](auto x) { return x.first; });
  REQUIRE(xs == std::list<std::pair<int, int>>{{1, 0}, {2, 0}, {3, 0}});
}

TEST_CASE("vector_reverse_inplace", "[inplace]") {
  std::vector<int> xs{1, 2, 3, 4, 5};
  xs ^= reverse_inplace();
  REQUIRE(xs == std::vector<int>{5, 4, 3, 2, 1});
}

TEST_CASE("vector_reverse_inplace_empty_is_noop", "[inplace]") {
  std::vector<int> xs{};
  xs ^= reverse_inplace();
  REQUIRE(xs.empty());
}

TEST_CASE("vector_reverse_inplace_returns_lhs_for_chaining", "[inplace]") {
  std::vector<int> xs{1, 2, 3};
  auto &same = (xs ^= reverse_inplace());
  REQUIRE(&same == &xs);
}

TEST_CASE("list_reverse_inplace_uses_member_reverse", "[inplace]") {
  std::list<int> xs{1, 2, 3};
  xs ^= reverse_inplace();
  REQUIRE(xs == std::list<int>{3, 2, 1});
}

TEST_CASE("deque_reverse_inplace", "[inplace]") {
  std::deque<int> xs{1, 2, 3};
  xs ^= reverse_inplace();
  REQUIRE(xs == std::deque<int>{3, 2, 1});
}

TEST_CASE("vector_shuffle_inplace_preserves_elements", "[inplace]") {
  std::vector<int> xs{1, 2, 3, 4, 5, 6, 7, 8};
  std::mt19937 rng{0xC0FFEE};
  xs ^= shuffle_inplace(rng);
  REQUIRE(xs.size() == 8);
  auto sorted = xs;
  sorted ^= sort_inplace();
  REQUIRE(sorted == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8});
}

TEST_CASE("vector_chained_inplace_ops", "[inplace]") {
  std::vector<int> xs{3, 1, 4, 1, 5, 9, 2, 6};
  (xs ^= sort_inplace()) ^= reverse_inplace();
  REQUIRE(xs == std::vector<int>{9, 6, 5, 4, 3, 2, 1, 1});
}

