#include "catch2/catch_test_macros.hpp"

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename K, typename V, typename Info, typename Bucket>
std::ostream &operator<<(std::ostream &os, const DenseMap<K, V, Info, Bucket> &xs) {
  os << "llvm::DenseMap{";
  bool first = true;
  for (const auto &kv : xs) {
    if (!first) os << ", ";
    first = false;
    os << "{" << kv.first << ", " << kv.second << "}";
  }
  return os << "}";
}
} // namespace llvm

using namespace aspartame;

TEST_CASE("llvm::DenseMap_map_rebind", "[llvm::DenseMap]") {
  llvm::DenseMap<int, int> xs;
  xs.insert({1, 10});
  xs.insert({2, 20});
  auto ys = xs ^ map([](auto k, auto v) { return std::pair<long, long>{k, v * 2L}; });
  static_assert(std::is_same_v<decltype(ys), llvm::DenseMap<long, long>>);
  REQUIRE(ys.lookup(1L) == 20L);
  REQUIRE(ys.lookup(2L) == 40L);
}

namespace test_helpers {
template <typename K, typename V>
class DenseMap : public llvm::DenseMap<K, V> {
public:
  using base = llvm::DenseMap<K, V>;
  using key_type = K;
  using mapped_type = V;
  DenseMap() = default;
  DenseMap(std::initializer_list<std::pair<K, V>> xs) {
    for (const auto &kv : xs) this->insert(kv);
  }
  DenseMap(const base &b) : base(b) {}
  DenseMap(base &&b) : base(std::move(b)) {}
  friend bool operator==(const DenseMap &a, const DenseMap &b) {
    return static_cast<const base &>(a) == static_cast<const base &>(b);
  }
  friend bool operator!=(const DenseMap &a, const DenseMap &b) { return !(a == b); }
};
} // namespace test_helpers

namespace aspartame {
template <typename K, typename V> struct enable_pipe<test_helpers::DenseMap<K, V>> : std::true_type {};
template <typename K, typename V> struct map_traits<test_helpers::DenseMap<K, V>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = test_helpers::DenseMap<K2, V2>;
  template <typename K2> using key_container = std::vector<K2>;
  template <typename V2> using value_container = std::vector<V2>;
  template <typename V2> using group_inner = std::vector<V2>;
};
} // namespace aspartame

namespace test_helpers {
using ::operator<<;
template <typename K, typename V> std::ostream &operator<<(std::ostream &os, const DenseMap<K, V> &xs) {
  os << "test_helpers::DenseMap{";
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
#define TPE_GROUP "test_llvm_dense_map"
#define TPE_NAME "llvm::DenseMap"
#define TPE_CTOR_IN(K, V) test_helpers::DenseMap<K, V>
#define TPE_CTOR_OUT(K, V) test_helpers::DenseMap<K, V>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED
// DenseMap iterates in hash order, so group_* tests produce inner vectors whose order
// is non-deterministic; the harness can sort the outer container but not the inner ones.
#define DISABLE_GROUP_BY
#define DISABLE_GROUP_MAP
#define DISABLE_GROUP_MAP_REDUCE

#include "templates/test_template_map.hpp"
