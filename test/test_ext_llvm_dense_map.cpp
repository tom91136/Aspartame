#include "catch2/catch_test_macros.hpp"

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename K, typename V, typename Info, typename Bucket>
std::ostream &operator<<(std::ostream &os, const DenseMap<K, V, Info, Bucket> &xs) {
  os << "llvm::DenseMap{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
} // namespace llvm

using namespace aspartame;

TEST_CASE("llvm::DenseMap_map_rebind", "[llvm::DenseMap]") {
  llvm::DenseMap<int, int> xs;
  xs.insert({1, 10});
  xs.insert({2, 20});
  auto ys = xs ^ map([](auto k, auto v) { return std::pair<long, long>{k, v * 2L}; });
  static_assert(std::is_same_v<decltype(ys), llvm::DenseMap<long, long>>);
  REQUIRE(ys.lookup(1L) == 20L);
  REQUIRE(ys.lookup(2L) == 40L);
}

#define OP_ ^
#define TPE_GROUP "test_llvm_dense_map"
#define TPE_NAME "llvm::DenseMap"
#define TPE_CTOR_IN(K, V) llvm::DenseMap<K, V>
#define TPE_CTOR_OUT(K, V) llvm::DenseMap<K, V>
#define TPE_CTOR_VAR_OUT(T) llvm::SmallVector<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
