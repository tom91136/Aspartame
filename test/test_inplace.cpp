#include "test_base_includes.hpp"
#include "fixtures.hpp"

#include <catch2/catch_test_macros.hpp>

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

