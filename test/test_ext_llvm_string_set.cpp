#include <algorithm>

#include "catch2/catch_test_macros.hpp"
#include <llvm/ADT/StringSet.h>

#include <aspartame/ext/llvm.hpp>

#include "fixtures.hpp"
#include "test_base_harness.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

namespace {
std::vector<std::string> sorted_strs(const llvm::SmallVector<llvm::StringRef> &xs) {
  std::vector<std::string> out;
  out.reserve(xs.size());
  for (auto sr : xs)
    out.emplace_back(sr.str());
  std::sort(out.begin(), out.end());
  return out;
}
} // namespace

TEST_CASE("llvm::StringSet_map", "[llvm::StringSet]") {
  llvm::StringSet<> xs;
  xs.insert("apple");
  xs.insert("banana");
  xs.insert("cherry");
  auto upper = xs ^ map([](llvm::StringRef s) { return s.size(); });
  static_assert(std::is_same_v<decltype(upper), llvm::SmallVector<size_t>>);
  REQUIRE(upper.size() == 3);
}

TEST_CASE("llvm::StringSet_filter_count", "[llvm::StringSet]") {
  llvm::StringSet<> xs;
  xs.insert("apple");
  xs.insert("apricot");
  xs.insert("banana");
  xs.insert("cherry");
  REQUIRE((xs ^ count([](llvm::StringRef s) { return s.starts_with("a"); })) == 2u);

  auto longs = xs ^ filter([](llvm::StringRef s) { return s.size() > 5; });
  static_assert(std::is_same_v<decltype(longs), llvm::SmallVector<llvm::StringRef>>);
  REQUIRE(sorted_strs(longs) == std::vector<std::string>{"apricot", "banana", "cherry"});
}

TEST_CASE("llvm::StringSet_exists_forall", "[llvm::StringSet]") {
  llvm::StringSet<> xs;
  xs.insert("foo");
  xs.insert("bar");
  REQUIRE((xs ^ exists([](llvm::StringRef s) { return s == "foo"; })));
  REQUIRE_FALSE((xs ^ exists([](llvm::StringRef s) { return s == "baz"; })));
  REQUIRE((xs ^ forall([](llvm::StringRef s) { return s.size() == 3; })));
}

TEST_CASE("llvm::StringSet_fold", "[llvm::StringSet]") {
  llvm::StringSet<> xs;
  xs.insert("ab");
  xs.insert("c");
  xs.insert("def");
  REQUIRE((xs ^ fold_left(0u, [](unsigned a, llvm::StringRef s) { return a + s.size(); })) == 6u);
}

TEST_CASE("llvm::StringSet_to_vector", "[llvm::StringSet]") {
  llvm::StringSet<> xs;
  xs.insert("a");
  xs.insert("b");
  xs.insert("c");
  auto v = xs ^ to_vector();
  static_assert(std::is_same_v<decltype(v), std::vector<llvm::StringRef>>);
  REQUIRE(v.size() == 3);
}

TEST_CASE("llvm::StringSet_empty", "[llvm::StringSet]") {
  llvm::StringSet<> xs;
  REQUIRE((xs ^ count([](llvm::StringRef) { return true; })) == 0u);
}
