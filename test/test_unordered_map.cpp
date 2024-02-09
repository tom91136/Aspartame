#include "test_base_includes.hpp"

#define OP_ ^
#define TPE_GROUP "test_unordered_map"
#define TPE_NAME "std::unordered_map"
#define TPE_CTOR_IN(K, V) std::unordered_map<K, V>
#define TPE_CTOR_OUT(K, V) std::unordered_map<K, V>
#define TPE_CTOR_VAR_OUT(T) std::unordered_set<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

#include "templates/test_template_map.hpp"
