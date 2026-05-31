#include "aspartame/expected.hpp"

#include "fixtures.hpp"
#include "test_base_includes.hpp"

#if __has_include(<version>)
  #include <version>
#endif

#if defined(__cpp_lib_expected)

namespace {
template <typename T> std::expected<T, std::string> to_std_expected(std::initializer_list<T> x) {
  return std::empty(x) ? std::expected<T, std::string>{std::unexpect, "error"} : std::expected<T, std::string>{*x.begin()};
}
} // namespace

  #define OP_ ^
  #define TPE_GROUP "test_expected"
  #define TPE_NAME "std::expected"
  #define TPE_CTOR_IN(T) std::expected<T, std::string>
  #define TPE_CTOR_OUT(T) std::expected<T, std::string>
  #define TPE_MAKE_ERR(T)                                                                                                                  \
    TPE_CTOR_OUT(T) { std::unexpect, std::string{"error"} }
  #define TPE_INIT_TO_CTOR_IN(x) to_std_expected(x)
  #define TPE_INIT_SKIP(x) ((x).size() > 1)

  #include "templates/test_template_expected.hpp"

#endif
