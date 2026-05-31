#include <type_traits>

#include <aspartame/ext/tsl.hpp>

#include "test_base_includes.hpp"

static_assert(aspartame::enable_pipe<tsl::robin_set<int>>::value);
static_assert(aspartame::enable_pipe<tsl::robin_map<int, int>>::value);
static_assert(aspartame::enable_pipe<tsl::sparse_set<int>>::value);
static_assert(aspartame::enable_pipe<tsl::sparse_map<int, int>>::value);
