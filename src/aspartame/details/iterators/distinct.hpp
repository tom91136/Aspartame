#pragma once

#include <unordered_set>

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Function,      //
          typename T = typename details::value_type_of_t<InputIterator>>
class distinct_iterator : public fwd_iterator<distinct_iterator<InputIterator, Function, T>, T> {
  InputIterator it, end;
  ca_optional<Function> f;
  using V = std::decay_t<decltype((*f)(*it))>;
  std::unordered_set<V> seen{}; // XXX keys, not whole elements for performance

  [[nodiscard]] constexpr bool has_next() const { return f && it != end; }

public:
  constexpr distinct_iterator() = default;
  ASPARTAME_CONSTEXPR_ALLOC distinct_iterator(InputIterator begin, InputIterator end, Function function)
      : it(std::move(begin)), end(std::move(end)), f(function) {
    static_assert(is_hashable<V>, "distinct type must be hashable");
    static_assert(is_comparable<V>, "distinct type must be comparable");
    while (it != this->end && !seen.insert((*f)(*it)).second)
      ++it;
  }
  ASPARTAME_CONSTEXPR_ALLOC distinct_iterator &operator++() {
    do {
      ++it;
    } while (it != end && !seen.insert((*f)(*it)).second);
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *it; }
  [[nodiscard]] constexpr bool operator==(const distinct_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
