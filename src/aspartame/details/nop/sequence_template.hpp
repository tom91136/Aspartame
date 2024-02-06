#include "../base.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

namespace aspartame {

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(prepend)(const In &in, const T &t) {
  if constexpr (details::unsupported<In>(in, t)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(head_maybe)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(last_maybe)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(init)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(tail)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(at_maybe)(const In &in, size_t idx) {
  if constexpr (details::unsupported<In>(in, idx)) {}
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_of)(const In &in, const T &t) {
  if constexpr (details::unsupported<In>(in, t)) {}
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_where)(const In &in, Predicate &&predicate) {
  if constexpr (details::unsupported<In>(in, predicate)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(zip_with_index)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(zip)(const In &in, const Container &other) {
  if constexpr (details::unsupported<In>(in, other)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(transpose)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(reverse)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In, typename URBG> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(shuffle)(const In &in, URBG &urbg) {
  if constexpr (details::unsupported<In>(in, urbg)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In, typename Compare> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort)(const In &in, Compare &&compare) {
  if constexpr (details::unsupported<In>(in, compare)) {}
}

template <typename In, typename Select> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort_by)(const In &in, Select &&select) {
  if constexpr (details::unsupported<In>(in, select)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(split_at)(const In &in, size_t idx) {
  if constexpr (details::unsupported<In>(in, idx)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take_right)(const In &in, size_t n) {
  if constexpr (details::unsupported<In>(in, n)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop_right)(const In &in, size_t n) {
  if constexpr (details::unsupported<In>(in, n)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take)(const In &in, size_t n) {
  if constexpr (details::unsupported<In>(in, n)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop)(const In &in, size_t n) {
  if constexpr (details::unsupported<In>(in, n)) {}
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take_while)(const In &in, Predicate &&predicate) {
  if constexpr (details::unsupported<In>(in, predicate)) {}
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop_while)(const In &in, Predicate &&predicate) {
  if constexpr (details::unsupported<In>(in, predicate)) {}
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold_left)(const In &in, Accumulator &&init, Function &&function) {
  if constexpr (details::unsupported<In>(in, init, function)) {}
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold_right)(const In &in, Accumulator &&init, Function &&function) {
  if constexpr (details::unsupported<In>(in, init, function)) {}
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sliding)(const In &in, size_t size, size_t step) {
  if constexpr (details::unsupported<In>(in, size, step)) {}
}

} // namespace aspartame