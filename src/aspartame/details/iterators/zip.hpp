#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename Iter1,                           //
          typename Iter2,                           //
          typename I1 = typename details::value_type_of_t<Iter1>, //
          typename I2 = typename details::value_type_of_t<Iter2>, //
          typename T = std::pair<I1, I2>>
class zip_iterator : public fwd_iterator<zip_iterator<Iter1, Iter2, I1, I2, T>, T> {
  Iter1 it1, end1;
  Iter2 it2, end2;
  std::optional<T> current;
  [[nodiscard]] constexpr bool has_next() const { return it1 != end1 && it2 != end2; }

public:
  constexpr zip_iterator() = default;
  constexpr zip_iterator(Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 end2)
      : it1(std::move(begin1)), end1(std::move(end1)), //
        it2(std::move(begin2)), end2(std::move(end2)) {}

  constexpr zip_iterator &operator++() {
    ++it1;
    ++it2;
    return *this;
  }

  [[nodiscard]] constexpr const T &operator*() { return *(current = std::pair{*it1, *it2}); }
  [[nodiscard]] constexpr bool operator==(const zip_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
