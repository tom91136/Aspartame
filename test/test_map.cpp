#include "aspartame/map.hpp"

#define OP_ ^
#define TPE_GROUP "test_map"
#define TPE_NAME "std::map"
#define TPE_CTOR_IN(K, V) std::map<K, V>
#define TPE_CTOR_OUT(K, V) std::map<K, V>
#define TPE_CTOR_VAR_OUT(T) std::set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"

