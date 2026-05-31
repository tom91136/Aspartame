#include <aspartame/ext/tsl.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace tsl {
using ::operator<<;
template <class K, class T, class H, class KE, class A, class GP, sh::exception_safety ES, sh::sparsity SP>
std::ostream &operator<<(std::ostream &os, const sparse_map<K, T, H, KE, A, GP, ES, SP> &xs) {
  os << "tsl::sparse_map{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
template <class K, class H, class KE, class A, class GP, sh::exception_safety ES, sh::sparsity SP>
std::ostream &operator<<(std::ostream &os, const sparse_set<K, H, KE, A, GP, ES, SP> &xs) {
  os << "tsl::sparse_set{";
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
#define TPE_GROUP "test_tsl_sparse_map"
#define TPE_NAME "tsl::sparse_map"
#define TPE_CTOR_IN(K, V) tsl::sparse_map<K, V>
#define TPE_CTOR_OUT(K, V) tsl::sparse_map<K, V>
#define TPE_CTOR_VAR_OUT(T) tsl::sparse_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
