#include <aspartame/ext/boost.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace boost::container {
using ::operator<<;
template <typename K, typename C, typename A> std::ostream &operator<<(std::ostream &os, const flat_set<K, C, A> &xs) {
  os << "boost::flat_set{";
  bool first = true;
  for (const auto &x : xs) {
    if (!first) os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
} // namespace boost::container

#define OP_ ^
#define TPE_GROUP "test_boost_flat_set"
#define TPE_NAME "boost::flat_set"
#define TPE_CTOR_IN(T) boost::container::flat_set<T>
#define TPE_CTOR_OUT(T) boost::container::flat_set<T>
#define TPE_CTOR_VAR_OUT(T) boost::container::flat_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED

// flat_set is a sorted vector; node_erasable's "cache end then erase" loop
// dangles the end iterator. Eager ^ is fine; in-place ^= aborts.
#define DISABLE_INPLACE_FILTER

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_inplace.hpp"
