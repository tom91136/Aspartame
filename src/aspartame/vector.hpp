#pragma once

#include <vector>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename T, typename A> struct enable_pipe<std::vector<T, A>> : std::true_type {};

template <typename T, typename A> struct sequence_traits<std::vector<T, A>> {
  using value_type = T;
  template <typename U> using rebind = std::vector<U>;
  static constexpr bool set_like = false;
};

template <typename T, typename A> struct monoid_traits<std::vector<T, A>> {
  static std::vector<T, A> empty() { return {}; }
  static std::vector<T, A> combine(std::vector<T, A> a, const std::vector<T, A> &b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
  }
};

} // namespace aspartame
