#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename Iter1, //
          typename Iter2, //
          typename T = typename details::value_type_of_t<Iter1>>
class concat_iterator : public fwd_iterator<concat_iterator<Iter1, Iter2, T>, T> {
  static_assert(std::is_same_v<T, typename details::value_type_of_t<Iter2>>, //
                "Iterators must be of the same value type");
  Iter1 it1, end1;
  Iter2 it2, end2;
  bool on_first = false;
  [[nodiscard]] constexpr bool has_next() const { return on_first || it2 != end2; }

public:
  constexpr concat_iterator() = default;
  constexpr concat_iterator(Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 end2)
      : it1(std::move(begin1)), end1(std::move(end1)), //
        it2(std::move(begin2)), end2(std::move(end2)), //
        on_first(this->it1 != this->end1) {}
  constexpr concat_iterator &operator++() {
    if (on_first) {
      if (++it1 == end1) on_first = false;
    } else ++it2;
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return on_first ? *it1 : *it2; }
  [[nodiscard]] constexpr bool operator==(const concat_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
