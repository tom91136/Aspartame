#include <vector>

#include "catch2/catch_test_macros.hpp"
#include <llvm/ADT/iterator_range.h>

#include <aspartame/ext/llvm.hpp>

#include "fixtures.hpp"
#include "test_base_harness.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("llvm::iterator_range_map_filter", "[llvm::iterator_range]") {
  std::vector<int> backing{4, 2, 3, 1, 5};
  auto r = llvm::make_range(backing.begin(), backing.end());
  auto doubled = r ^ map([](int x) { return x * 2; });
  static_assert(std::is_same_v<decltype(doubled), llvm::SmallVector<int>>);
  REQUIRE(doubled.size() == 5);
  REQUIRE(doubled[0] == 8);

  auto evens = r ^ filter([](int x) { return x % 2 == 0; });
  REQUIRE(evens.size() == 2);
}

TEST_CASE("llvm::iterator_range_fold_count", "[llvm::iterator_range]") {
  std::vector<int> backing{1, 2, 3, 4, 5};
  auto r = llvm::make_range(backing.begin(), backing.end());
  REQUIRE((r ^ fold_left(0, [](int a, int b) { return a + b; })) == 15);
  REQUIRE((r ^ count([](int x) { return x > 2; })) == 3u);
}

TEST_CASE("llvm::iterator_range_index_of_min", "[llvm::iterator_range]") {
  std::vector<int> backing{4, 2, 3, 1, 5};
  auto r = llvm::make_range(backing.begin(), backing.end());
  REQUIRE((r ^ index_of_min()) == std::optional<size_t>{3});
  REQUIRE((r ^ index_of_max()) == std::optional<size_t>{4});
}

TEST_CASE("llvm::iterator_range_empty", "[llvm::iterator_range]") {
  std::vector<int> backing;
  auto r = llvm::make_range(backing.begin(), backing.end());
  REQUIRE((r ^ count([](int) { return true; })) == 0u);
}

TEST_CASE("llvm::iterator_range_flat_map", "[llvm::iterator_range]") {
  std::vector<int> backing{1, 2, 3};
  auto r = llvm::make_range(backing.begin(), backing.end());
  auto doubled = r ^ flat_map([](int x) { return std::vector<int>{x, x}; });
  REQUIRE(doubled.size() == 6);
}

TEST_CASE("llvm::iterator_range_distinct", "[llvm::iterator_range]") {
  std::vector<int> backing{1, 2, 2, 3, 3, 3};
  auto r = llvm::make_range(backing.begin(), backing.end());
  auto d = r ^ distinct();
  REQUIRE(d.size() == 3);
}

TEST_CASE("llvm::iterator_range_partition", "[llvm::iterator_range]") {
  std::vector<int> backing{1, 2, 3, 4, 5};
  auto r = llvm::make_range(backing.begin(), backing.end());
  auto [evens, odds] = r ^ partition([](int x) { return x % 2 == 0; });
  REQUIRE(evens.size() == 2);
  REQUIRE(odds.size() == 3);
}

TEST_CASE("llvm::iterator_range_zip_with_index", "[llvm::iterator_range][zip_with_index][regression]") {
  std::vector<int> backing{40, 41, 42};
  auto r = llvm::make_range(backing.begin(), backing.end());
  auto pairs = r ^ zip_with_index<size_t>();
  REQUIRE(pairs.size() == 3);
  REQUIRE(pairs[0].first == 40);
  REQUIRE(pairs[0].second == 0u);
  REQUIRE(pairs[2].first == 42);
  REQUIRE(pairs[2].second == 2u);
}
