#include <aspartame/ext/boost.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace boost::container {
using ::operator<<;
template <typename K, typename V, typename C, typename AC> std::ostream &operator<<(std::ostream &os, const flat_map<K, V, C, AC> &xs) {
  os << "boost::flat_map{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
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
#define TPE_GROUP "test_boost_flat_map"
#define TPE_NAME "boost::flat_map"
#define TPE_CTOR_IN(K, V) boost::container::flat_map<K, V>
#define TPE_CTOR_OUT(K, V) boost::container::flat_map<K, V>
#define TPE_CTOR_VAR_OUT(T) boost::container::flat_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
