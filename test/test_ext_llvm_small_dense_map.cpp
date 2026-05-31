#include "catch2/catch_test_macros.hpp"

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename K, typename V, unsigned InlineBuckets, typename Info, typename Bucket>
std::ostream &operator<<(std::ostream &os, const SmallDenseMap<K, V, InlineBuckets, Info, Bucket> &xs) {
  os << "llvm::SmallDenseMap{";
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
class SmallDenseMap : public llvm::SmallDenseMap<K, V> {
public:
  using base = llvm::SmallDenseMap<K, V>;
  using key_type = K;
  using mapped_type = V;
  SmallDenseMap() = default;
  SmallDenseMap(std::initializer_list<std::pair<K, V>> xs) {
    for (const auto &kv : xs) this->insert(kv);
  }
  SmallDenseMap(const base &b) : base(b) {}
  SmallDenseMap(base &&b) : base(std::move(b)) {}
  friend bool operator==(const SmallDenseMap &a, const SmallDenseMap &b) {
    return static_cast<const base &>(a) == static_cast<const base &>(b);
  }
  friend bool operator!=(const SmallDenseMap &a, const SmallDenseMap &b) { return !(a == b); }
};
} // namespace test_helpers

namespace aspartame {
template <typename K, typename V> struct enable_pipe<test_helpers::SmallDenseMap<K, V>> : std::true_type {};
template <typename K, typename V> struct map_traits<test_helpers::SmallDenseMap<K, V>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = test_helpers::SmallDenseMap<K2, V2>;
  template <typename K2> using key_container = std::vector<K2>;
  template <typename V2> using value_container = std::vector<V2>;
  template <typename V2> using group_inner = std::vector<V2>;
};
} // namespace aspartame

namespace test_helpers {
using ::operator<<;
template <typename K, typename V> std::ostream &operator<<(std::ostream &os, const SmallDenseMap<K, V> &xs) {
  os << "test_helpers::SmallDenseMap{";
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
#define TPE_GROUP "test_llvm_small_dense_map"
#define TPE_NAME "llvm::SmallDenseMap"
#define TPE_CTOR_IN(K, V) test_helpers::SmallDenseMap<K, V>
#define TPE_CTOR_OUT(K, V) test_helpers::SmallDenseMap<K, V>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED
// Hash-order iteration produces non-deterministic inner ordering in group_* outputs.
#define DISABLE_GROUP_BY
#define DISABLE_GROUP_MAP
#define DISABLE_GROUP_MAP_REDUCE

#include "templates/test_template_map.hpp"
