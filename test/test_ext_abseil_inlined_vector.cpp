#include <aspartame/ext/abseil.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace absl {
using ::operator<<;
template <class T, std::size_t N, class A> std::ostream &operator<<(std::ostream &os, const InlinedVector<T, N, A> &xs) {
  os << "absl::InlinedVector{";
  for (std::size_t i = 0; i < xs.size(); ++i) {
    if (i > 0) os << ", ";
    os << xs[i];
  }
  return os << "}";
}
} // namespace absl

#define OP_ ^
#define TPE_GROUP "test_abseil_inlined_vector"
#define TPE_NAME "absl::InlinedVector"
#define TPE_CTOR_IN(T) absl::InlinedVector<T, 4>
#define TPE_CTOR_OUT(T) absl::InlinedVector<T, 4>
#define TPE_CTOR_VAR_OUT(T) absl::InlinedVector<T, 4>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_sequence.hpp"
#define TPE_INPLACE_SEQ 1
#include "templates/test_template_inplace.hpp"
