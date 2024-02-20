#pragma once

#include <algorithm>
#include <string>

#include "base.hpp"

namespace aspartame::details::container2 {

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto map(const In &in, Function f) {
  using T = decltype(details::ap(f, *in.begin()));
  if constexpr (details::assert_non_void<T>()) {}
  static_assert(is_pair<T>, "return type for mapping a map-like container must be a tuple");
  using K = typename T::first_type;
  using V = typename T::second_type;
  Out<K, V> ys;
  std::transform(in.begin(), in.end(), std::inserter(ys, ys.begin()), [&](auto &&x) { return details::ap(f, x); });
  return ys;
}

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto collect(const In &in, Function f) {
  using T = decltype(details::ap(f, *in.begin()));
  static_assert(is_optional<T>, "collect function should return an optional");
  static_assert(is_pair<typename T::value_type>, "return type for mapping a map-like container must be a tuple");
  using K = typename T::value_type::first_type;
  using V = typename T::value_type::second_type;
  Out<K, V> ys;
  for (auto &&x : in)
    if (auto y = details::ap(f, x); y) ys.emplace(y->first, y->second);
  return ys;
}

template <typename In, typename Predicate, template <typename...> typename Out> //
[[nodiscard]] constexpr auto filter(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  using K = typename In::key_type;
  using V = typename In::mapped_type;
  Out<K, V> ys;
  std::copy_if(in.begin(), in.end(), std::inserter(ys, ys.begin()), [&](auto &&x) { return details::ap(p, x); });
  return ys;
}

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto bind(const In &in, Function f) {
  static_assert(is_map_like<decltype(details::ap(f, *in.begin()))>, "bind function should return a map-like type");
  using R = decltype(details::ap(f, *in.begin()));
  using K = typename R::key_type;
  using V = typename R::mapped_type;
  Out<K, V> ys;
  for (auto &&x : in) {
    auto zs = details::ap(f, x);
    ys.insert(zs.begin(), zs.end());
  }
  return ys;
}

template <typename In, template <typename...> typename Out> //
[[nodiscard]] constexpr auto flatten(const In &in) {
  static_assert(is_map_like<typename In::mapped_type>, "not a nested type that is map-like");
  using R = typename In::mapped_type;
  using K = typename R::key_type;
  using V = typename R::mapped_type;
  Out<K, V> ys;
  for (auto [_, v] : in)
    ys.insert(v.begin(), v.end());
  return ys;
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto reduce(const In &in, Function f) {
  using K = typename In::key_type;
  using V = typename In::mapped_type;
  using T = std::pair<K, V>;
  static_assert(std::is_invocable_v<Function, T, T>, "function must be invocable with two value types of the container");
  static_assert(std::is_convertible_v<std::invoke_result_t<Function, T, T>, T>,
                "function must return something that is convertable to value type");
  if (auto size = in.size(); size == 0) return std::optional<T>{std::nullopt};
  else if (size == 1) return std::optional<T>{*in.begin()};
  else {
    auto it = in.begin();
    if (it == in.end()) return std::optional<T>{std::nullopt};
    T r = *it;
    ++it;
    for (; it != in.end(); ++it)
      r = f(r, *it);
    return std::optional<T>{r};
  }
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto find(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  auto it = std::find_if(in.begin(), in.end(), [&](auto &&x) { return details::ap(p, x); });
  using K = typename In::key_type;
  using V = typename In::mapped_type;
  using T = std::pair<K, V>;
  if (it == in.end()) return std::optional<T>{std::nullopt};
  else return std::optional<T>{std::pair{it->first, it->second}};
}

template <typename In, typename GroupFunction, typename MapFunction, template <typename...> typename Out> //
[[nodiscard]] constexpr auto group_map(const In &in, GroupFunction &&group, MapFunction &&map) {
  using K = decltype(details::ap(group, *in.begin()));
  using V = decltype(details::ap(map, *in.begin()));
  if constexpr (details::assert_non_void<K>() && details::assert_non_void<V>()) {}
  std::unordered_map<K, Out<V>> ys;
  for (auto &&x : in) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) it->second.insert(it->second.end(), details::ap(map, x));
    else ys.emplace(k, Out<V>{details::ap(map, x)});
  }
  return ys;
}

template <typename In, typename GroupFunction, template <typename...> typename Out> //
[[nodiscard]] constexpr auto group_by(const In &in, GroupFunction &&group) {
  using K = decltype(details::ap(group, *in.begin()));
  if constexpr (details::assert_non_void<K>()) {}
  using V = typename In::value_type;
  using W = std::pair<std::remove_const_t<typename V::first_type>, typename V::second_type>;
  std::unordered_map<K, Out<W>> ys;
  for (auto &&x : in) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) it->second.insert(it->second.end(), x);
    else ys.emplace(k, Out<W>{x});
  }
  return ys;
}

} // namespace aspartame::details::container2