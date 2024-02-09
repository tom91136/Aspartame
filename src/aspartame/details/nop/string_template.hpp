#include "../base.hpp"

#ifndef ASPARTAME_IN_TYPE1
  #error "ASPARTAME_IN_TYPE1 unimplemented"
#endif

namespace aspartame {
template <typename C> //
[[nodiscard]] constexpr auto trim_leading(const ASPARTAME_IN_TYPE1(C) &in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {};
}
template <typename C> //
[[nodiscard]] constexpr auto trim_trailing(const ASPARTAME_IN_TYPE1(C) &in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto trim(const ASPARTAME_IN_TYPE1(C) &in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto is_blank(const ASPARTAME_IN_TYPE1(C) &in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto indent(const ASPARTAME_IN_TYPE1(C) &in, int n) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, n)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto to_upper(const ASPARTAME_IN_TYPE1(C) &in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto to_lower(const ASPARTAME_IN_TYPE1(C) &in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto replace_all(const ASPARTAME_IN_TYPE1(C) &in, const ASPARTAME_IN_TYPE1(C) &needle, const ASPARTAME_IN_TYPE1(C) &with) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, needle, with)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto contains_ignore_case(const ASPARTAME_IN_TYPE1(C) &in, const ASPARTAME_IN_TYPE1(C) &that) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, that)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto equals_ignore_case(const ASPARTAME_IN_TYPE1(C) &in, const ASPARTAME_IN_TYPE1(C) &that) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, that)) {}
}
template <typename C, typename Delimiter> //
[[nodiscard]] constexpr auto split(const ASPARTAME_IN_TYPE1(C) &in, const Delimiter &delimiter) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, delimiter)) {}
}
template <typename C> //
[[nodiscard]] constexpr auto lines(const ASPARTAME_IN_TYPE1(C) &in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

} // namespace aspartame