#pragma once

#include "details/base.hpp"
#include "fluent.hpp"

#include <list>

namespace aspartame {
namespace details {
template <typename> constexpr bool is_list_impl = false;
template <typename T> constexpr bool is_list_impl<std::list<T>> = true;
} // namespace details

template <typename T> constexpr bool is_list = details::is_list_impl<std::decay_t<T>>;
template <typename T, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::list<T> &, tag>
#endif
auto operator^(const std::list<T> &l, const Op &r) {
  return r(l, tag{});
}
} // namespace aspartame

#define ASPARTAME_IN_TYPE2(K, V) std::list<std::pair<K, V>>
#define ASPARTAME_IN_TYPE1(C) std::list<C>
#define ASPARTAME_OUT_TYPE std::list
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
