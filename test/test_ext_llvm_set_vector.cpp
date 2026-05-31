#include "catch2/catch_test_macros.hpp"

#include <aspartame/ext/llvm.hpp>

#include "ext/llvm_dense_info.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace test_helpers {
template <typename T>
class SetVector : public llvm::SetVector<T> {
public:
  using base = llvm::SetVector<T>;
  using base::base;
  SetVector() = default;
  SetVector(std::initializer_list<T> xs) {
    for (const auto &x : xs) this->insert(x);
  }
  SetVector(const base &b) : base(b) {}
  SetVector(base &&b) : base(std::move(b)) {}
  friend bool operator==(const SetVector &a, const SetVector &b) {
    return static_cast<const base &>(a) == static_cast<const base &>(b);
  }
  friend bool operator!=(const SetVector &a, const SetVector &b) { return !(a == b); }
};
} // namespace test_helpers

namespace aspartame {
template <typename T> struct enable_pipe<test_helpers::SetVector<T>> : std::true_type {};
template <typename T> struct sequence_traits<test_helpers::SetVector<T>> {
  using value_type = T;
  template <typename U> using rebind = test_helpers::SetVector<U>;
  static constexpr bool set_like = true;
};
} // namespace aspartame

namespace test_helpers {
using ::operator<<;
template <typename T> std::ostream &operator<<(std::ostream &os, const SetVector<T> &xs) {
  os << "test_helpers::SetVector{";
  bool first = true;
  for (const auto &x : xs) {
    if (!first) os << ", ";
    first = false;
    os << x;
  }
  return os << "}";
}
} // namespace test_helpers

#define OP_ ^
#define TPE_GROUP "test_llvm_set_vector"
#define TPE_NAME "llvm::SetVector"
#define TPE_CTOR_IN(T) test_helpers::SetVector<T>
#define TPE_CTOR_OUT(T) test_helpers::SetVector<T>
#define TPE_CTOR_VAR_OUT(T) test_helpers::SetVector<T>
#define TPE_INIT_TO_CTOR_IN(x) test_helpers::SetVector<typename std::decay_t<decltype(x)>::value_type>(x)
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

// SetVector wraps DenseSet, so SetVector<SetVector<T>> would need DenseMapInfo<SetVector<T>>.
#define DISABLE_FLATTEN
// SetVector<std::optional<T>> would need DenseMapInfo<std::optional<T>> which only
// exists on LLVM 22+; the older LLVM headers on Ubuntu 24.04 ship llvm-18.
#define DISABLE_SEQUENCE
#define DISABLE_SEQUENCE_NESTED

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
