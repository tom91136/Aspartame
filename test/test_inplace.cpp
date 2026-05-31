#include <catch2/catch_test_macros.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("vector_concat_extends_in_place", "[inplace]") {
  std::vector<int> xs{1, 2, 3};
  xs ^= concat(std::vector<int>{4, 5});
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4, 5});
  xs.reserve(16);
  const auto *retained = xs.data();
  xs ^= concat(std::vector<int>{6, 7});
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4, 5, 6, 7});
  REQUIRE(xs.data() == retained);
}

TEST_CASE("vector_concat_returns_lhs_for_chaining", "[inplace]") {
  std::vector<int> xs{1};
  auto &same = (xs ^= concat(std::vector<int>{2, 3}));
  REQUIRE(&same == &xs);
  same ^= concat(std::vector<int>{4});
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("vector_concat_with_empty_other_is_noop", "[inplace]") {
  std::vector<int> xs{1, 2, 3};
  xs ^= concat(std::vector<int>{});
  REQUIRE(xs == std::vector<int>{1, 2, 3});
}

TEST_CASE("vector_concat_with_empty_self", "[inplace]") {
  std::vector<int> xs{};
  xs ^= concat(std::vector<int>{1, 2});
  REQUIRE(xs == std::vector<int>{1, 2});
}

TEST_CASE("vector_concat_with_strings", "[inplace]") {
  std::vector<string> xs{"a", "b"};
  xs ^= concat(std::vector<string>{"c", "d"});
  REQUIRE(xs == std::vector<string>{"a", "b", "c", "d"});
}

TEST_CASE("vector_concat_with_non_default_constructible", "[inplace]") {
  std::vector<Foo> xs{Foo(1), Foo(2)};
  xs ^= concat(std::vector<Foo>{Foo(3)});
  REQUIRE(xs == std::vector<Foo>{Foo(1), Foo(2), Foo(3)});
}

TEST_CASE("set_concat_unions", "[inplace]") {
  std::set<int> xs{1, 2, 3};
  xs ^= concat(std::set<int>{3, 4, 5});
  REQUIRE(xs == std::set<int>{1, 2, 3, 4, 5});
}

TEST_CASE("set_concat_with_empty", "[inplace]") {
  std::set<int> xs{};
  xs ^= concat(std::set<int>{1, 2});
  REQUIRE(xs == std::set<int>{1, 2});
}

TEST_CASE("unordered_set_concat_unions", "[inplace]") {
  std::unordered_set<int> xs{1, 2, 3};
  xs ^= concat(std::unordered_set<int>{3, 4, 5});
  REQUIRE(xs == std::unordered_set<int>{1, 2, 3, 4, 5});
}

TEST_CASE("vector_concat_accepts_other_iterable_types", "[inplace]") {
  std::vector<int> xs{1, 2};
  std::list<int> ys{3, 4};
  xs ^= concat(ys);
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("non_inplace_concat_still_returns_fresh_container", "[inplace]") {
  const std::vector<int> xs{1, 2};
  const auto ys = xs ^ concat(std::vector<int>{3, 4});
  REQUIRE(xs == std::vector<int>{1, 2});
  REQUIRE(ys == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("vector_filter_inplace_does_not_reallocate", "[inplace]") {
  std::vector<int> xs{1, 2, 3, 4, 5, 6};
  const auto *p = xs.data();
  xs ^= filter([](int x) { return x % 2 == 0; });
  REQUIRE(xs == std::vector<int>{2, 4, 6});
  REQUIRE(xs.data() == p);
}

TEST_CASE("vector_map_inplace_same_type_no_realloc", "[inplace]") {
  std::vector<int> xs{1, 2, 3};
  const auto *p = xs.data();
  xs ^= map([](int x) { return x * 2; });
  REQUIRE(xs == std::vector<int>{2, 4, 6});
  REQUIRE(xs.data() == p);
}

TEST_CASE("map_filter_inplace_node_erase", "[inplace]") {
  std::map<int, string> m{{1, "a"}, {2, "b"}, {3, "c"}};
  m ^= filter([](int k, const string &) { return k != 2; });
  REQUIRE(m.size() == 2);
  REQUIRE(m.count(2) == 0);
  REQUIRE(m.at(3) == "c");
}

TEST_CASE("set_map_inplace_falls_back_to_rebuild", "[inplace]") {
  std::set<int> s{1, 2, 3};
  s ^= map([](int x) { return x * 10; });
  REQUIRE(s == std::set<int>{10, 20, 30});
}

TEST_CASE("map_get_or_emplace_inplace", "[inplace]") {
  std::map<int, string> m{{1, "a"}};
  auto &same = (m ^= get_or_emplace(2, [](int k) { return std::to_string(k); }));
  REQUIRE(&same == &m);
  REQUIRE(m.size() == 2);
  REQUIRE(m.at(2) == "2");
  m ^= get_or_emplace(1, [](int) { return string("X"); });
  REQUIRE(m.at(1) == "a");
}

TEST_CASE("map_map_values_inplace", "[inplace]") {
  std::map<int, int> m{{1, 10}, {2, 20}, {3, 30}};
  auto &same = (m ^= map_values([](int v) { return v + 1; }));
  REQUIRE(&same == &m);
  REQUIRE(m.at(1) == 11);
  REQUIRE(m.at(3) == 31);
}

TEST_CASE("unordered_map_map_values_inplace", "[inplace]") {
  std::unordered_map<int, int> m{{1, 10}, {2, 20}};
  m ^= map_values([](int v) { return v * 2; });
  REQUIRE(m.at(2) == 40);
}

TEST_CASE("vector_self_concat_inplace_single_allocation", "[inplace]") {
  std::vector<int> xs{1, 2, 3};
  xs.reserve(6);
  const auto *p = xs.data();
  xs ^= concat(xs);
  REQUIRE(xs == std::vector<int>{1, 2, 3, 1, 2, 3});
  REQUIRE(xs.data() == p);
}

TEST_CASE("list_self_concat_inplace", "[inplace]") {
  std::list<int> xs{1, 2, 3};
  xs ^= concat(xs);
  REQUIRE(xs == std::list<int>{1, 2, 3, 1, 2, 3});
}

TEST_CASE("set_self_concat_inplace_is_noop", "[inplace]") {
  std::set<int> s{1, 2, 3};
  s ^= concat(s);
  REQUIRE(s == std::set<int>{1, 2, 3});
}

TEST_CASE("set_distinct_inplace_is_noop", "[inplace]") {
  std::set<int> s{1, 2, 3};
  s ^= distinct();
  REQUIRE(s == std::set<int>{1, 2, 3});
}
