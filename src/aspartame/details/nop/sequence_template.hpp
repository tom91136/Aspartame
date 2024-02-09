#include "../base.hpp"

#ifndef ASPARTAME_IN_TYPE1
  #error "ASPARTAME_IN_TYPE1 unimplemented"
#endif

namespace aspartame {

template <typename C, typename T> //
[[nodiscard]] constexpr auto prepend(const ASPARTAME_IN_TYPE1(C) & in, const T &t) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, t)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto head_maybe(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto last_maybe(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto init(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto tail(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto at_maybe(const ASPARTAME_IN_TYPE1(C) & in, size_t idx) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, idx)) {}
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto index_of(const ASPARTAME_IN_TYPE1(C) & in, const T &t) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, t)) {}
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto index_where(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, predicate)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto zip_with_index(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto zip(const ASPARTAME_IN_TYPE1(C) & in, const Container &other) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, other)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto transpose(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto reverse(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C, typename URBG> //
[[nodiscard]] constexpr auto shuffle(const ASPARTAME_IN_TYPE1(C) & in, URBG &urbg) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, urbg)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto sort(const ASPARTAME_IN_TYPE1(C) & in) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in)) {}
}

template <typename C, typename Compare> //
[[nodiscard]] constexpr auto sort(const ASPARTAME_IN_TYPE1(C) & in, Compare &&compare) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, compare)) {}
}

template <typename C, typename Select> //
[[nodiscard]] constexpr auto sort_by(const ASPARTAME_IN_TYPE1(C) & in, Select &&select) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, select)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto split_at(const ASPARTAME_IN_TYPE1(C) & in, size_t idx) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, idx)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto take_right(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, n)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto drop_right(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, n)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto take(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, n)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto drop(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, n)) {}
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto take_while(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, predicate)) {}
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto drop_while(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, predicate)) {}
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(const ASPARTAME_IN_TYPE1(C) & in, Accumulator &&init, Function &&function) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, init, function)) {}
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(const ASPARTAME_IN_TYPE1(C) & in, Accumulator &&init, Function &&function) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, init, function)) {}
}

template <typename C> //
[[nodiscard]] constexpr auto sliding(const ASPARTAME_IN_TYPE1(C) & in, size_t size, size_t step) {
  if constexpr (details::unsupported<ASPARTAME_IN_TYPE1(C)>(in, size, step)) {}
}

} // namespace aspartame