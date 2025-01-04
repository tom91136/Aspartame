#pragma once

#include "details/base.hpp"
#include "details/map_impl.hpp"
#include "fluent.hpp"

#include <unordered_map>
#include <unordered_set>

namespace aspartame {
namespace details {
template <typename> constexpr bool is_unordered_map_impl = false;
template <typename K, typename V> constexpr bool is_unordered_map_impl<std::unordered_map<K, V>> = true;
} // namespace details
template <typename T> constexpr bool is_unordered_map = details::is_unordered_map_impl<std::decay_t<T>>;
template <typename K, typename V, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::unordered_map<K, V> &, tag>
#endif
auto operator^(const std::unordered_map<K, V> &l, const Op &r) {
  return r(l, tag{});
}
template <typename K, typename V, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, std::unordered_map<K, V> &, tag>
#endif
auto operator^=(std::unordered_map<K, V> &l, const Op &r) {
  return r(l, tag{});
}

} // namespace aspartame

#define ASPARTAME_IN_TYPE2(K, V) std::unordered_map<K, V>
#define ASPARTAME_IN_TYPE1(C) std::unordered_map<C, C>
#define ASPARTAME_OUT_TYPE std::unordered_map
#define ASPARTAME_OUT_TYPE_C1 std::unordered_set
#define ASPARTAME_SET_LIKE true

#include "details/container2_template.hpp"
#include "details/map_template.hpp"

#include "details/nop/optional_template.hpp"
#include "details/nop/sequence_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_IN_TYPE2
#undef ASPARTAME_IN_TYPE1
#undef ASPARTAME_OUT_TYPE_C1
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_SET_LIKE
