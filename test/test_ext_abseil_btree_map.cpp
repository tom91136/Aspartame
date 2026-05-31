#include <aspartame/ext/abseil.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace absl {
using ::operator<<;
template <class K, class V, class C, class A> std::ostream &operator<<(std::ostream &os, const btree_map<K, V, C, A> &xs) {
  os << "absl::btree_map{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
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
#define TPE_GROUP "test_abseil_btree_map"
#define TPE_NAME "absl::btree_map"
#define TPE_CTOR_IN(K, V) absl::btree_map<K, V>
#define TPE_CTOR_OUT(K, V) absl::btree_map<K, V>
#define TPE_CTOR_VAR_OUT(T) absl::btree_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
