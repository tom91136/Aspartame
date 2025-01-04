#define OP_ | // NOLINT(*-macro-parentheses)
#define TPE_GROUP "test_view+bind"
#define TPE_NAME "view+bind"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_MANY_INIT 1
#define TPE_RUN_TEST

#define TPE_TEST_CHAIN(xs) ((xs) | ::aspartame::flat_map([](auto x) { return TPE_CTOR_IN(decltype(x)){x}; }))

#include "templates/test_template_view.hpp"
