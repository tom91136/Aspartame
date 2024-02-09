#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"
#include <unordered_set>

namespace aspartame::details {

template <typename V, class F> struct Hash {
  ca_optional<F> f;
  Hash() = default;
  explicit Hash(ca_optional<F> f) : f(f) {}
  template <typename T> size_t operator()(const T &x) const { return std::hash<V>()((*f)(x)); }
};

template <class F> struct Equal {
  ca_optional<F> f;
  Equal() = default;
  explicit Equal(ca_optional<F> f) : f(f) {}
  template <typename T> bool operator()(const T &l, const T &r) const { return (*f)(l) == (*f)(r); }
};

template <typename InputIterator, //
          typename Function,      //
          typename T = typename InputIterator::value_type>
class distinct_iterator : public fwd_iterator<distinct_iterator<InputIterator, Function, T>, T> {
  InputIterator it, end;
  ca_optional<Function> f;
  using V = decltype((*f)(*it));
  std::unordered_set<T, Hash<V, Function>, Equal<Function>> seen{};

  [[nodiscard]] constexpr bool has_next() const { return f && it != end; }

public:
  constexpr distinct_iterator() = default;
  constexpr distinct_iterator(InputIterator begin, InputIterator end, Function function)
      : it(std::move(begin)), end(std::move(end)), f(function), seen(0, Hash<V, Function>(f), Equal<Function>(f)) {
    static_assert(is_hashable<V>, "distinct type must be hashable");
    static_assert(is_comparable<V>, "distinct type must be comparable");
    while (it != this->end && !seen.emplace(*it).second)
      ++it;
  }
  constexpr distinct_iterator &operator++() {
    do {
      ++it;
    } while (it != end && !seen.emplace(*it).second);
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *it; }

  [[nodiscard]] constexpr bool operator==(const distinct_iterator &that) const {
    return (it == that.it) || (!this->has_next() == !that.has_next());
  }
};

} // namespace aspartame::details
