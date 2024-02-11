#include "test_base_includes.hpp"

#define OP_ ^
#define TPE_GROUP "test_unordered_set"
#define TPE_NAME "std::unordered_set"
#define TPE_CTOR_IN(T) std::unordered_set<T>
#define TPE_CTOR_OUT(T) std::unordered_set<T>
#define TPE_CTOR_VAR_OUT(T) std::unordered_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1
#define TPE_UNORDERED

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"