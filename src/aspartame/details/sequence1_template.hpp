#include <algorithm>

#include "base.hpp"
#include "sequence1_impl.hpp"

#ifndef ASPARTAME_IN_TYPE1
  #error "ASPARTAME_IN_TYPE1 unimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename C, typename T> //
[[nodiscard]] constexpr auto prepend(const ASPARTAME_IN_TYPE1(C) & in, const T &t) {
  return details::sequence1::prepend<ASPARTAME_IN_TYPE1(C), T, ASPARTAME_IN_TYPE1(C)>(in, t);
}

template <typename C> //
[[nodiscard]] constexpr auto head_maybe(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::head_maybe<ASPARTAME_IN_TYPE1(C)>(in);
}

template <typename C> //
[[nodiscard]] constexpr auto last_maybe(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::last_maybe<ASPARTAME_IN_TYPE1(C)>(in);
}

template <typename C> //
[[nodiscard]] constexpr auto init(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::init<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in);
}

template <typename C> //
[[nodiscard]] constexpr auto tail(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::tail<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in);
}

template <typename C> //
[[nodiscard]] constexpr auto at_maybe(const ASPARTAME_IN_TYPE1(C) & in, size_t idx) {
  return details::sequence1::at_maybe<ASPARTAME_IN_TYPE1(C)>(in, idx);
}

template <typename C> //
[[nodiscard]] constexpr auto slice(const ASPARTAME_IN_TYPE1(C) & in, size_t from_inclusive, size_t to_exclusive) {
  return details::sequence1::slice<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in, from_inclusive, to_exclusive);
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto index_of_slice(const ASPARTAME_IN_TYPE1(C) & in, const Container &other) {
  return details::sequence1::index_of_slice<ASPARTAME_IN_TYPE1(C), Container>(in, other);
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto contains_slice(const ASPARTAME_IN_TYPE1(C) & in, const Container &other) {
  return details::sequence1::index_of_slice<ASPARTAME_IN_TYPE1(C), Container>(in, other) != -1;
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto index_of(const ASPARTAME_IN_TYPE1(C) & in, const T &t) {
  return details::sequence1::index_of<ASPARTAME_IN_TYPE1(C)>(in, t);
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto contains(const ASPARTAME_IN_TYPE1(C) & in, const T &t) {
  return details::sequence1::index_of<ASPARTAME_IN_TYPE1(C)>(in, t) != -1;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto find_last(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate) {
  return details::sequence1::find_last<ASPARTAME_IN_TYPE1(C), Predicate>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto index_where(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate) {
  return details::sequence1::index_where<ASPARTAME_IN_TYPE1(C), Predicate>(in, predicate);
}

template <typename C> //
[[nodiscard]] constexpr auto zip_with_index(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::zip_with_index<ASPARTAME_IN_TYPE1(C), ASPARTAME_OUT_TYPE>(in);
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto zip(const ASPARTAME_IN_TYPE1(C) & in, const Container &other) {
  return details::sequence1::zip<ASPARTAME_IN_TYPE1(C), Container, ASPARTAME_OUT_TYPE>(in, other);
}

template <typename C> //
[[nodiscard]] constexpr auto transpose(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::transpose<ASPARTAME_IN_TYPE1(C), ASPARTAME_OUT_TYPE>(in);
}

template <typename C> //
[[nodiscard]] constexpr auto reverse(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::reverse<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in);
}

template <typename C, typename URBG> //
[[nodiscard]] constexpr auto shuffle(const ASPARTAME_IN_TYPE1(C) & in, URBG &&urbg) {
  return details::sequence1::shuffle<ASPARTAME_IN_TYPE1(C), URBG, ASPARTAME_IN_TYPE1(C)>(in, std::forward<URBG &&>(urbg));
}

template <typename C> //
[[nodiscard]] constexpr auto sort(const ASPARTAME_IN_TYPE1(C) & in) {
  return details::sequence1::sort<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in);
}

template <typename C, typename Compare> //
[[nodiscard]] constexpr auto sort(const ASPARTAME_IN_TYPE1(C) & in, Compare &&compare) {
  return details::sequence1::sort<ASPARTAME_IN_TYPE1(C), Compare, ASPARTAME_IN_TYPE1(C)>(in, compare);
}

template <typename C, typename Select> //
[[nodiscard]] constexpr auto sort_by(const ASPARTAME_IN_TYPE1(C) & in, Select &&select) {
  return details::sequence1::sort_by<ASPARTAME_IN_TYPE1(C), Select, ASPARTAME_IN_TYPE1(C)>(in, select);
}

template <typename C> //
[[nodiscard]] constexpr auto split_at(const ASPARTAME_IN_TYPE1(C) & in, size_t idx) {
  return details::sequence1::split_at<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in, idx);
}

template <typename C> //
[[nodiscard]] constexpr auto take_right(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  return details::sequence1::take_right<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in, n);
}

template <typename C> //
[[nodiscard]] constexpr auto drop_right(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  return details::sequence1::drop_right<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in, n);
}

template <typename C> //
[[nodiscard]] constexpr auto take(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  return details::sequence1::take<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in, n);
}

template <typename C> //
[[nodiscard]] constexpr auto drop(const ASPARTAME_IN_TYPE1(C) & in, size_t n) {
  return details::sequence1::drop<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C)>(in, n);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto take_while(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate) {
  return details::sequence1::take_while<ASPARTAME_IN_TYPE1(C), Predicate, ASPARTAME_IN_TYPE1(C)>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto drop_while(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate) {
  return details::sequence1::drop_while<ASPARTAME_IN_TYPE1(C), Predicate, ASPARTAME_IN_TYPE1(C)>(in, predicate);
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(const ASPARTAME_IN_TYPE1(C) & in, Accumulator &&init, Function &&function) {
  return details::sequence1::fold_left<ASPARTAME_IN_TYPE1(C), Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(const ASPARTAME_IN_TYPE1(C) & in, Accumulator &&init, Function &&function) {
  return details::sequence1::fold_right<ASPARTAME_IN_TYPE1(C), Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C> //
[[nodiscard]] constexpr auto sliding(const ASPARTAME_IN_TYPE1(C) & in, size_t size, size_t step) {
  return details::sequence1::sliding<ASPARTAME_IN_TYPE1(C), ASPARTAME_OUT_TYPE>(in, size, step);
}

} // namespace aspartame