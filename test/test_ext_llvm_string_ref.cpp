#include "catch2/catch_test_macros.hpp"

#include <aspartame/ext/llvm.hpp>

#include "fixtures.hpp"
#include "test_base_harness.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("llvm::StringRef_map", "[llvm::StringRef]") {
  llvm::StringRef s = "hello";
  auto ys = s ^ map([](char c) { return static_cast<char>(c + 1); });
  static_assert(std::is_same_v<decltype(ys), llvm::SmallVector<char>>);
  REQUIRE(ys.size() == 5);
  REQUIRE(ys[0] == 'i');
}

TEST_CASE("llvm::StringRef_filter", "[llvm::StringRef]") {
  llvm::StringRef s = "hello world";
  auto vowels = s ^ filter([](char c) { return c == 'o' || c == 'e'; });
  REQUIRE(vowels.size() == 3);
}

TEST_CASE("llvm::StringRef_count_fold", "[llvm::StringRef]") {
  llvm::StringRef s = "banana";
  REQUIRE((s ^ count([](char c) { return c == 'a'; })) == 3u);
  REQUIRE((s ^ fold_left(0, [](int acc, char c) { return acc + (c == 'a' ? 1 : 0); })) == 3);
}

TEST_CASE("llvm::StringRef_empty", "[llvm::StringRef]") {
  llvm::StringRef s = "";
  REQUIRE((s ^ count([](char) { return true; })) == 0u);
  auto ys = s ^ map([](char c) { return c; });
  REQUIRE(ys.empty());
}
