#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename T, //
          typename Function>
class iterate_iterator : public fwd_iterator<iterate_iterator<T, Function>, T> {
  ca_optional<Function> f;
  std::optional<T> current;
  [[nodiscard]] constexpr bool has_next() const { return static_cast<bool>(f); }

public:
  constexpr iterate_iterator() = default;
  constexpr iterate_iterator(T init, Function next) : f(next), current(init) {
    static_assert(std::is_same_v<T, decltype((*f)(*current))>, "return type of iterate function does not match");
  }
  constexpr iterate_iterator &operator++() {
    current = (*f)(*current);
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *current; }
  [[nodiscard]] constexpr bool operator==(const iterate_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
