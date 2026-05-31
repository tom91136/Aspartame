#include <type_traits>

#include <aspartame/ext/qt.hpp>

#include "test_base_includes.hpp"

static_assert(aspartame::enable_pipe<QList<int>>::value);
static_assert(aspartame::enable_pipe<QMap<int, int>>::value);
static_assert(aspartame::enable_pipe<QHash<int, int>>::value);
