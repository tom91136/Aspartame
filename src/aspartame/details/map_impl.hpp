#pragma once

#include <iterator>
#include <optional>

#include "../traits.hpp"

namespace aspartame::details::container2 {

template <typename In, typename Out> //
[[nodiscard]] constexpr auto keys(const In &in) {
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : map_access<In>::entries(in))
    push(ys, x.first);
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr auto values(const In &in) {
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : map_access<In>::entries(in))
    push(ys, x.second);
  return ys;
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto map_keys(const In &in, Function &&function) {
  auto &&es = map_access<In>::entries(in);
  using K = decltype(details::ap(function, (*std::begin(es)).first));
  if constexpr (details::assert_non_void<K>()) {}
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es)
    map_access<Out>::put(ys, details::ap(function, x.first), x.second);
  return ys;
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto map_values(const In &in, Function &&function) {
  auto &&es = map_access<In>::entries(in);
  using V = decltype(details::ap(function, (*std::begin(es)).second));
  if constexpr (details::assert_non_void<V>()) {}
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es)
    map_access<Out>::put(ys, x.first, details::ap(function, x.second));
  return ys;
}

template <typename In, typename Out, typename Predicate> //
[[nodiscard]] constexpr auto filter_keys(const In &in, Predicate p) {
  auto &&es = map_access<In>::entries(in);
  if constexpr (details::assert_predicate<decltype(details::ap(p, (*std::begin(es)).first))>()) {}
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es)
    if (details::ap(p, x.first)) map_access<Out>::put(ys, x.first, x.second);
  return ys;
}

template <typename In, typename Out, typename Predicate> //
[[nodiscard]] constexpr auto filter_values(const In &in, Predicate p) {
  auto &&es = map_access<In>::entries(in);
  if constexpr (details::assert_predicate<decltype(details::ap(p, (*std::begin(es)).second))>()) {}
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es)
    if (details::ap(p, x.second)) map_access<Out>::put(ys, x.first, x.second);
  return ys;
}

template <typename In, typename Key> //
[[nodiscard]] constexpr auto get_maybe(const In &in, const Key &key) {
  return map_access<In>::lookup(in, key);
}

template <typename In, typename K, typename V> //
[[nodiscard]] constexpr typename In::mapped_type get_or_default(const In &in, const K &key, const V &value) {
  static_assert(std::is_convertible_v<V, typename In::mapped_type>, "else type does not unify with map's value type");
  if (auto o = map_access<In>::lookup(in, key); o) return *o;
  else return static_cast<typename In::mapped_type>(value);
}

template <typename In, typename Out, typename Other, typename Function> //
[[nodiscard]] constexpr auto combine(const In &in, const Other &other, Function f) {
  Out ys;
  reserve_if(ys, in.size() + other.size());
  for (auto &&x : map_access<In>::entries(in)) {
    if (auto o = map_access<Other>::lookup(other, x.first)) map_access<Out>::put(ys, x.first, f(x.second, *o));
    else map_access<Out>::put(ys, x.first, x.second);
  }
  for (auto &&x : map_access<Other>::entries(other))
    if (!map_access<In>::lookup(in, x.first)) map_access<Out>::put(ys, x.first, x.second);
  return ys;
}

} // namespace aspartame::details::container2
