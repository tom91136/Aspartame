#include "../base.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

namespace aspartame {
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(trim_leading)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {};
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(trim_trailing)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(trim)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(is_blank)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(indent)(const In &in, int n) {
  if constexpr (details::unsupported<In>(in, n)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(to_upper)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(to_lower)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(replace_all)(const In &in, const In &needle, const In &with) {
  if constexpr (details::unsupported<In>(in, needle, with)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(contains_ignore_case)(const In &in, const In &that) {
  if constexpr (details::unsupported<In>(in, that)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(equals_ignore_case)(const In &in, const In &that) {
  if constexpr (details::unsupported<In>(in, that)) {}
}
template <typename In, typename Delimiter> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(split)(const In &in, const Delimiter &delimiter) {
  if constexpr (details::unsupported<In>(in, delimiter)) {}
}
template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(lines)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

} // namespace aspartame