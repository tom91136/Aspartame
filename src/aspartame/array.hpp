#pragma once

#include <array>

#include "details/base.hpp"

#define ASPARTAME_ARRAY(T, op, ...)                                                                                                        \
  if constexpr (is_array<T>) { return array_##op(__VA_ARGS__); }

namespace aspartame {
namespace details {
template <typename> constexpr bool is_array_impl = false;
template <typename T, size_t N> constexpr bool is_array_impl<std::array<T, N>> = true;
} // namespace details
template <typename T> constexpr bool is_array = details::is_array_impl<std::decay_t<T>>;
template <typename T, size_t N, typename Op> auto operator^(const std::array<T, N> &l, const Op &r) { return r(l); }
} // namespace aspartame

#define ASPARTAME_OUT_TYPE In
#define ASPARTAME_SET_LIKE false

// #include "details/star1/container_template.hpp"
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_SET_LIKE