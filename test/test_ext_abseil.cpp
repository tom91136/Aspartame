#include <type_traits>

#include <aspartame/ext/abseil.hpp>

#include "test_base_includes.hpp"

static_assert(aspartame::enable_pipe<absl::InlinedVector<int, 4>>::value);
static_assert(aspartame::enable_pipe<absl::flat_hash_set<int>>::value);
static_assert(aspartame::enable_pipe<absl::btree_set<int>>::value);
static_assert(aspartame::enable_pipe<absl::flat_hash_map<int, int>>::value);
static_assert(aspartame::enable_pipe<absl::btree_map<int, int>>::value);
