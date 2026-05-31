#include <aspartame/ext/qt.hpp>

#include "ext/qt_print.hpp"
#include "fixtures.hpp"
#include "test_base_includes.hpp"

#define OP_ ^
#define TPE_GROUP "test_qt_qmap"
#define TPE_NAME "QMap"
#define TPE_CTOR_IN(K, V) QMap<K, V>
#define TPE_CTOR_OUT(K, V) QMap<K, V>
#define TPE_CTOR_VAR_OUT(T) QList<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_UNORDERED

// QMap yields key+value pairs via constKeyValueBegin/End and has no nested-set
// hashing, no order-preserving append/concat, and no flatten/group on entries.
#define DISABLE_FLAT_MAP
#define DISABLE_TAP_EACH
#define DISABLE_FOR_EACH
#define DISABLE_FLATTEN
#define DISABLE_APPEND
#define DISABLE_CONCAT
#define DISABLE_GROUP_MAP
#define DISABLE_GROUP_BY

#include "templates/test_template_map.hpp"
