#pragma once

#include "details/base.hpp"

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp"
#endif

#define ASPARTAME_ITERABLE(T, op, ...)                                                                                                       \
  if constexpr (is_iterable<T>) { return view_##op(__VA_ARGS__); }

namespace aspartame {

template <typename Iterator> class view {
  Iterator begin_, end_;
//  static_assert(is_iterable<Iterator>, "cannot create a view from non-iterator types");
public:
  using value_type = typename Iterator::value_type;
  using const_iterator = Iterator;
  constexpr view() = default;
  view(Iterator begin, Iterator end = {}) : begin_(std::move(begin)), end_(std::move(end)) {
    static_assert(std::is_default_constructible_v<view>);
    static_assert(std::is_copy_constructible_v<view>);
    static_assert(std::is_copy_assignable_v<view>);
    static_assert(std::is_move_constructible_v<view>);
    static_assert(std::is_move_assignable_v<view>);
    static_assert(std::is_destructible_v<view>);
  }
  template <typename C> //
  constexpr explicit view(const C &c) : view(c.begin(), c.end()) {}

  [[nodiscard]] constexpr Iterator begin() const { return begin_; }
  [[nodiscard]] constexpr Iterator end() const { return end_; }
  [[nodiscard]] constexpr bool operator==(const view &rhs) const { return begin_ == rhs.begin_ && end_ == rhs.end_; }
  [[nodiscard]] constexpr bool operator!=(const view &rhs) const { return !rhs.operator==(*this); }
};
template <typename C> view(C) -> view<typename C::const_iterator>;

template <typename Iterable, typename Op, typename = std::enable_if_t<is_iterable<Iterable>>>
auto operator|(const Iterable &l, const Op &r) {
  return r(l);
}
} // namespace aspartame

#define ASPARTAME_PREFIX(name) view_##name

#include "details/iterators.hpp"

#undef ASPARTAME_PREFIX
