#include "catch2/catch_test_macros.hpp"
#include <llvm/ADT/ArrayRef.h>

#include <aspartame/ext/llvm.hpp>

#include "fixtures.hpp"
#include "test_base_harness.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("llvm::ArrayRef_map", "[llvm::ArrayRef]") {
  int storage[] = {4, 2, 3, 1, 5};
  llvm::ArrayRef<int> xs{storage, 5};
  auto ys = xs ^ map([](int x) { return x * 2; });
  static_assert(std::is_same_v<decltype(ys), llvm::SmallVector<int>>);
  REQUIRE(ys.size() == 5);
  REQUIRE(ys[0] == 8);
  REQUIRE(ys[4] == 10);
}

TEST_CASE("llvm::ArrayRef_filter_fold", "[llvm::ArrayRef]") {
  int storage[] = {1, 2, 3, 4, 5};
  llvm::ArrayRef<int> xs{storage, 5};
  auto evens = xs ^ filter([](int x) { return x % 2 == 0; });
  static_assert(std::is_same_v<decltype(evens), llvm::SmallVector<int>>);
  REQUIRE(evens.size() == 2);

  REQUIRE((xs ^ fold_left(0, [](int a, int b) { return a + b; })) == 15);
  REQUIRE((xs ^ count([](int x) { return x > 2; })) == 3u);
}

TEST_CASE("llvm::ArrayRef_index_of_min_max", "[llvm::ArrayRef]") {
  int storage[] = {4, 2, 3, 1, 5};
  llvm::ArrayRef<int> xs{storage, 5};
  REQUIRE((xs ^ index_of_min()) == std::optional<size_t>{3});
  REQUIRE((xs ^ index_of_max()) == std::optional<size_t>{4});
}

TEST_CASE("llvm::ArrayRef_empty", "[llvm::ArrayRef]") {
  llvm::ArrayRef<int> xs{};
  REQUIRE(xs.empty());
  auto ys = xs ^ map([](int x) { return x + 1; });
  REQUIRE(ys.empty());
}
