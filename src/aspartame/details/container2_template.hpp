#include <algorithm>

#include "base.hpp"
#include "container1_impl.hpp"
#include "container2_impl.hpp"

#ifndef ASPARTAME_IN_TYPE2
  #error "ASPARTAME_IN_TYPE2 K, Vnimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename K, typename V, typename Function> //
[[nodiscard]] auto mk_string(const ASPARTAME_IN_TYPE2(K, V) & in, const std::string_view &prefix, const std::string_view &sep,
                             const std::string_view &suffix, Function &&f) {
  return details::container1::mk_string(in, prefix, sep, suffix, f);
}

template <typename K, typename V, typename T> //
[[nodiscard]] constexpr auto append(const ASPARTAME_IN_TYPE2(K, V) & in, const T &t) {
  return details::container1::append<ASPARTAME_IN_TYPE2(K, V), T, ASPARTAME_IN_TYPE2(K, V)>(in, t);
}

template <typename K, typename V, typename Container> //
[[nodiscard]] constexpr auto concat(const ASPARTAME_IN_TYPE2(K, V) & in, const Container &other) {
  return details::container1::concat<ASPARTAME_IN_TYPE2(K, V), Container, ASPARTAME_IN_TYPE2(K, V)>(in, other);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto map(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&f) {
  return details::container2::map<ASPARTAME_IN_TYPE2(K, V), Function, ASPARTAME_OUT_TYPE>(in, f);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto collect(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  return details::container2::collect<ASPARTAME_IN_TYPE2(K, V), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename K, typename V, typename Predicate> //
[[nodiscard]] constexpr auto filter(const ASPARTAME_IN_TYPE2(K, V) & in, Predicate &&predicate) {
  return details::container2::filter<ASPARTAME_IN_TYPE2(K, V), Predicate, ASPARTAME_OUT_TYPE>(in, predicate);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto bind(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  return details::container2::bind<ASPARTAME_IN_TYPE2(K, V), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename K, typename V> //
[[nodiscard]] constexpr auto flatten(const ASPARTAME_IN_TYPE2(K, V) & in) {
  return details::container2::flatten<ASPARTAME_IN_TYPE2(K, V), ASPARTAME_OUT_TYPE>(in);
}

template <typename K, typename V, typename Predicate> //
[[nodiscard]] constexpr auto exists(const ASPARTAME_IN_TYPE2(K, V) & in, Predicate &&predicate) {
  return details::container1::exists<ASPARTAME_IN_TYPE2(K, V), Predicate>(in, predicate);
}

template <typename K, typename V, typename Predicate> //
[[nodiscard]] constexpr auto forall(const ASPARTAME_IN_TYPE2(K, V) & in, Predicate &&predicate) {
  return details::container1::forall<ASPARTAME_IN_TYPE2(K, V), Predicate>(in, predicate);
}

template <typename K, typename V, typename Predicate> //
[[nodiscard]] constexpr auto find(const ASPARTAME_IN_TYPE2(K, V) & in, Predicate &&predicate) {
  return details::container2::find<ASPARTAME_IN_TYPE2(K, V), Predicate>(in, predicate);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto reduce(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  return details::container2::reduce<ASPARTAME_IN_TYPE2(K, V), Function>(in, function);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto tap_each(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  return details::container1::tap_each<ASPARTAME_IN_TYPE2(K, V), Function>(in, function);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto for_each(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  details::container1::for_each<ASPARTAME_IN_TYPE2(K, V), Function>(in, function);
}

template <typename K, typename V, typename Predicate> //
[[nodiscard]] constexpr auto partition(const ASPARTAME_IN_TYPE2(K, V) & in, Predicate &&predicate) {
  return details::container1::partition<ASPARTAME_IN_TYPE2(K, V), Predicate, ASPARTAME_IN_TYPE2(K, V)>(in, predicate);
}

template <typename K, typename V, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(const ASPARTAME_IN_TYPE2(K, V) & in, GroupFunction &&group, MapFunction &&map,
                                              ReduceFunction &&reduce) {
  return details::container1::group_map_reduce<ASPARTAME_IN_TYPE2(K, V), GroupFunction, MapFunction, ReduceFunction>(in, group, map,
                                                                                                                     reduce);
}

template <typename K, typename V, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(const ASPARTAME_IN_TYPE2(K, V) & in, GroupFunction &&group, MapFunction &&map) {
  return details::container2::group_map<ASPARTAME_IN_TYPE2(K, V), GroupFunction, MapFunction, ASPARTAME_OUT_TYPE_C1>(in, group, map);
}

template <typename K, typename V, typename Function> //
[[nodiscard]] constexpr auto group_by(const ASPARTAME_IN_TYPE2(K, V) & in, Function &&function) {
  return details::container2::group_by<ASPARTAME_IN_TYPE2(K, V), Function, ASPARTAME_OUT_TYPE_C1>(in, function);
}

template <typename K, typename V> //
[[nodiscard]] constexpr auto to_vector(const ASPARTAME_IN_TYPE2(K, V) & in) {
  return std::vector<std::pair<K, V>>{in.begin(), in.end()};
}

} // namespace aspartame