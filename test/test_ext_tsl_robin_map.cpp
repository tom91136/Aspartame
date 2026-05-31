#include <aspartame/ext/tsl.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace tsl {
using ::operator<<;
template <class K, class T, class H, class KE, class A, bool SH, class GP>
std::ostream &operator<<(std::ostream &os, const robin_map<K, T, H, KE, A, SH, GP> &xs) {
  os << "tsl::robin_map{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
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
#define TPE_GROUP "test_tsl_robin_map"
#define TPE_NAME "tsl::robin_map"
#define TPE_CTOR_IN(K, V) tsl::robin_map<K, V>
#define TPE_CTOR_OUT(K, V) tsl::robin_map<K, V>
#define TPE_CTOR_VAR_OUT(T) tsl::robin_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
