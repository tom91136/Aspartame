#pragma once

#include "details/base.hpp"
#include "details/map_impl.hpp"
#include "fluent.hpp"

#include <map>
#include <set>

namespace aspartame {
namespace details {
template <typename> constexpr bool is_map_impl = false;
template <typename K, typename V> constexpr bool is_map_impl<std::map<K, V>> = true;
} // namespace details
template <typename T> constexpr bool is_map = details::is_map_impl<std::decay_t<T>>;
template <typename K, typename V, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::map<K, V> &, tag>
#endif
auto operator^(const std::map<K, V> &l, const Op &r) {
  return r(l, tag{});
}
template <typename K, typename V, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, std::map<K, V> &, tag>
#endif
auto operator^=(std::map<K, V> &l, const Op &r) {
  return r(l, tag{});
}

} // namespace aspartame

#define ASPARTAME_IN_TYPE2(K, V) std::map<K, V>
#define ASPARTAME_IN_TYPE1(C) std::map<C, C>
#define ASPARTAME_OUT_TYPE std::map
#define ASPARTAME_OUT_TYPE_C1 std::set
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
