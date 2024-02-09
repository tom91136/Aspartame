#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename T = typename InputIterator::value_type>
class slice_iterator : public fwd_iterator<slice_iterator<InputIterator, T>, T> {
  InputIterator it, end;
  size_t idx{}, to_exclusive{};
  [[nodiscard]] constexpr bool has_next() const { return idx < to_exclusive && it != end; }

public:
  constexpr slice_iterator() = default;
  constexpr slice_iterator(InputIterator begin, InputIterator end, size_t from_inclusive, size_t to_exclusive)
      : it(std::move(begin)), end(std::move(end)), idx(0), to_exclusive(to_exclusive) {
    while (it != this->end && idx < from_inclusive) {
      ++it;
      idx++;
    }
  }
  constexpr slice_iterator &operator++() {
    ++it;
    idx++;
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *it; }

  [[nodiscard]] constexpr bool operator==(const slice_iterator &that) const {
    return (it == that.it) || (!this->has_next() == !that.has_next());
  }
};

} // namespace aspartame::details
