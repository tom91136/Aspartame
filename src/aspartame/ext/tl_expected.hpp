#pragma once

#include <utility>

#include <tl/expected.hpp>

#include <aspartame/all.hpp>

namespace aspartame {

template <typename V, typename E> struct expected_traits<tl::expected<V, E>> {
  using value_type = V;
  using error_type = E;
  template <typename U> using rebind_value = tl::expected<U, E>;
  template <typename E2> using rebind_error = tl::expected<V, E2>;
  static bool engaged(const tl::expected<V, E> &e) noexcept { return e.has_value(); }
  static const V &get(const tl::expected<V, E> &e) noexcept { return *e; }
  static const E &error(const tl::expected<V, E> &e) noexcept { return e.error(); }
  template <typename U, typename Arg> static rebind_value<U> make_value(Arg &&v) { return rebind_value<U>{std::forward<Arg>(v)}; }
  template <typename U> static rebind_value<U> propagate_error(const E &e) { return rebind_value<U>{tl::unexpect, e}; }
  template <typename E2, typename Arg> static rebind_error<E2> propagate_value(Arg &&v) { return rebind_error<E2>{std::forward<Arg>(v)}; }
  template <typename E2, typename Arg> static rebind_error<E2> make_error(Arg &&e) {
    return rebind_error<E2>{tl::unexpect, std::forward<Arg>(e)};
  }
};

} // namespace aspartame
