#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <unordered_set>

#include "../traits.hpp"

namespace aspartame::details::inplace {

struct always_true_pred {
  template <typename T> constexpr bool operator()(T &&) const { return true; }
};

template <typename C, typename = void> constexpr bool has_remove_if = false;
template <typename C> constexpr bool has_remove_if<C, std::void_t<decltype(std::declval<C &>().remove_if(always_true_pred{}))>> = true;

template <typename C, typename = void> constexpr bool node_erasable = false;
template <typename C>
constexpr bool node_erasable<
    C, std::void_t<typename C::key_type,
                   std::enable_if_t<!std::is_void_v<decltype(std::declval<C &>().erase(std::declval<typename C::const_iterator>()))>>>> =
    true;

template <typename C, typename = void> constexpr bool has_erase_range = false;
template <typename C>
constexpr bool has_erase_range<C, std::void_t<decltype(std::declval<C &>().erase(std::declval<typename C::const_iterator>(),
                                                                                 std::declval<typename C::const_iterator>()))>> = true;

template <typename C, typename = void> constexpr bool is_random_access = false;
template <typename C>
constexpr bool is_random_access<C, std::void_t<typename std::iterator_traits<typename C::iterator>::iterator_category>> =
    std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<typename C::iterator>::iterator_category>;

template <typename C>
constexpr bool can_filter_inplace = has_remove_if<C> || node_erasable<C> || (is_random_access<C> && has_erase_range<C>);
template <typename C> constexpr bool can_truncate_inplace = has_erase_range<C> && (is_random_access<C> || has_remove_if<C>);
template <typename C> constexpr bool can_sort_inplace = has_sort<C> || is_random_access<C>;
template <typename C> constexpr bool can_reverse_inplace = has_reverse<C> || is_random_access<C>;
template <typename C> constexpr bool can_concat_inplace = has_associative_insert<C> || has_push_back<C>;
template <typename C> constexpr bool can_append_inplace = has_push_back<C> || has_value_insert_v<C>;
template <typename C> constexpr bool can_shuffle_inplace = is_random_access<C>;

template <typename C, typename = void> constexpr bool can_prepend_inplace = false;
template <typename C>
constexpr bool can_prepend_inplace<
    C, std::void_t<decltype(std::declval<C &>().insert(std::declval<C &>().begin(), std::declval<typename C::value_type>()))>> = true;

template <typename C, typename F, typename = void> constexpr bool can_map_inplace = false;
template <typename C, typename F>
constexpr bool can_map_inplace<C, F, std::void_t<decltype(*std::begin(std::declval<C &>()))>> =
    std::is_same_v<std::decay_t<decltype(*std::begin(std::declval<C &>()))>,
                   std::decay_t<decltype(details::ap(std::declval<F &>(), *std::begin(std::declval<C &>())))>> &&
    std::is_assignable_v<decltype(*std::begin(std::declval<C &>())),
                         decltype(details::ap(std::declval<F &>(), *std::begin(std::declval<C &>())))>;

template <typename C, typename F, typename = void> constexpr bool can_map_values_inplace = false;
template <typename C, typename F>
constexpr bool can_map_values_inplace<C, F, std::void_t<decltype(std::declval<C &>().begin()->second)>> =
    !std::is_const_v<std::remove_reference_t<decltype(std::declval<C &>().begin()->second)>> &&
    std::is_same_v<std::decay_t<decltype(std::declval<C &>().begin()->second)>,
                   std::decay_t<decltype(details::ap(std::declval<F &>(), std::declval<C &>().begin()->second))>>;

template <typename C, typename V> constexpr bool can_distinct_inplace = can_filter_inplace<C> && is_hashable<V> && is_comparable<V>;

template <typename C, typename = void> constexpr bool has_subscript = false;
template <typename C> constexpr bool has_subscript<C, std::void_t<decltype(std::declval<C &>()[size_t{}])>> = true;

template <typename C, typename = void> constexpr bool has_range_insert = false;
template <typename C>
constexpr bool has_range_insert<
    C, std::void_t<decltype(std::declval<C &>().insert(std::declval<C &>().end(), std::declval<typename C::const_iterator>(),
                                                       std::declval<typename C::const_iterator>()))>> = true;

template <typename C, typename Other> void concat(C &c, const Other &other) {
  // Self-append (xs ^= concat(xs)) aliases storage; snapshot before insert.
  if constexpr (std::is_same_v<C, Other>) {
    if (static_cast<const void *>(&c) == static_cast<const void *>(&other)) {
      if constexpr (has_associative_insert<C>) {
        return;
      } else {
        const size_t n = c.size();
        reserve_if(c, n + n);
        if constexpr (has_subscript<C>) {
          for (size_t i = 0; i < n; ++i)
            c.push_back(c[i]);
        } else {
          auto it = c.begin();
          for (size_t i = 0; i < n; ++i, ++it)
            c.push_back(*it);
        }
        return;
      }
    }
  }
  if constexpr (has_associative_insert<C>) c.insert(other.begin(), other.end());
  else if constexpr (has_range_insert<C>) c.insert(c.end(), other.begin(), other.end());
  else
    for (auto &&x : other)
      push(c, x);
}

template <typename C, typename T> void append(C &c, const T &t) { push(c, t); }

template <typename C, typename T> void prepend(C &c, const T &t) { c.insert(c.begin(), t); }

template <typename C, typename F> void map_values(C &c, F f) {
  for (auto it = c.begin(), end = c.end(); it != end; ++it)
    it->second = details::ap(f, it->second);
}

template <typename C, typename F> void distinct_by(C &c, F f) {
  using V = std::decay_t<decltype(details::ap(f, *c.begin()))>;
  std::unordered_set<V> seen;
  seen.reserve(c.size());
  const auto dup = [&](auto &&x) { return !seen.insert(details::ap(f, x)).second; };
  if constexpr (has_remove_if<C>) c.remove_if(dup);
  else if constexpr (node_erasable<C>) {
    for (auto it = c.begin(), end = c.end(); it != end;) {
      if (dup(*it)) it = c.erase(it);
      else ++it;
    }
  } else c.erase(std::remove_if(c.begin(), c.end(), dup), c.end());
}

template <typename C> void distinct(C &c) {
  if constexpr (is_set_like_impl<C>) return;
  else distinct_by(c, [](auto &&x) { return x; });
}

template <typename C> void slice(C &c, size_t from_inclusive, size_t to_exclusive) {
  const size_t c_size = static_cast<size_t>(c.size());
  if (from_inclusive >= c_size || from_inclusive > to_exclusive) {
    c.clear();
    return;
  }
  size_t hi = to_exclusive < c_size ? to_exclusive : c_size;
  auto tail = c.begin();
  std::advance(tail, static_cast<std::make_signed_t<size_t>>(hi));
  c.erase(tail, c.end());
  auto head = c.begin();
  std::advance(head, static_cast<std::make_signed_t<size_t>>(from_inclusive));
  c.erase(c.begin(), head);
}

template <typename C, typename URBG> void shuffle(C &c, URBG &&urbg) { std::shuffle(c.begin(), c.end(), std::forward<URBG>(urbg)); }

template <typename C, typename F> void tap_each(C &c, F f) {
  for (auto &&x : c)
    details::ap(f, x);
}

template <typename C, typename P> void filter(C &c, P p) {
  if constexpr (has_remove_if<C>) {
    c.remove_if([&](auto &&x) { return !details::ap(p, x); });
  } else if constexpr (node_erasable<C>) {
    for (auto it = c.begin(), end = c.end(); it != end;) {
      if (details::ap(p, *it)) ++it;
      else it = c.erase(it);
    }
  } else {
    c.erase(std::remove_if(c.begin(), c.end(), [&](auto &&x) { return !details::ap(p, x); }), c.end());
  }
}

template <typename C, typename F> void map(C &c, F f) {
  for (auto &x : c)
    x = details::ap(f, x);
}

template <typename C> void sort(C &c) {
  if constexpr (has_sort<C>) c.sort();
  else std::sort(c.begin(), c.end());
}

template <typename C, typename Cmp> void sort(C &c, Cmp cmp) {
  if constexpr (has_sort<C>) c.sort(cmp);
  else std::sort(c.begin(), c.end(), cmp);
}

template <typename C, typename Sel> void sort_by(C &c, Sel s) {
  const auto cmp = [&](auto &&l, auto &&r) { return details::ap(s, l) < details::ap(s, r); };
  if constexpr (has_sort<C>) c.sort(cmp);
  else std::sort(c.begin(), c.end(), cmp);
}

template <typename C> void reverse(C &c) {
  if constexpr (has_reverse<C>) c.reverse();
  else std::reverse(c.begin(), c.end());
}

template <typename C> void take(C &c, size_t n) {
  if (n >= static_cast<size_t>(c.size())) return;
  auto it = c.begin();
  std::advance(it, static_cast<std::make_signed_t<size_t>>(n));
  c.erase(it, c.end());
}

template <typename C> void drop(C &c, size_t n) {
  if (n >= static_cast<size_t>(c.size())) {
    c.clear();
    return;
  }
  auto it = c.begin();
  std::advance(it, static_cast<std::make_signed_t<size_t>>(n));
  c.erase(c.begin(), it);
}

template <typename C, typename P> void take_while(C &c, P p) {
  auto it = c.begin();
  while (it != c.end() && details::ap(p, *it))
    ++it;
  c.erase(it, c.end());
}

template <typename C, typename P> void drop_while(C &c, P p) {
  auto it = c.begin();
  while (it != c.end() && details::ap(p, *it))
    ++it;
  c.erase(c.begin(), it);
}

} // namespace aspartame::details::inplace
