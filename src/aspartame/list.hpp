#pragma once

#include <list>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename T, typename A> struct enable_pipe<std::list<T, A>> : std::true_type {};

template <typename T, typename A> struct sequence_traits<std::list<T, A>> {
  using value_type = T;
  template <typename U> using rebind = std::list<U>;
  static constexpr bool set_like = false;
};

template <typename T, typename A> struct monoid_traits<std::list<T, A>> {
  static std::list<T, A> empty() { return {}; }
  static std::list<T, A> combine(std::list<T, A> a, std::list<T, A> b) {
    a.splice(a.end(), b);
    return a;
  }
};

} // namespace aspartame
