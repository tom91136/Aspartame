#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Function,      //
          typename T = typename InputIterator::value_type>
class tap_each_iterator : public fwd_iterator<tap_each_iterator<InputIterator, Function, T>, T> {
  InputIterator it, end;
  ca_optional<Function> f;
  [[nodiscard]] constexpr bool has_next() const { return it != end; }

public:
  constexpr tap_each_iterator() = default;
  constexpr tap_each_iterator(InputIterator begin, InputIterator end, Function function)
      : it(std::move(begin)), end(std::move(end)), f(function) {}

  constexpr tap_each_iterator &operator++() {
    ++it;
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() {
    auto &&x = *it;
    (*f)(x);
    return x;
  }
  [[nodiscard]] constexpr bool operator==(const tap_each_iterator &that) const {
    return (it == that.it) || (!this->has_next() == !that.has_next());
  }
};

} // namespace aspartame::details
