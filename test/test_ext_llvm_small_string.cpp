#include "catch2/catch_test_macros.hpp"
#include <llvm/ADT/SmallString.h>

#include <aspartame/ext/llvm.hpp>

#include "fixtures.hpp"
#include "test_base_harness.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("llvm::SmallString_map", "[llvm::SmallString]") {
  llvm::SmallString<32> s("hello");
  auto ys = s ^ map([](char c) { return static_cast<char>(c + 1); });
  static_assert(std::is_same_v<decltype(ys), llvm::SmallVector<char, 32>>);
  REQUIRE(ys.size() == 5);
  REQUIRE(ys[0] == 'i');
}

TEST_CASE("llvm::SmallString_filter_count", "[llvm::SmallString]") {
  llvm::SmallString<32> s("banana");
  auto as = s ^ filter([](char c) { return c == 'a'; });
  REQUIRE(as.size() == 3);
  REQUIRE((s ^ count([](char c) { return c == 'n'; })) == 2u);
}

TEST_CASE("llvm::SmallString_fold", "[llvm::SmallString]") {
  llvm::SmallString<32> s("abc");
  REQUIRE((s ^ fold_left(0, [](int acc, char c) { return acc + c; })) == 'a' + 'b' + 'c');
}

TEST_CASE("llvm::SmallString_empty", "[llvm::SmallString]") {
  llvm::SmallString<32> s;
  REQUIRE((s ^ count([](char) { return true; })) == 0u);
}
