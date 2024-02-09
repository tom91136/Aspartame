#include "../base.hpp"

#ifndef ASPARTAME_IN_TYPE2
  #error "ASPARTAME_IN_TYPE2 unimplemented"
#endif

namespace aspartame {

template <typename K, typename V> //
[[nodiscard]] constexpr auto keys(const ASPARTAME_IN_TYPE2(K, V) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE2(K, V)>(in)) {}
}

template <typename K, typename V> //
[[nodiscard]] constexpr auto values(const ASPARTAME_IN_TYPE2(K, V) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE2(K, V)>(in)) {}
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto map_keys(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE2(K, V)>(in, function)) {}
}
template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto map_values(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE2(K, V)>(in, function)) {}
}

template <typename K, typename V> //
[[nodiscard]] constexpr auto get(const ASPARTAME_IN_TYPE2(K, V) & in, const K &key) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE2(K, V)>(in, key)) {}
}

template <typename K, typename V> //
[[nodiscard]] constexpr auto get_or_default(const ASPARTAME_IN_TYPE2(K, V) & in, const K &key, const V &value) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE2(K, V)>(in, key, value)) {}
}

} // namespace aspartame