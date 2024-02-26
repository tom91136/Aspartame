#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Predicate,     //
          typename T = typename details::value_type_of_t<InputIterator>>
class filter_iterator : public fwd_iterator<filter_iterator<InputIterator, Predicate, T>, T> {
  InputIterator it, end;
  ca_optional<Predicate> p;
  [[nodiscard]] constexpr bool has_next() const { return p && it != end; }

public:
  constexpr filter_iterator() = default;
  constexpr filter_iterator(InputIterator begin_, InputIterator end, Predicate predicate)
      : it(std::move(begin_)), end(std::move(end)), p(predicate) {
    if constexpr (details::assert_predicate<decltype((*p)(*it))>()) {}
    while (it != this->end && !(*p)(*it))
      ++it;
  }
  constexpr filter_iterator &operator++() {
    do {
      ++it;
    } while (it != end && !(*p)(*it));
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *it; }
  [[nodiscard]] constexpr bool operator==(const filter_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
