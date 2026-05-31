#include <aspartame/ext/tsl.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace tsl {
using ::operator<<;
template <class K, class H, class KE, class A, bool SH, class GP>
std::ostream &operator<<(std::ostream &os, const robin_set<K, H, KE, A, SH, GP> &xs) {
  os << "tsl::robin_set{";
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
#define TPE_GROUP "test_tsl_robin_set"
#define TPE_NAME "tsl::robin_set"
#define TPE_CTOR_IN(T) tsl::robin_set<T>
#define TPE_CTOR_OUT(T) tsl::robin_set<T>
#define TPE_CTOR_VAR_OUT(T) tsl::robin_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED

// No std::hash for tsl::robin_set<T>, so nested-set ops can't compile.
#define DISABLE_FLATTEN
#define DISABLE_TAP

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_inplace.hpp"
