#include "base.hpp"
#include "map_impl.hpp"

#ifndef ASPARTAME_IN_TYPE2
  #error "ASPARTAME_IN_TYPE2 unimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename K, typename V, typename Key, typename Function> //
[[nodiscard]] constexpr auto get_or_emplace(ASPARTAME_IN_TYPE2(K, V) & in, Key &key, Function &&function, tag = {}) {
  if (auto it = in.find(key); it != in.end()) {
    return it->second;
  } else {
    return in.emplace(key, std::move(details::ap(function, key))).first->second;
  }
}

template <typename K, typename V> //
[[nodiscard]] constexpr auto keys(const ASPARTAME_IN_TYPE2(K, V) & in, tag = {}) {
  return details::container2::keys<ASPARTAME_IN_TYPE2(K, V), ASPARTAME_OUT_TYPE_C1>(in);
}

template <typename K, typename V> //
[[nodiscard]] constexpr auto values(const ASPARTAME_IN_TYPE2(K, V) & in, tag = {}) {
  return details::container2::values<ASPARTAME_IN_TYPE2(K, V), std::vector>(in);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto map_keys(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function, tag = {}) {
  return details::container2::map_keys<ASPARTAME_IN_TYPE2(K, V), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto map_values(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function, tag = {}) {
  return details::container2::map_values<ASPARTAME_IN_TYPE2(K, V), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename K, typename V, typename Key> //
[[nodiscard]] constexpr auto get_maybe(const ASPARTAME_IN_TYPE2(K, V) & in, const Key &key, tag = {}) {
  return details::container2::get_maybe<ASPARTAME_IN_TYPE2(K, V), Key>(in, key);
}

template <typename K, typename V, typename Key, typename Value> //
[[nodiscard]] constexpr auto get_or_default(const ASPARTAME_IN_TYPE2(K, V) & in, const Key &key, const Value &value, tag = {}) {
  return details::container2::get_or_default<ASPARTAME_IN_TYPE2(K, V), Key, Value>(in, key, value);
}

} // namespace aspartame