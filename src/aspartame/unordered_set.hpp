#pragma once

#include <unordered_set>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename T, typename H, typename E, typename A> struct enable_pipe<std::unordered_set<T, H, E, A>> : std::true_type {};

template <typename T, typename H, typename E, typename A> struct sequence_traits<std::unordered_set<T, H, E, A>> {
  using value_type = T;
  template <typename U> using rebind = std::unordered_set<U>;
  static constexpr bool set_like = true;
};

template <typename T, typename H, typename E, typename A> struct monoid_traits<std::unordered_set<T, H, E, A>> {
  static std::unordered_set<T, H, E, A> empty() { return {}; }
  static std::unordered_set<T, H, E, A> combine(std::unordered_set<T, H, E, A> a, const std::unordered_set<T, H, E, A> &b) {
    a.insert(b.begin(), b.end());
    return a;
  }
};

} // namespace aspartame
