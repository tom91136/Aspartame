#pragma once

#include <unordered_set>

#include "details/base.hpp"

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp"
#endif

#define ASPARTAME_UNORDERED_SET(T, op, ...)                                                                                                \
  if constexpr (is_unordered_set<T>) { return unordered_set_##op(__VA_ARGS__); }

namespace aspartame {
namespace details {
template <typename> constexpr bool is_unordered_set_impl = false;
template <typename T> constexpr bool is_unordered_set_impl<std::unordered_set<T>> = true;
} // namespace details
template <typename T> constexpr bool is_unordered_set = details::is_unordered_set_impl<std::decay_t<T>>;
template <typename T, typename Op> auto operator|(const std::unordered_set<T> &l, const Op &r) { return r(l); }
} // namespace aspartame

#define ASPARTAME_PREFIX(name) unordered_set_##name
#define ASPARTAME_OUT_TYPE std::unordered_set
#define ASPARTAME_SET_LIKE true

#include "details/container1_template.hpp"

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"
#include "details/nop/sequence_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_PREFIX
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_SET_LIKE