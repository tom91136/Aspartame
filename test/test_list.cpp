#include "aspartame/list.hpp"

#define TPE_GROUP "test_list"
#define TPE_NAME "std::list"
#define TPE_CTOR_IN(T) std::list<T>
#define TPE_CTOR_OUT(T) std::list<T>
#define TPE_CTOR_VAR_OUT(T) std::list<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_sequence.hpp"
