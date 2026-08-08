#pragma once

#include <unordered_set>

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Function,      //
          typename T = typename details::value_type_of_t<InputIterator>, typename Reference = decltype(*std::declval<InputIterator &>())>
class distinct_iterator
    : public fwd_iterator<distinct_iterator<InputIterator, Function, T, Reference>, T, std::forward_iterator_tag, Reference> {
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
  [[nodiscard]] constexpr Reference operator*() { return *it; }
  [[nodiscard]] constexpr bool operator==(const distinct_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

template <typename InputIterator, typename Predicate, typename Function, typename T = typename details::value_type_of_t<InputIterator>,
          typename Reference = decltype(*std::declval<InputIterator &>())>
class conditional_distinct_iterator : public fwd_iterator<conditional_distinct_iterator<InputIterator, Predicate, Function, T, Reference>,
                                                          T, std::forward_iterator_tag, Reference> {
  InputIterator it, end;
  ca_optional<Predicate> predicate;
  ca_optional<Function> f;
  using V = std::decay_t<decltype((*f)(*it))>;
  std::unordered_set<V> seen;
  [[nodiscard]] constexpr bool has_next() const { return predicate && f && it != end; }

  constexpr bool accept() {
    if (!(*predicate)(*it)) return true;
    return seen.insert((*f)(*it)).second;
  }

public:
  constexpr conditional_distinct_iterator() = default;
  constexpr conditional_distinct_iterator(InputIterator begin, InputIterator end_, Predicate predicate_, Function function_)
      : it(std::move(begin)), end(std::move(end_)), predicate(std::move(predicate_)), f(std::move(function_)) {
    static_assert(is_hashable<V>, "distinct type must be hashable");
    static_assert(is_comparable<V>, "distinct type must be comparable");
    if (it != end && !accept()) operator++();
  }
  constexpr conditional_distinct_iterator &operator++() {
    do {
      ++it;
    } while (it != end && !accept());
    return *this;
  }
  [[nodiscard]] constexpr Reference operator*() { return *it; }
  [[nodiscard]] constexpr bool operator==(const conditional_distinct_iterator &that) const { return (!has_next() == !that.has_next()); }
};

} // namespace aspartame::details
