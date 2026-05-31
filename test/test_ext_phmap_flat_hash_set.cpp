#include <aspartame/ext/phmap.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace phmap {
using ::operator<<;
template <class T, class H, class E, class A> std::ostream &operator<<(std::ostream &os, const flat_hash_set<T, H, E, A> &xs) {
  os << "phmap::flat_hash_set{";
  bool first = true;
  for (const auto &x : xs) {
    if (!first) os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
} // namespace phmap

#define OP_ ^
#define TPE_GROUP "test_phmap_flat_hash_set"
#define TPE_NAME "phmap::flat_hash_set"
#define TPE_CTOR_IN(T) phmap::flat_hash_set<T>
#define TPE_CTOR_OUT(T) phmap::flat_hash_set<T>
#define TPE_CTOR_VAR_OUT(T) phmap::flat_hash_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED

// No std::hash for phmap::flat_hash_set<T>, so nested-set ops can't compile.
#define DISABLE_FLATTEN
#define DISABLE_TAP

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_inplace.hpp"
