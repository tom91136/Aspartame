#include <algorithm>

#include "base.hpp"
#include "container1_impl.hpp"

#ifndef ASPARTAME_IN_TYPE1
  #error "ASPARTAME_IN_TYPE1 unimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename C, typename Function> //
[[nodiscard]] auto mk_string(const ASPARTAME_IN_TYPE1(C) & in, const std::string_view &prefix, const std::string_view &sep,
                             const std::string_view &suffix, Function &&f,  tag = {}) {
  return details::container1::mk_string<ASPARTAME_IN_TYPE1(C), Function>(in, prefix, sep, suffix, f);
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto append(const ASPARTAME_IN_TYPE1(C) & in, const T &t,  tag = {}) {
  return details::container1::append<ASPARTAME_IN_TYPE1(C), T, ASPARTAME_IN_TYPE1(C)>(in, t);
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto concat(const ASPARTAME_IN_TYPE1(C) & in, const Container &other,  tag = {}) {
  return details::container1::concat<ASPARTAME_IN_TYPE1(C), Container, ASPARTAME_IN_TYPE1(C)>(in, other);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto map(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  return details::container1::map<ASPARTAME_IN_TYPE1(C), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto collect(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  return details::container1::collect<ASPARTAME_IN_TYPE1(C), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto filter(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate,  tag = {}) {
  return details::container1::filter<ASPARTAME_IN_TYPE1(C), Predicate, ASPARTAME_OUT_TYPE>(in, predicate);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto flat_map(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  return details::container1::flat_map<ASPARTAME_IN_TYPE1(C), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename C> //
[[nodiscard]] constexpr auto flatten(const ASPARTAME_IN_TYPE1(C) & in,  tag = {}) {
  return details::container1::flatten<ASPARTAME_IN_TYPE1(C), ASPARTAME_OUT_TYPE>(in);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto distinct_by(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  return details::container1::distinct_by<ASPARTAME_IN_TYPE1(C), Function, ASPARTAME_IN_TYPE1(C)>(in, function);
}

template <typename C> //
[[nodiscard]] constexpr auto distinct(const ASPARTAME_IN_TYPE1(C) & in,  tag = {}) {
  return details::container1::distinct<ASPARTAME_IN_TYPE1(C), ASPARTAME_IN_TYPE1(C), ASPARTAME_SET_LIKE>(in);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto count(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate,  tag = {}) {
  return details::container1::count<ASPARTAME_IN_TYPE1(C), Predicate>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto exists(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate,  tag = {}) {
  return details::container1::exists<ASPARTAME_IN_TYPE1(C), Predicate>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto forall(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate,  tag = {}) {
  return details::container1::forall<ASPARTAME_IN_TYPE1(C), Predicate>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto find(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate,  tag = {}) {
  return details::container1::find<ASPARTAME_IN_TYPE1(C), Predicate>(in, predicate);
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto contains(const ASPARTAME_IN_TYPE1(C) & in, const T &t,  tag = {}) {
  return details::container1::contains<ASPARTAME_IN_TYPE1(C)>(in, t);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto reduce(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  return details::container1::reduce<ASPARTAME_IN_TYPE1(C), Function>(in, function);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto tap_each(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  return details::container1::tap_each<ASPARTAME_IN_TYPE1(C), Function>(in, function);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto for_each(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  details::container1::for_each<ASPARTAME_IN_TYPE1(C), Function>(in, function);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto partition(const ASPARTAME_IN_TYPE1(C) & in, Predicate &&predicate,  tag = {}) {
  return details::container1::partition<ASPARTAME_IN_TYPE1(C), Predicate, ASPARTAME_IN_TYPE1(C)>(in, predicate);
}

template <typename C, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(const ASPARTAME_IN_TYPE1(C) & in, GroupFunction &&group, MapFunction &&map,
                                              ReduceFunction &&reduce,  tag = {}) {
  return details::container1::group_map_reduce<ASPARTAME_IN_TYPE1(C), GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename C, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(const ASPARTAME_IN_TYPE1(C) & in, GroupFunction &&group, MapFunction &&map,  tag = {}) {
  return details::container1::group_map<ASPARTAME_IN_TYPE1(C), GroupFunction, MapFunction, ASPARTAME_OUT_TYPE>(in, group, map);
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto group_by(const ASPARTAME_IN_TYPE1(C) & in, Function &&function,  tag = {}) {
  return details::container1::group_by<ASPARTAME_IN_TYPE1(C), Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename C> //
[[nodiscard]] constexpr auto to_vector(const ASPARTAME_IN_TYPE1(C) & in,  tag = {}) {
  if constexpr (is_vector<ASPARTAME_IN_TYPE1(C)>) return in;
  else return std::vector<C>{in.begin(), in.end()};
}

template <template <typename...> typename Cs, typename C> //
[[nodiscard]] constexpr auto to(const ASPARTAME_IN_TYPE1(C) & in,  tag = {}) {
  if constexpr (is_unary_instantiable<Cs, C>) {
    if constexpr (std::is_same_v<ASPARTAME_IN_TYPE1(C), std::decay_t<Cs<C>>>) return in;
    else return Cs<C>{in.begin(), in.end()};
  } else if constexpr (is_pair<C>) {
    return Cs<typename C::first_type, typename C::second_type>{in.begin(), in.end()};
  } else {
    return Cs<C>{in.begin(), in.end()};
  }
}

} // namespace aspartame