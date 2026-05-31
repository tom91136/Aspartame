#pragma once

#include <set>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename T, typename C, typename A> struct enable_pipe<std::set<T, C, A>> : std::true_type {};

template <typename T, typename C, typename A> struct sequence_traits<std::set<T, C, A>> {
  using value_type = T;
  template <typename U> using rebind = std::set<U>;
  static constexpr bool set_like = true;
};

template <typename T, typename C, typename A> struct monoid_traits<std::set<T, C, A>> {
  static std::set<T, C, A> empty() { return {}; }
  static std::set<T, C, A> combine(std::set<T, C, A> a, const std::set<T, C, A> &b) {
    a.insert(b.begin(), b.end());
    return a;
  }
};

} // namespace aspartame
