#include "../base.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

namespace aspartame {

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(keys)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(values)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(map_keys)(const In &in, Function &&function) {
  if constexpr (details::unsupported<In>(in, function)) {}
}
template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(map_values)(const In &in, Function &&function) {
  if constexpr (details::unsupported<In>(in, function)) {}
}

template <typename In, typename Key> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(get)(const In &in, const Key &key) {
  if constexpr (details::unsupported<In>(in, key)) {}
}

template <typename In, typename K, typename V> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(get_or_default)(const In &in, const K &key, const V &value) {
  if constexpr (details::unsupported<In>(in, key, value)) {}
}

} // namespace aspartame