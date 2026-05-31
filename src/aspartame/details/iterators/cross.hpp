#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename LeftIterator, //
          typename RightIterator, typename T = std::pair<value_type_of_t<LeftIterator>, value_type_of_t<RightIterator>>>
class cross_iterator : public fwd_iterator<cross_iterator<LeftIterator, RightIterator, T>, T, std::input_iterator_tag> {
  LeftIterator left_it, left_end;
  RightIterator right_begin, right_it, right_end;
  std::optional<T> current;

  [[nodiscard]] constexpr bool has_next() const { return left_it != left_end; }

public:
  constexpr cross_iterator() = default;
  constexpr cross_iterator(LeftIterator l_begin, LeftIterator l_end, RightIterator r_begin, RightIterator r_end)
      : left_it(std::move(l_begin)), left_end(std::move(l_end)), right_begin(r_begin), right_it(r_begin), right_end(std::move(r_end)) {
    if (right_begin == right_end) left_it = left_end;
  }
  constexpr cross_iterator &operator++() {
    ++right_it;
    if (right_it == right_end) {
      ++left_it;
      right_it = right_begin;
    }
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() {
    current = T{*left_it, *right_it};
    return *current;
  }
  [[nodiscard]] constexpr bool operator==(const cross_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
