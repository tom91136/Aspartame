#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>

#include "../traits.hpp"

namespace aspartame::details::container2 {

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto map(const In &in, Function f) {
  auto &&es = map_access<In>::entries(in);
  using T = decltype(details::ap(f, *std::begin(es)));
  if constexpr (details::assert_non_void<T>()) {}
  static_assert(is_pair<T>, "return type for mapping a map-like container must be a tuple");
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es) {
    auto kv = details::ap(f, x);
    map_access<Out>::put(ys, kv.first, kv.second);
  }
  return ys;
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto collect(const In &in, Function f) {
  auto &&es = map_access<In>::entries(in);
  using T = decltype(details::ap(f, *std::begin(es)));
  static_assert(is_optional<T>, "collect function should return an optional");
  static_assert(is_pair<typename T::value_type>, "return type for mapping a map-like container must be a tuple");
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es)
    if (auto y = details::ap(f, x); y) map_access<Out>::put(ys, y->first, y->second);
  return ys;
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto collect_first(const In &in, Function f) {
  auto &&es = map_access<In>::entries(in);
  using T = decltype(details::ap(f, *std::begin(es)));
  static_assert(is_optional<T>, "collect function should return an optional");
  static_assert(is_pair<typename T::value_type>, "return type for mapping a map-like container must be a tuple");
  using K = typename T::value_type::first_type;
  using V = typename T::value_type::second_type;
  for (auto &&x : es)
    if (auto y = details::ap(f, x); y) return std::optional<std::pair<K, V>>{{y->first, y->second}};
  return std::optional<std::pair<K, V>>{};
}

template <typename In, typename Out, typename Predicate> //
[[nodiscard]] constexpr auto filter(const In &in, Predicate p) {
  auto &&es = map_access<In>::entries(in);
  if constexpr (details::assert_predicate<decltype(details::ap(p, *std::begin(es)))>()) {}
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es)
    if (details::ap(p, x)) map_access<Out>::put(ys, x.first, x.second);
  return ys;
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto flat_map(const In &in, Function f) {
  auto &&es = map_access<In>::entries(in);
  static_assert(is_map_like<decltype(details::ap(f, *std::begin(es)))>, "flat_map function should return a map-like type");
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&x : es) {
    auto zs = details::ap(f, x);
    for (auto &&z : map_access<std::decay_t<decltype(zs)>>::entries(zs))
      map_access<Out>::put(ys, z.first, z.second);
  }
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr auto flatten(const In &in) {
  static_assert(is_map_like<typename In::mapped_type>, "not a nested type that is map-like");
  Out ys;
  reserve_if(ys, in.size());
  for (auto &&kv : map_access<In>::entries(in)) {
    auto &&inner = kv.second;
    for (auto &&z : map_access<std::decay_t<decltype(inner)>>::entries(inner))
      map_access<Out>::put(ys, z.first, z.second);
  }
  return ys;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto find(const In &in, Predicate p) {
  auto &&es = map_access<In>::entries(in);
  if constexpr (details::assert_predicate<decltype(details::ap(p, *std::begin(es)))>()) {}
  using K = typename In::key_type;
  using V = typename In::mapped_type;
  using T = std::pair<K, V>;
  for (auto &&x : es)
    if (details::ap(p, x)) return std::optional<T>{std::pair{x.first, x.second}};
  return std::optional<T>{std::nullopt};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto reduce(const In &in, Function f) {
  using K = typename In::key_type;
  using V = typename In::mapped_type;
  using T = std::pair<K, V>;
  static_assert(std::is_invocable_v<Function, T, T>, "function must be invocable with two value types of the container");
  static_assert(std::is_convertible_v<std::invoke_result_t<Function, T, T>, T>,
                "function must return something that is convertable to value type");
  auto &&es = map_access<In>::entries(in);
  auto it = std::begin(es);
  auto end = std::end(es);
  if (it == end) return std::optional<T>{std::nullopt};
  T r{(*it).first, (*it).second};
  ++it;
  for (; it != end; ++it)
    r = f(std::move(r), T{(*it).first, (*it).second});
  return std::optional<T>{std::move(r)};
}

template <typename In, typename Out, typename Predicate> //
[[nodiscard]] constexpr std::pair<Out, Out> partition(const In &in, Predicate p) {
  auto &&es = map_access<In>::entries(in);
  if constexpr (details::assert_predicate<decltype(details::ap(p, *std::begin(es)))>()) {}
  Out a, b;
  reserve_if(a, in.size());
  reserve_if(b, in.size());
  for (auto &&x : es) {
    if (details::ap(p, x)) map_access<Out>::put(a, x.first, x.second);
    else map_access<Out>::put(b, x.first, x.second);
  }
  return {a, b};
}

template <typename In, typename Inner, typename GroupFunction, typename MapFunction> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_map(const In &in, GroupFunction &&group, MapFunction &&map) {
  auto &&es = map_access<In>::entries(in);
  using K = decltype(details::ap(group, *std::begin(es)));
  using V = decltype(details::ap(map, *std::begin(es)));
  if constexpr (details::assert_non_void<K>() && details::assert_non_void<V>()) {}
  std::unordered_map<K, Inner> ys;
  for (auto &&x : es) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) push(it->second, details::ap(map, x));
    else ys.emplace(k, Inner{details::ap(map, x)});
  }
  return ys;
}

template <typename In, typename Inner, typename GroupFunction> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_by(const In &in, GroupFunction &&group) {
  auto &&es = map_access<In>::entries(in);
  using K = decltype(details::ap(group, *std::begin(es)));
  if constexpr (details::assert_non_void<K>()) {}
  using W = typename Inner::value_type;
  std::unordered_map<K, Inner> ys;
  for (auto &&x : es) {
    auto k = details::ap(group, x);
    W w{x.first, x.second};
    if (auto it = ys.find(k); it != ys.end()) push(it->second, w);
    else ys.emplace(k, Inner{w});
  }
  return ys;
}

} // namespace aspartame::details::container2
