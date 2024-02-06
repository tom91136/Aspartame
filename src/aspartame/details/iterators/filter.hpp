#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Predicate,     //
          typename T = std::add_const_t<typename InputIterator::value_type>>
class filter_iterator : public fwd_iterator<filter_iterator<InputIterator, Predicate>, T> {
  ca_optional<Predicate> p;
  InputIterator it, end;
  [[nodiscard]] constexpr bool has_next() const { return p && it != end; }

public:
  constexpr filter_iterator() = default;
  constexpr filter_iterator(InputIterator current, InputIterator end_, Predicate predicate)
      : p(predicate), it(std::move(current)), end(std::move(end_)) {
    while (it != end && !(*p)(*it))
      ++it;
  }
  constexpr filter_iterator &operator++() {
    do {
      ++it;
    } while (it != end && !(*p)(*it));
    return *this;
  }
  [[nodiscard]] constexpr T &operator*() const { return *it; }
  [[nodiscard]] constexpr bool operator==(const filter_iterator &that) const {
    return (it == that.it) || (!this->has_next() == !that.has_next());
  }
};

} // namespace aspartame::details
