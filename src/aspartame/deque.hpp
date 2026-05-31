#pragma once

#include <deque>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename T, typename A> struct enable_pipe<std::deque<T, A>> : std::true_type {};

template <typename T, typename A> struct sequence_traits<std::deque<T, A>> {
  using value_type = T;
  template <typename U> using rebind = std::deque<U>;
  static constexpr bool set_like = false;
};

template <typename T, typename A> struct monoid_traits<std::deque<T, A>> {
  static std::deque<T, A> empty() { return {}; }
  static std::deque<T, A> combine(std::deque<T, A> a, const std::deque<T, A> &b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
  }
};

} // namespace aspartame
