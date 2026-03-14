#include "test_base_includes.hpp"

#include <array>
#include <deque>
#include <type_traits>
#include <vector>

#include "catch2/catch_test_macros.hpp"

using namespace ::aspartame;

namespace {
struct NoDefault {
  explicit NoDefault(int v) : value(v) {}
  NoDefault() = delete;
  int value;
  bool operator==(const NoDefault &rhs) const { return value == rhs.value; }
};
} // namespace

TEST_CASE("std::array_container_ops", "[std::array][test_array]") {
  const std::array<int, 5> xs{4, 2, 3, 1, 5};

  auto mapped = xs ^ map([](int x) { return x * 2; });
  CHECK((std::is_same_v<decltype(mapped), std::vector<int>>));
  CHECK(mapped == std::vector<int>{8, 4, 6, 2, 10});

  auto mapped_non_default = xs ^ map([](int x) { return NoDefault(x * 3); });
  CHECK((mapped_non_default == std::vector<NoDefault>{NoDefault(12), NoDefault(6), NoDefault(9), NoDefault(3), NoDefault(15)}));

  auto filtered = xs ^ filter([](int x) { return x % 2 == 0; });
  CHECK(filtered == std::vector<int>{4, 2});

  auto appended = xs ^ append(6);
  CHECK(appended == std::vector<int>{4, 2, 3, 1, 5, 6});

  auto prepended = xs ^ prepend(9);
  CHECK(prepended == std::vector<int>{9, 4, 2, 3, 1, 5});
}

TEST_CASE("std::array_sequence_ops", "[std::array][test_array]") {
  const std::array<int, 5> xs{4, 2, 3, 1, 5};

  CHECK((xs ^ head_maybe()) == std::optional<int>{4});
  CHECK((xs ^ last_maybe()) == std::optional<int>{5});
  CHECK((xs ^ at_maybe(1)) == std::optional<int>{2});
  CHECK((xs ^ at_maybe(100)) == std::optional<int>{});

  CHECK((xs ^ slice(1, 4)) == std::vector<int>{2, 3, 1});
  CHECK((xs ^ take(3)) == std::vector<int>{4, 2, 3});
  CHECK((xs ^ drop(3)) == std::vector<int>{1, 5});
  CHECK((xs ^ take_right(2)) == std::vector<int>{1, 5});
  CHECK((xs ^ drop_right(2)) == std::vector<int>{4, 2, 3});

  CHECK((xs ^ reverse()) == std::vector<int>{5, 1, 3, 2, 4});
  CHECK((xs ^ sort()) == std::vector<int>{1, 2, 3, 4, 5});

  auto split = xs ^ split_at(2);
  CHECK(split == std::pair{std::vector<int>{4, 2}, std::vector<int>{3, 1, 5}});

  auto windows = xs ^ sliding(2, 2);
  CHECK(windows == std::vector<std::vector<int>>{{4, 2}, {3, 1}, {5}});
}

TEST_CASE("std::array_group_and_fold_ops", "[std::array][test_array]") {
  const std::array<int, 5> xs{4, 2, 3, 1, 5};

  CHECK((xs ^ fold_left(0, [](int acc, int x) { return acc + x; })) == 15);
  CHECK((xs ^ fold_right(0, [](int x, int acc) { return acc + x; })) == 15);
  CHECK((xs ^ index_of(3)) == 2);
  CHECK((xs ^ index_where([](int x) { return x % 2 != 0; })) == 2);

  auto grouped = xs ^ group_by([](int x) { return x % 2; });
  REQUIRE(grouped.size() == 2);
  CHECK(grouped.at(0) == std::vector<int>{4, 2});
  CHECK(grouped.at(1) == std::vector<int>{3, 1, 5});
}

TEST_CASE("std::array_nested_and_conversion_ops", "[std::array][test_array]") {
  const std::array<std::array<int, 2>, 2> nested{{std::array<int, 2>{1, 2}, std::array<int, 2>{3, 4}}};

  auto transposed = nested ^ transpose();
  CHECK(transposed == std::vector<std::vector<int>>{{1, 3}, {2, 4}});

  auto sequenced = nested ^ sequence();
  CHECK(sequenced == std::vector<std::vector<int>>{{1, 3}, {1, 4}, {2, 3}, {2, 4}});

  const std::array<int, 3> xs{9, 8, 7};
  CHECK((xs ^ to_vector()) == std::vector<int>{9, 8, 7});
  CHECK((xs ^ to<std::deque>()) == std::deque<int>{9, 8, 7});
}
