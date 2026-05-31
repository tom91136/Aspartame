#include <boost/optional/optional_io.hpp>

#include <aspartame/ext/boost.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace {
template <typename T> boost::optional<T> to_boost_opt(std::initializer_list<T> x) {
  return std::empty(x) ? boost::optional<T>{} : boost::optional<T>{*x.begin()};
}
} // namespace

#define OP_ ^
#define TPE_GROUP "test_boost_optional"
#define TPE_NAME "boost::optional"
#define TPE_CTOR_IN(T) boost::optional<T>
#define TPE_CTOR_OUT(T) boost::optional<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) to_boost_opt(x)
#define TPE_INIT_SKIP(x) ((x).size() > 1)

// boost::optional's value ctor is unconditionally explicit, so copy-list-init
// from the std-shaped templates' brace expecteds won't compile.
#include "templates/test_template_optional.hpp"
