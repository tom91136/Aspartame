#include <aspartame/ext/boost.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace boost {
using ::operator<<;
template <typename K, typename V, typename H, typename E, typename A>
std::ostream &operator<<(std::ostream &os, const unordered_flat_map<K, V, H, E, A> &xs) {
  os << "boost::unordered_flat_map{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
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
#define TPE_GROUP "test_boost_unordered_flat_map"
#define TPE_NAME "boost::unordered_flat_map"
#define TPE_CTOR_IN(K, V) boost::unordered_flat_map<K, V>
#define TPE_CTOR_OUT(K, V) boost::unordered_flat_map<K, V>
#define TPE_CTOR_VAR_OUT(T) boost::unordered_flat_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
