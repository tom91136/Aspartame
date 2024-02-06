#include <algorithm>

#include "base.hpp"
#include "container1_impl.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename In, typename Function> //
[[nodiscard]] auto ASPARTAME_PREFIX(mk_string)(const In &in, const std::string_view &sep, const std::string_view &prefix,
                                               const std::string_view &suffix, Function &&f) {
  return details::container::mk_string(in, sep, prefix, suffix, f);
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(append)(const In &in, T &&t) {
  return details::container::append<In, T, In>(in, std::forward<T &&>(t));
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(concat)(const In &in, Container &&other) {
  return details::container::concat<In, Container, In>(in, std::forward<Container &&>(other));
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(map)(const In &in, Function &&function) {
  return details::container::map<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(collect)(const In &in, Function &&function) {
  return details::container::collect<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(filter)(const In &in, Predicate &&predicate) {
  return details::container::filter<In, Predicate, ASPARTAME_OUT_TYPE>(in, predicate);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(bind)(const In &in, Function &&function) {
  return details::container::bind<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(flatten)(const In &in) {
  return details::container::flatten<In, ASPARTAME_OUT_TYPE>(in);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(distinct_by)(const In &in, Function &&function) {
  return details::container::distinct_by<In, Function, In>(in, function);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(distinct)(const In &in) {
  return details::container::distinct<In, In, ASPARTAME_SET_LIKE>(in);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(count)(const In &in, Predicate &&predicate) {
  return details::container::count<In, Predicate>(in, predicate);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(exists)(const In &in, Predicate &&predicate) {
  return details::container::exists<In, Predicate>(in, predicate);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(forall)(const In &in, Predicate &&predicate) {
  return details::container::forall<In, Predicate>(in, predicate);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(reduce)(const In &in, Function &&function) {
  return details::container::reduce<In, Function>(in, function);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(tap_each)(const In &in, Function &&function) {
  return details::container::tap_each<In, Function>(in, function);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(for_each)(const In &in, Function &&function) {
  details::container::for_each<In, Function>(in, function);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(partition)(const In &in, Predicate &&predicate) {
  return details::container::partition<In, Predicate, In>(in, predicate);
}

template <typename In, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_map_reduce)(const In &in, GroupFunction &&group, MapFunction &&map,
                                                                ReduceFunction &&reduce) {
  return details::container::group_map_reduce<In, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename In, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_map)(const In &in, GroupFunction &&group, MapFunction &&map) {
  return details::container::group_map<In, GroupFunction, MapFunction, ASPARTAME_OUT_TYPE>(in, group, map);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_by)(const In &in, Function &&function) {
  return details::container::group_by<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(to_vector)(const In &in) {
  if constexpr (is_vector<In>) return in;
  else return std::vector<typename In::value_type>{in.begin(), in.end()};
}

} // namespace aspartame