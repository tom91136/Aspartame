#include <aspartame/ext/abseil.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace absl {
using ::operator<<;
template <class K, class C, class A> std::ostream &operator<<(std::ostream &os, const btree_set<K, C, A> &xs) {
  os << "absl::btree_set{";
  bool first = true;
  for (const auto &x : xs) {
    if (!first) os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
} // namespace absl

#define OP_ ^
#define TPE_GROUP "test_abseil_btree_set"
#define TPE_NAME "absl::btree_set"
#define TPE_CTOR_IN(T) absl::btree_set<T>
#define TPE_CTOR_OUT(T) absl::btree_set<T>
#define TPE_CTOR_VAR_OUT(T) absl::btree_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED
// btree_set iterators don't survive concurrent erase; the inplace_filter helper relies on it.
#define DISABLE_INPLACE_FILTER

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_inplace.hpp"
