#pragma once

#include "details/base.hpp"
#include "fluent.hpp"

#include <unordered_set>

namespace aspartame {
namespace details {
template <typename> constexpr bool is_unordered_set_impl = false;
template <typename T> constexpr bool is_unordered_set_impl<std::unordered_set<T>> = true;
} // namespace details
template <typename T> constexpr bool is_unordered_set = details::is_unordered_set_impl<std::decay_t<T>>;
template <typename T, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::unordered_set<T> &, tag>
#endif
auto operator^(const std::unordered_set<T> &l, const Op &r) {
  return r(l, tag{});
}
} // namespace aspartame

#define ASPARTAME_IN_TYPE2(K, V) std::unordered_set<std::pair<K, V>>
#define ASPARTAME_IN_TYPE1(C) std::unordered_set<C>
#define ASPARTAME_OUT_TYPE std::unordered_set
#define ASPARTAME_SET_LIKE true

#include "details/container1_template.hpp"

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"
#include "details/nop/sequence_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_SET_LIKE
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_IN_TYPE1
#undef ASPARTAME_IN_TYPE2
