#include "catch2/catch_test_macros.hpp"

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename K, typename V, unsigned InlineBuckets, typename Info, typename Bucket>
std::ostream &operator<<(std::ostream &os, const SmallDenseMap<K, V, InlineBuckets, Info, Bucket> &xs) {
  os << "llvm::SmallDenseMap{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
} // namespace llvm

#define OP_ ^
#define TPE_GROUP "test_llvm_small_dense_map"
#define TPE_NAME "llvm::SmallDenseMap"
#define TPE_CTOR_IN(K, V) llvm::SmallDenseMap<K, V>
#define TPE_CTOR_OUT(K, V) llvm::SmallDenseMap<K, V>
#define TPE_CTOR_VAR_OUT(T) llvm::SmallVector<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
