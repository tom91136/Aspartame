#pragma once

#include <array>
#include <vector>

#include "details/base.hpp"
#include "details/container1_impl.hpp"
#include "details/sequence1_impl.hpp"
#include "fluent.hpp"

namespace aspartame {
namespace details {
template <typename> constexpr bool is_array_impl = false;
template <typename T, size_t N> constexpr bool is_array_impl<std::array<T, N>> = true;
} // namespace details
template <typename T> constexpr bool is_array = details::is_array_impl<std::decay_t<T>>;
template <typename T, size_t N, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::array<T, N> &, tag>
#endif
auto operator^(const std::array<T, N> &l, const Op &r) {
  return r(l, tag{});
}
} // namespace aspartame

namespace aspartame {

template <typename C, size_t N, typename Function> //
[[nodiscard]] auto mk_string(const std::array<C, N> &in, const std::string_view &prefix, const std::string_view &sep,
                             const std::string_view &suffix, Function &&f, tag = {}) {
  return details::container1::mk_string<std::array<C, N>, Function>(in, prefix, sep, suffix, f);
}

template <typename C, size_t N, typename T> //
[[nodiscard]] constexpr auto append(const std::array<C, N> &in, const T &t, tag = {}) {
  return details::container1::append<std::array<C, N>, T, std::vector<C>>(in, t);
}

template <typename C, size_t N, typename Container> //
[[nodiscard]] constexpr auto concat(const std::array<C, N> &in, const Container &other, tag = {}) {
  return details::container1::concat<std::array<C, N>, Container, std::vector<C>>(in, other);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto map(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::container1::map<std::array<C, N>, Function, std::vector>(in, function);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto collect(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::container1::collect<std::array<C, N>, Function, std::vector>(in, function);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto filter(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::container1::filter<std::array<C, N>, Predicate, std::vector>(in, predicate);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto flat_map(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::container1::flat_map<std::array<C, N>, Function, std::vector>(in, function);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto flatten(const std::array<C, N> &in, tag = {}) {
  return details::container1::flatten<std::array<C, N>, std::vector>(in);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto distinct_by(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::container1::distinct_by<std::array<C, N>, Function, std::vector<C>>(in, function);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto distinct(const std::array<C, N> &in, tag = {}) {
  return details::container1::distinct<std::array<C, N>, std::vector<C>, false>(in);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto count(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::container1::count<std::array<C, N>, Predicate>(in, predicate);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto exists(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::container1::exists<std::array<C, N>, Predicate>(in, predicate);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto forall(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::container1::forall<std::array<C, N>, Predicate>(in, predicate);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto find(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::container1::find<std::array<C, N>, Predicate>(in, predicate);
}

template <typename C, size_t N, typename T> //
[[nodiscard]] constexpr auto contains(const std::array<C, N> &in, const T &t, tag = {}) {
  return details::container1::contains<std::array<C, N>>(in, t);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto reduce(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::container1::reduce<std::array<C, N>, Function>(in, function);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto tap_each(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::container1::tap_each<std::array<C, N>, Function>(in, function);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto for_each(const std::array<C, N> &in, Function &&function, tag = {}) {
  details::container1::for_each<std::array<C, N>, Function>(in, function);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto partition(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::container1::partition<std::array<C, N>, Predicate, std::vector<C>>(in, predicate);
}

template <typename C, size_t N, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(const std::array<C, N> &in, GroupFunction &&group, MapFunction &&map, ReduceFunction &&reduce,
                                              tag = {}) {
  return details::container1::group_map_reduce<std::array<C, N>, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename C, size_t N, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(const std::array<C, N> &in, GroupFunction &&group, MapFunction &&map, tag = {}) {
  return details::container1::group_map<std::array<C, N>, GroupFunction, MapFunction, std::vector>(in, group, map);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto group_by(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::container1::group_by<std::array<C, N>, Function, std::vector>(in, function);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto to_vector(const std::array<C, N> &in, tag = {}) {
  return std::vector<C>{in.begin(), in.end()};
}

template <template <typename...> typename Cs, typename C, size_t N> //
[[nodiscard]] constexpr auto to(const std::array<C, N> &in, tag = {}) {
  if constexpr (is_unary_instantiable<Cs, C>) return Cs<C>{in.begin(), in.end()};
  else if constexpr (is_pair<C>) return Cs<typename C::first_type, typename C::second_type>{in.begin(), in.end()};
  else return Cs<C>{in.begin(), in.end()};
}

template <typename C, size_t N, typename T> //
[[nodiscard]] constexpr auto prepend(const std::array<C, N> &in, const T &t, tag = {}) {
  return details::sequence1::prepend<std::array<C, N>, T, std::vector<C>>(in, t);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto head_maybe(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::head_maybe<std::array<C, N>>(in);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto last_maybe(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::last_maybe<std::array<C, N>>(in);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto init(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::init<std::array<C, N>, std::vector<C>>(in);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto tail(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::tail<std::array<C, N>, std::vector<C>>(in);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto at_maybe(const std::array<C, N> &in, size_t idx, tag = {}) {
  return details::sequence1::at_maybe<std::array<C, N>>(in, idx);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto slice(const std::array<C, N> &in, size_t from_inclusive, size_t to_exclusive, tag = {}) {
  return details::sequence1::slice<std::array<C, N>, std::vector<C>>(in, from_inclusive, to_exclusive);
}

template <typename C, size_t N, typename Container> //
[[nodiscard]] constexpr auto index_of_slice(const std::array<C, N> &in, const Container &other, tag = {}) {
  return details::sequence1::index_of_slice<std::array<C, N>, Container>(in, other);
}

template <typename C, size_t N, typename Container> //
[[nodiscard]] constexpr auto contains_slice(const std::array<C, N> &in, const Container &other, tag = {}) {
  return details::sequence1::index_of_slice<std::array<C, N>, Container>(in, other) != -1;
}

template <typename C, size_t N, typename T> //
[[nodiscard]] constexpr auto index_of(const std::array<C, N> &in, const T &t, tag = {}) {
  return details::sequence1::index_of<std::array<C, N>>(in, t);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto find_last(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::find_last<std::array<C, N>, Predicate>(in, predicate);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto index_where(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::index_where<std::array<C, N>, Predicate>(in, predicate);
}

template <typename C, size_t N, typename Function> //
[[nodiscard]] constexpr auto collect_first(const std::array<C, N> &in, Function &&function, tag = {}) {
  return details::sequence1::collect_first<std::array<C, N>, Function>(in, function);
}

template <typename C, size_t N, typename Number> //
[[nodiscard]] constexpr auto zip_with_index(const std::array<C, N> &in, Number from, tag = {}) {
  return details::sequence1::zip_with_index<std::array<C, N>, std::vector, Number>(in, from);
}

template <typename C, size_t N, typename Container> //
[[nodiscard]] constexpr auto zip(const std::array<C, N> &in, const Container &other, tag = {}) {
  return details::sequence1::zip<std::array<C, N>, Container, std::vector>(in, other);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto transpose(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::transpose<std::array<C, N>, std::vector>(in);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto sequence(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::sequence<std::array<C, N>, std::vector>(in);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto reverse(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::reverse<std::array<C, N>, std::vector<C>>(in);
}

template <typename C, size_t N, typename URBG> //
[[nodiscard]] constexpr auto shuffle(const std::array<C, N> &in, URBG &&urbg, tag = {}) {
  return details::sequence1::shuffle<std::array<C, N>, URBG, std::vector<C>>(in, std::forward<URBG &&>(urbg));
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto sort(const std::array<C, N> &in, tag = {}) {
  return details::sequence1::sort<std::array<C, N>, std::vector<C>>(in);
}

template <typename C, size_t N, typename Compare> //
[[nodiscard]] constexpr auto sort(const std::array<C, N> &in, Compare &&compare, tag = {}) {
  return details::sequence1::sort<std::array<C, N>, Compare, std::vector<C>>(in, compare);
}

template <typename C, size_t N, typename Select> //
[[nodiscard]] constexpr auto sort_by(const std::array<C, N> &in, Select &&select, tag = {}) {
  return details::sequence1::sort_by<std::array<C, N>, Select, std::vector<C>>(in, select);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto split_at(const std::array<C, N> &in, size_t idx, tag = {}) {
  return details::sequence1::split_at<std::array<C, N>, std::vector<C>>(in, idx);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto take_right(const std::array<C, N> &in, size_t n, tag = {}) {
  return details::sequence1::take_right<std::array<C, N>, std::vector<C>>(in, n);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto drop_right(const std::array<C, N> &in, size_t n, tag = {}) {
  return details::sequence1::drop_right<std::array<C, N>, std::vector<C>>(in, n);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto take(const std::array<C, N> &in, size_t n, tag = {}) {
  return details::sequence1::take<std::array<C, N>, std::vector<C>>(in, n);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto drop(const std::array<C, N> &in, size_t n, tag = {}) {
  return details::sequence1::drop<std::array<C, N>, std::vector<C>>(in, n);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto take_while(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::take_while<std::array<C, N>, Predicate, std::vector<C>>(in, predicate);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto drop_while(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::drop_while<std::array<C, N>, Predicate, std::vector<C>>(in, predicate);
}

template <typename C, size_t N, typename Predicate> //
[[nodiscard]] constexpr auto span(const std::array<C, N> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::span<std::array<C, N>, Predicate, std::vector<C>>(in, predicate);
}

template <typename C, size_t N, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(const std::array<C, N> &in, Accumulator &&init, Function &&function, tag = {}) {
  return details::sequence1::fold_left<std::array<C, N>, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C, size_t N, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(const std::array<C, N> &in, Accumulator &&init, Function &&function, tag = {}) {
  return details::sequence1::fold_right<std::array<C, N>, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C, size_t N> //
[[nodiscard]] constexpr auto sliding(const std::array<C, N> &in, size_t size, size_t step, tag = {}) {
  return details::sequence1::sliding<std::array<C, N>, std::vector>(in, size, step);
}

} // namespace aspartame
