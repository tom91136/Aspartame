#include <aspartame/ext/llvm.hpp>

// XXX DenseSet<std::optional<T>> requires a DenseMapInfo<std::optional<T>> specialization
// that only exists on LLVM 22+
#define DISABLE_SEQUENCE
#define DISABLE_SEQUENCE_NESTED

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename T, typename Info> std::ostream &operator<<(std::ostream &os, const DenseSet<T, Info> &xs) {
  os << "llvm::DenseSet{";
  bool first = true;
  for (const auto &x : xs) {
    if (!first) os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
} // namespace llvm

namespace {
template <typename T> llvm::DenseSet<T> to_dense_set(std::initializer_list<T> xs) {
  llvm::DenseSet<T> s;
  for (const auto &x : xs)
    s.insert(x);
  return s;
}
} // namespace

#define OP_ ^
#define TPE_GROUP "test_llvm_dense_set"
#define TPE_NAME "llvm::DenseSet"
#define TPE_CTOR_IN(T) llvm::DenseSet<T>
#define TPE_CTOR_OUT(T) llvm::DenseSet<T>
#define TPE_CTOR_VAR_OUT(T) llvm::DenseSet<T>
#define TPE_INIT_TO_CTOR_IN(x) to_dense_set(x)
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED

// DenseSet<DenseSet<T>> unimplementable: no DenseMapInfo for DenseSet.
#define DISABLE_FLATTEN
#define DISABLE_TAP

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_inplace.hpp"
