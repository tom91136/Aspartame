#include <aspartame/ext/tl_expected.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace tl {
using ::operator<<;
template <typename V, typename E> std::ostream &operator<<(std::ostream &os, const tl::expected<V, E> &e) {
  if (e.has_value()) return os << "tl::expected{" << *e << "}";
  return os << "tl::unexpect{" << e.error() << "}";
}
} // namespace tl

namespace {
template <typename T> tl::expected<T, std::string> to_tl_expected(std::initializer_list<T> x) {
  return std::empty(x) ? tl::expected<T, std::string>{tl::unexpect, "error"} : tl::expected<T, std::string>{*x.begin()};
}
} // namespace

#define OP_ ^
#define TPE_GROUP "test_tl_expected"
#define TPE_NAME "tl::expected"
#define TPE_CTOR_IN(T) tl::expected<T, std::string>
#define TPE_CTOR_OUT(T) tl::expected<T, std::string>
#define TPE_MAKE_ERR(T)                                                                                                                    \
  TPE_CTOR_OUT(T) { tl::unexpect, std::string{"error"} }
#define TPE_INIT_TO_CTOR_IN(x) to_tl_expected(x)
#define TPE_INIT_SKIP(x) ((x).size() > 1)

#include "templates/test_template_expected.hpp"
