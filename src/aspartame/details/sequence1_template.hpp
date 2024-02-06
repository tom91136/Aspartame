#include <algorithm>

#include "base.hpp"
#include "sequence1_impl.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(prepend)(const In &in, const T &t) {
  return details::sequence::prepend<In, T, In>(in, t);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(head_maybe)(const In &in) {
  return details::sequence::head_maybe<In>(in);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(last_maybe)(const In &in) {
  return details::sequence::last_maybe<In>(in);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(init)(const In &in) {
  return details::sequence::init<In, In>(in);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(tail)(const In &in) {
  return details::sequence::tail<In, In>(in);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(at_maybe)(const In &in, size_t idx) {
  return details::sequence::at_maybe<In>(in, idx);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(slice)(const In &in, size_t from_inclusive, size_t to_exclusive) {
  return details::sequence::slice<In, In>(in, from_inclusive, to_exclusive);
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_of_slice)(const In &in, const Container &other) {
  return details::sequence::index_of_slice<In, Container>(in, other);
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(contains_slice)(const In &in, const Container &other) {
  return details::sequence::index_of_slice<In, Container>(in, other) != -1;
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_of)(const In &in, const T &t) {
  return details::sequence::index_of<In>(in, t);
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(contains)(const In &in, const T &t) {
  return details::sequence::index_of<In>(in, t) != -1;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_where)(const In &in, Predicate &&predicate) {
  return details::sequence::index_where<In, Predicate>(in, predicate);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(zip_with_index)(const In &in) {
  return details::sequence::zip_with_index<In, ASPARTAME_OUT_TYPE>(in);
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(zip)(const In &in, const Container &other) {
  return details::sequence::zip<In, Container, ASPARTAME_OUT_TYPE>(in, other);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(transpose)(const In &in) {
  return details::sequence::transpose<In, ASPARTAME_OUT_TYPE>(in);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(reverse)(const In &in) {
  return details::sequence::reverse<In, In>(in);
}

template <typename In, typename URBG> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(shuffle)(const In &in, URBG &&urbg) {
  return details::sequence::shuffle<In, URBG, In>(in, std::forward<URBG &&>(urbg));
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort)(const In &in) {
  return details::sequence::sort<In, In>(in);
}

template <typename In, typename Compare> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort)(const In &in, Compare &&compare) {
  return details::sequence::sort<In, Compare, In>(in, compare);
}

template <typename In, typename Select> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort_by)(const In &in, Select &&select) {
  return details::sequence::sort_by<In, Select, In>(in, select);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(split_at)(const In &in, size_t idx) {
  return details::sequence::split_at<In, In>(in, idx);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take_right)(const In &in, size_t n) {
  return details::sequence::take_right<In, In>(in, n);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop_right)(const In &in, size_t n) {
  return details::sequence::drop_right<In, In>(in, n);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take)(const In &in, size_t n) {
  return details::sequence::take<In, In>(in, n);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop)(const In &in, size_t n) {
  return details::sequence::drop<In, In>(in, n);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take_while)(const In &in, Predicate &&predicate) {
  return details::sequence::take_while<In, Predicate, In>(in, predicate);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop_while)(const In &in, Predicate &&predicate) {
  return details::sequence::drop_while<In, Predicate, In>(in, predicate);
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold_left)(const In &in, Accumulator &&init, Function &&function) {
  return details::sequence::fold_left<In, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold_right)(const In &in, Accumulator &&init, Function &&function) {
  return details::sequence::fold_right<In, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sliding)(const In &in, size_t size, size_t step) {
  return details::sequence::sliding<In, ASPARTAME_OUT_TYPE>(in, size, step);
}

} // namespace aspartame