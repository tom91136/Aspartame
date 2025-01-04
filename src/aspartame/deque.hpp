#pragma once

#include "details/base.hpp"
#include "fluent.hpp"

#include <deque>

namespace aspartame {
namespace details {
template <typename> constexpr bool is_deque_impl = false;
template <typename T> constexpr bool is_deque_impl<std::deque<T>> = true;
} // namespace details

template <typename T> constexpr bool is_deque = details::is_deque_impl<std::decay_t<T>>;
template <typename T, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::deque<T> &, tag>
#endif
auto operator^(const std::deque<T> &l, const Op &r) {
  return r(l, tag{});
}
} // namespace aspartame

#define ASPARTAME_IN_TYPE2(K, V) std::deque<std::pair<K, V>>
#define ASPARTAME_IN_TYPE1(C) std::deque<C>
#define ASPARTAME_OUT_TYPE std::deque
#define ASPARTAME_SET_LIKE false

#include "details/container1_template.hpp"
#include "details/sequence1_template.hpp"

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_SET_LIKE
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_IN_TYPE1
#undef ASPARTAME_IN_TYPE2
