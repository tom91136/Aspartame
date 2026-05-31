#include "catch2/catch_test_macros.hpp"
#include <llvm/ADT/StringMap.h>

#include <aspartame/ext/llvm.hpp>

#include "fixtures.hpp"
#include "test_base_harness.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

namespace {
using SM = llvm::StringMap<int>;
} // namespace

TEST_CASE("llvm::StringMap_filter", "[llvm::StringMap]") {
  SM xs;
  xs["a"] = 10;
  xs["b"] = 20;
  xs["c"] = 30;
  auto ys = xs ^ filter([](llvm::StringRef k, int) { return k != "a"; });
  static_assert(std::is_same_v<decltype(ys), SM>);
  REQUIRE(ys.size() == 2);
  REQUIRE(ys.lookup("b") == 20);
  REQUIRE(ys.lookup("c") == 30);
}

TEST_CASE("llvm::StringMap_map_values", "[llvm::StringMap]") {
  SM xs;
  xs["a"] = 10;
  xs["b"] = 20;
  auto ys = xs ^ map_values([](int v) { return v + 1; });
  static_assert(std::is_same_v<decltype(ys), SM>);
  REQUIRE(ys.lookup("a") == 11);
  REQUIRE(ys.lookup("b") == 21);
}

TEST_CASE("llvm::StringMap_get_maybe", "[llvm::StringMap]") {
  SM xs;
  xs["x"] = 7;
  REQUIRE((xs ^ get_maybe(llvm::StringRef("x"))) == std::optional<int>{7});
  REQUIRE((xs ^ get_maybe(llvm::StringRef("nope"))) == std::nullopt);
}

TEST_CASE("llvm::StringMap_count", "[llvm::StringMap]") {
  SM xs;
  xs["a"] = 1;
  xs["b"] = 2;
  xs["c"] = 3;
  REQUIRE((xs ^ count([](llvm::StringRef, int v) { return v >= 2; })) == 2u);
}

TEST_CASE("llvm::StringMap_exists_forall", "[llvm::StringMap]") {
  SM xs;
  xs["a"] = 1;
  xs["b"] = 2;
  REQUIRE((xs ^ exists([](llvm::StringRef k, int) { return k == "a"; })));
  REQUIRE((xs ^ forall([](llvm::StringRef, int v) { return v > 0; })));
  REQUIRE_FALSE((xs ^ forall([](llvm::StringRef, int v) { return v > 1; })));
}

TEST_CASE("llvm::StringMap_empty", "[llvm::StringMap]") {
  SM xs;
  REQUIRE(xs.empty());
  auto ys = xs ^ filter([](llvm::StringRef, int) { return true; });
  REQUIRE(ys.empty());
}
