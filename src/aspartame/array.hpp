#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename T, size_t N> struct enable_pipe<std::array<T, N>> : std::true_type {};

template <typename T, size_t N> struct sequence_traits<std::array<T, N>> {
  using value_type = T;
  template <typename U> using rebind = std::conditional_t<std::is_same_v<U, T>, std::array<T, N>, std::vector<U>>;
  static constexpr bool set_like = false;
  static constexpr bool fixed_size = true;
};

} // namespace aspartame
