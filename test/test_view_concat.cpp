#define OP_ | // NOLINT(*-macro-parentheses)
#define TPE_GROUP "test_view+concat"
#define TPE_NAME "view+concat"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_MANY_INIT 1
#define TPE_RUN_TEST

#define TPE_TEST_CHAIN(xs) ((xs) | ::aspartame::concat(TPE_CTOR_IN(typename std::decay_t<decltype((xs))>::value_type){}))

#include "templates/test_template_view.hpp"
