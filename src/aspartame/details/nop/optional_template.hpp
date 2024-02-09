#include "../base.hpp"

#ifndef ASPARTAME_IN_TYPE1
  #error "ASPARTAME_IN_TYPE1 unimplemented"
#endif

namespace aspartame {

template <typename C, typename Action, typename EmptyAction> //
[[nodiscard]] constexpr auto fold(const ASPARTAME_IN_TYPE1(C) & in, Action &&action, EmptyAction &&empty) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, action, empty)) {}
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto get_or_else(const ASPARTAME_IN_TYPE1(C) & in, T &&t) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, t)) {}
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto or_else(const ASPARTAME_IN_TYPE1(C) & in, T &&t) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, t)) {}
}

} // namespace aspartame