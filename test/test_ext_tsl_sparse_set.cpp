#include <aspartame/ext/tsl.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace tsl {
using ::operator<<;
template <class K, class H, class KE, class A, class GP, sh::exception_safety ES, sh::sparsity SP>
std::ostream &operator<<(std::ostream &os, const sparse_set<K, H, KE, A, GP, ES, SP> &xs) {
  os << "tsl::sparse_set{";
  bool first = true;
  for (const auto &x : xs) {
    if (!first) os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
} // namespace tsl

#define OP_ ^
#define TPE_GROUP "test_tsl_sparse_set"
#define TPE_NAME "tsl::sparse_set"
#define TPE_CTOR_IN(T) tsl::sparse_set<T>
#define TPE_CTOR_OUT(T) tsl::sparse_set<T>
#define TPE_CTOR_VAR_OUT(T) tsl::sparse_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED

// No std::hash for tsl::sparse_set<T>, so nested-set ops can't compile.
#define DISABLE_FLATTEN
#define DISABLE_TAP

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_inplace.hpp"
