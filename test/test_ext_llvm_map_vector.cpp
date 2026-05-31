#include "catch2/catch_test_macros.hpp"
#include <llvm/ADT/MapVector.h>

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename K, typename V, typename M, typename Vec> std::ostream &operator<<(std::ostream &os, const MapVector<K, V, M, Vec> &xs) {
  os << "llvm::MapVector{";
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
#define TPE_GROUP "test_llvm_map_vector"
#define TPE_NAME "llvm::MapVector"
#define TPE_CTOR_IN(K, V) llvm::MapVector<K, V>
#define TPE_CTOR_OUT(K, V) llvm::MapVector<K, V>
#define TPE_CTOR_VAR_OUT(T) llvm::SmallVector<T>
#define TPE_INIT_TO_CTOR_IN(x) x

#include "templates/test_template_map.hpp"
