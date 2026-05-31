#include <type_traits>

#include <aspartame/ext/boost.hpp>

#include "test_base_includes.hpp"

static_assert(aspartame::enable_pipe<boost::container::small_vector<int, 4>>::value);
static_assert(aspartame::enable_pipe<boost::container::flat_set<int>>::value);
static_assert(aspartame::enable_pipe<boost::container::flat_map<int, int>>::value);
static_assert(aspartame::enable_pipe<boost::unordered_flat_set<int>>::value);
static_assert(aspartame::enable_pipe<boost::unordered_flat_map<int, int>>::value);
static_assert(aspartame::details::has_optional_traits_v<boost::optional<int>>);
