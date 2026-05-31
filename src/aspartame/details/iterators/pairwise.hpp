#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename T = value_type_of_t<InputIterator>>
class pairwise_iterator : public fwd_iterator<pairwise_iterator<InputIterator, T>, std::pair<T, T>, std::input_iterator_tag> {
  using OutT = std::pair<T, T>;
  InputIterator prev, it, end;
  std::optional<OutT> current;
  bool done = true;

  [[nodiscard]] constexpr bool has_next() const { return !done; }

public:
  constexpr pairwise_iterator() = default;
  constexpr pairwise_iterator(InputIterator begin, InputIterator end_) : prev(begin), it(begin), end(std::move(end_)) {
    if (it == end) return;
    ++it;
    if (it == end) return;
    done = false;
  }
  constexpr pairwise_iterator &operator++() {
    ++prev;
    ++it;
    if (it == end) done = true;
    return *this;
  }
  [[nodiscard]] constexpr const OutT &operator*() {
    current = OutT{*prev, *it};
    return *current;
  }
  [[nodiscard]] constexpr bool operator==(const pairwise_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
