#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename T = value_type_of_t<InputIterator>>
class stride_iterator : public fwd_iterator<stride_iterator<InputIterator, T>, T> {
  InputIterator it, end;
  size_t n = 0;

  [[nodiscard]] constexpr bool has_next() const { return n > 0 && it != end; }

public:
  constexpr stride_iterator() = default;
  constexpr stride_iterator(InputIterator begin, InputIterator end_, size_t n_) : it(std::move(begin)), end(std::move(end_)), n(n_) {}
  constexpr stride_iterator &operator++() {
    for (size_t i = 0; i < n && it != end; ++i)
      ++it;
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *it; }
  [[nodiscard]] constexpr bool operator==(const stride_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
