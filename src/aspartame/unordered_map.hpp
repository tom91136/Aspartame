#pragma once

#include <unordered_map>
#include <unordered_set>

#include "details/base.hpp"
#include "details/map_impl.hpp"

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp"
#endif

#define ASPARTAME_UNORDERED_MAP(T, op, ...)                                                                                                \
  if constexpr (is_unordered_map<T>) { return unordered_map_##op(__VA_ARGS__); }

namespace aspartame {
namespace details {
template <typename> constexpr bool is_unordered_map_impl = false;
template <typename K, typename V> constexpr bool is_unordered_map_impl<std::unordered_map<K, V>> = true;
} // namespace details
template <typename T> constexpr bool is_unordered_map = details::is_unordered_map_impl<std::decay_t<T>>;
template <typename K, typename V, typename Op> auto operator|(const std::unordered_map<K, V> &l, const Op &r) { return r(l); }
} // namespace aspartame

#define ASPARTAME_PREFIX(name) unordered_map_##name
#define ASPARTAME_OUT_TYPE std::unordered_map
#define ASPARTAME_OUT_TYPE_C1 std::unordered_set
#define ASPARTAME_SET_LIKE true

#include "details/container2_template.hpp"
#include "details/map_template.hpp"

#include "details/nop/optional_template.hpp"
#include "details/nop/sequence_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_PREFIX
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_SET_LIKE
