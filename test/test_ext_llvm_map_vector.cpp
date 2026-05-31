#include "catch2/catch_test_macros.hpp"
#include <llvm/ADT/MapVector.h>

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename K, typename V, typename M, typename Vec> std::ostream &operator<<(std::ostream &os, const MapVector<K, V, M, Vec> &xs) {
  os << "llvm::MapVector{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
} // namespace llvm

namespace test_helpers {
template <typename K, typename V>
class MapVector : public llvm::MapVector<K, V> {
public:
  using base = llvm::MapVector<K, V>;
  using base::base;
  using key_type = K;
  using mapped_type = V;
  MapVector() = default;
  MapVector(std::initializer_list<std::pair<K, V>> xs) {
    for (const auto &kv : xs) this->insert(kv);
  }
  MapVector(const base &b) : base(b) {}
  MapVector(base &&b) : base(std::move(b)) {}
  friend bool operator==(const MapVector &a, const MapVector &b) {
    if (a.size() != b.size()) return false;
    for (const auto &kv : a) {
      auto it = b.find(kv.first);
      if (it == b.end() || it->second != kv.second) return false;
    }
    return true;
  }
  friend bool operator!=(const MapVector &a, const MapVector &b) { return !(a == b); }
};
} // namespace test_helpers

namespace aspartame {
template <typename K, typename V> struct enable_pipe<test_helpers::MapVector<K, V>> : std::true_type {};
template <typename K, typename V> struct map_traits<test_helpers::MapVector<K, V>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = test_helpers::MapVector<K2, V2>;
  template <typename K2> using key_container = std::vector<K2>;
  template <typename V2> using value_container = std::vector<V2>;
  template <typename V2> using group_inner = std::vector<V2>;
};
template <typename K, typename V> struct map_access<test_helpers::MapVector<K, V>> {
  static const test_helpers::MapVector<K, V> &entries(const test_helpers::MapVector<K, V> &m) { return m; }
  template <typename Kx, typename Vx> static void put(test_helpers::MapVector<K, V> &out, Kx &&k, Vx &&v) {
    out.insert({std::forward<Kx>(k), std::forward<Vx>(v)});
  }
  template <typename Key> static std::optional<V> lookup(const test_helpers::MapVector<K, V> &m, const Key &key) {
    auto it = m.find(key);
    return it != m.end() ? std::optional<V>{it->second} : std::optional<V>{};
  }
};
} // namespace aspartame

namespace test_helpers {
using ::operator<<;
template <typename K, typename V> std::ostream &operator<<(std::ostream &os, const MapVector<K, V> &xs) {
  os << "test_helpers::MapVector{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
} // namespace test_helpers

#define OP_ ^
#define TPE_GROUP "test_llvm_map_vector"
#define TPE_NAME "llvm::MapVector"
#define TPE_CTOR_IN(K, V) test_helpers::MapVector<K, V>
#define TPE_CTOR_OUT(K, V) test_helpers::MapVector<K, V>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) x
// MapVector preserves insertion order; the harness's _to_vector expects sorted output
// (matching std::map). Mark unordered so the sort-compare path is used.
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
