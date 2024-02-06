#include "../base.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

namespace aspartame {

template <typename In, typename Action, typename EmptyAction> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold)(const In &in, Action &&action, EmptyAction &&empty) {
  if constexpr (details::unsupported<In>(in, action, empty)) {}
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(get_or_else)(const In &in, T &&t) {
  if constexpr (details::unsupported<In>(in, t)) {}
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(or_else)(const In &in, T &&t) {
  if constexpr (details::unsupported<In>(in, t)) {}
}

} // namespace aspartame