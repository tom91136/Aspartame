#include <type_traits>

#include <aspartame/ext/phmap.hpp>

#include "test_base_includes.hpp"

static_assert(aspartame::enable_pipe<phmap::flat_hash_set<int>>::value);
static_assert(aspartame::enable_pipe<phmap::flat_hash_map<int, int>>::value);
