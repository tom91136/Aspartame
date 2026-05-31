#include "catch2/catch_test_macros.hpp"

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename T, typename V, typename S> std::ostream &operator<<(std::ostream &os, const SetVector<T, V, S> &xs) {
  os << "llvm::SetVector{";
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
template <typename T> llvm::SetVector<T> to_set_vector(std::initializer_list<T> xs) {
  llvm::SetVector<T> s;
  for (const auto &x : xs)
    s.insert(x);
  return s;
}
} // namespace

#define OP_ ^
#define TPE_GROUP "test_llvm_set_vector"
#define TPE_NAME "llvm::SetVector"
#define TPE_CTOR_IN(T) llvm::SetVector<T>
#define TPE_CTOR_OUT(T) llvm::SetVector<T>
#define TPE_CTOR_VAR_OUT(T) llvm::SetVector<T>
#define TPE_INIT_TO_CTOR_IN(x) to_set_vector(x)
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
