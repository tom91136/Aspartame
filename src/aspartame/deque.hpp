#pragma once

#include <deque>

#include "details/base.hpp"

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp"
#endif

#define ASPARTAME_DEQUE(T, op, ...)                                                                                                        \
  if constexpr (is_deque<T>) { return deque_##op(__VA_ARGS__); }

namespace aspartame {
namespace details {
template <typename> constexpr bool is_deque_impl = false;
template <typename T> constexpr bool is_deque_impl<std::deque<T>> = true;
} // namespace details

template <typename T> constexpr bool is_deque = details::is_deque_impl<std::decay_t<T>>;
template <typename T, typename Op> auto operator^(const std::deque<T> &l, const Op &r) { return r(l); }
} // namespace aspartame

#define ASPARTAME_PREFIX(name) deque_##name
#define ASPARTAME_OUT_TYPE std::deque
#define ASPARTAME_SET_LIKE false

#include "details/container1_template.hpp"
#include "details/sequence1_template.hpp"

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_PREFIX
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_SET_LIKE
