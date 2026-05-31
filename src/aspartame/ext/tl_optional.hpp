#pragma once

#include <tl/optional.hpp>

#include <aspartame/all.hpp>

namespace aspartame {

template <typename T> struct optional_traits<tl::optional<T>> {
  using value_type = T;
  template <typename U> using rebind = tl::optional<U>;
  static bool engaged(const tl::optional<T> &o) noexcept { return o.has_value(); }
  static const T &get(const tl::optional<T> &o) noexcept { return *o; }
};

} // namespace aspartame
