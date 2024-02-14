#define OP_ | // NOLINT(*-macro-parentheses)
#define TPE_GROUP "test_view+filter"
#define TPE_NAME "view+filter"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_MANY_INIT 1
#define TPE_RUN_TEST

#define TPE_TEST_CHAIN(xs) ((xs) | ::aspartame::filter([](auto) { return true; }))

#include "templates/test_template_view.hpp"
