#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename T, typename Function>
class unfold_iterator : public fwd_iterator<unfold_iterator<T, Function>, T, std::input_iterator_tag> {
  ca_optional<Function> next;
  std::optional<T> current;
  [[nodiscard]] constexpr bool has_next() const { return next && current; }

public:
  constexpr unfold_iterator() = default;
  constexpr unfold_iterator(T init, Function function) : next(std::move(function)), current(std::move(init)) {
    using R = std::decay_t<decltype((*next)(*current))>;
    static_assert(is_optional<R>, "unfold function must return std::optional<T>");
    static_assert(std::is_same_v<T, typename R::value_type>, "unfold successor type must match its initial value");
  }
  constexpr unfold_iterator &operator++() {
    current = (*next)(*current);
    if (!current) next.reset();
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *current; }
  [[nodiscard]] constexpr bool operator==(const unfold_iterator &that) const { return (!has_next() == !that.has_next()); }
};

} // namespace aspartame::details
