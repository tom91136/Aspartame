#pragma once

#include "details/base.hpp"
#include "fluent.hpp"

#include <set>

namespace aspartame {
namespace details {
template <typename> constexpr bool is_set_impl = false;
template <typename T> constexpr bool is_set_impl<std::set<T>> = true;
} // namespace details
template <typename T> constexpr bool is_set = details::is_set_impl<std::decay_t<T>>;
template <typename T, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::set<T> &, tag>
#endif
auto operator^(const std::set<T> &l, const Op &r) {
  return r(l, tag{});
}
} // namespace aspartame

#define ASPARTAME_IN_TYPE2(K, V) std::set<std::pair<K, V>>
#define ASPARTAME_IN_TYPE1(C) std::set<C>
#define ASPARTAME_OUT_TYPE std::set
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
