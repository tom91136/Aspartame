#include "base.hpp"
#include "map_impl.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(keys)(const In &in) {
  return details::container2::keys<In, ASPARTAME_OUT_TYPE_C1>(in);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(values)(const In &in) {
  return details::container2::values<In, std::vector>(in);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(map_keys)(const In &in, Function &&function) {
  return details::container2::map_keys<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(map_values)(const In &in, Function &&function) {
  return details::container2::map_values<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In, typename Key> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(get)(const In &in, const Key &key) {
  return details::container2::get<In, Key>(in, key);
}

template <typename In, typename Key, typename Value> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(get_or_default)(const In &in, const Key &key, const Value &value) {
  return details::container2::get_or_default<In, Key, Value>(in, key, value);
}

} // namespace aspartame