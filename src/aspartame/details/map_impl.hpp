#pragma once
#include "base.hpp"

namespace aspartame::details::container2 {

template <typename In, template <typename...> typename Out> //
[[nodiscard]] constexpr auto keys(const In &in) {
  Out<typename In::key_type> ys;
  if constexpr (has_reserve<Out<typename In::key_type>>) ys.reserve(in.size());

  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
    auto k = it->first;
    if constexpr (has_push_back<Out<typename In::key_type>>) ys.push_back(k);
    else ys.insert(ys.end(), k);
  }

  //  for (auto [k, _] : in)
  //    if constexpr (has_push_back<Out<typename In::key_type>>) ys.push_back(k);
  //    else ys.insert(ys.end(), k);
  return ys;
}

template <typename In, template <typename...> typename Out> //
[[nodiscard]] constexpr auto values(const In &in) {
  Out<typename In::mapped_type> ys;
  if constexpr (has_reserve<Out<typename In::key_type>>) ys.reserve(in.size());

  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
    auto v = it->second;
    if constexpr (has_push_back<Out<typename In::key_type>>) ys.push_back(v);
    else ys.insert(ys.end(), v);
  }

  //  for (auto [_, v] : in)
  //    if constexpr (has_push_back<Out<typename In::key_type>>) ys.push_back(v);
  //    else ys.insert(ys.end(), v);
  return ys;
}

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto map_keys(const In &in, Function &&function) {
  using K = decltype(details::ap(function, in.begin()->first));
  if constexpr (details::assert_non_void<K>()) {}
  Out<K, typename In::mapped_type> ys;
  for (auto [k, v] : in)
    ys.emplace(details::ap(function, k), v);
  return ys;
}

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto map_values(const In &in, Function &&function) {
  using V = decltype(details::ap(function, in.begin()->second));
  if constexpr (details::assert_non_void<V>()) {}
  Out<typename In::key_type, V> ys;
  for (auto [k, v] : in)
    ys.emplace(k, details::ap(function, v));
  return ys;
}

template <typename In, typename Key> //
[[nodiscard]] constexpr std::optional<typename In::mapped_type> get(const In &in, const Key &key) {
  if (auto it = in.find(key); it != in.end()) return {it->second};
  else return std::nullopt;
}

template <typename In, typename K, typename V> //
[[nodiscard]] constexpr typename In::mapped_type get_or_default(const In &in, const K &key, const V &value) {
  static_assert(std::is_convertible_v<V, typename In::mapped_type>, "else type does not unify with map's value type");
  if (auto it = in.find(key); it != in.end()) return it->second;
  else return value;
}

} // namespace aspartame::details::container2