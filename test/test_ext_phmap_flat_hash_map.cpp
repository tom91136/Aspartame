#include <aspartame/ext/phmap.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace phmap {
using ::operator<<;
template <class K, class V, class H, class E, class A> std::ostream &operator<<(std::ostream &os, const flat_hash_map<K, V, H, E, A> &xs) {
  os << "phmap::flat_hash_map{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
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
#define TPE_GROUP "test_phmap_flat_hash_map"
#define TPE_NAME "phmap::flat_hash_map"
#define TPE_CTOR_IN(K, V) phmap::flat_hash_map<K, V>
#define TPE_CTOR_OUT(K, V) phmap::flat_hash_map<K, V>
#define TPE_CTOR_VAR_OUT(T) phmap::flat_hash_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
