#include <aspartame/ext/boost.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace boost {
using ::operator<<;
template <typename K, typename H, typename E, typename A>
std::ostream &operator<<(std::ostream &os, const unordered_flat_set<K, H, E, A> &xs) {
  os << "boost::unordered_flat_set{";
  bool first = true;
  for (const auto &x : xs) {
    if (!first) os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
} // namespace boost

#define OP_ ^
#define TPE_GROUP "test_boost_unordered_flat_set"
#define TPE_NAME "boost::unordered_flat_set"
#define TPE_CTOR_IN(T) boost::unordered_flat_set<T>
#define TPE_CTOR_OUT(T) boost::unordered_flat_set<T>
#define TPE_CTOR_VAR_OUT(T) boost::unordered_flat_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_inplace.hpp"
