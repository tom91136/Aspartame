#include <algorithm>

#include "base.hpp"
#include "container1_impl.hpp"
#include "container2_impl.hpp"

#ifndef ASPARTAME_PREFIX
  #error "ASPARTAME_PREFIX unimplemented"
#endif

#ifndef ASPARTAME_OUT_TYPE
  #error "ASPARTAME_OUT_TYPE unimplemented"
#endif

namespace aspartame {

template <typename In, typename Function> //
auto ASPARTAME_PREFIX(mk_string)(const In &in, const std::string_view &sep, const std::string_view &prefix, const std::string_view &suffix,
                                 Function &&f) {
  return details::container::mk_string(in, sep, prefix, suffix, f);
}

template <typename In, typename T> //
constexpr auto ASPARTAME_PREFIX(append)(const In &in, const T &t) {
  return details::container::append<In, T, In>(in, t);
}

template <typename In, typename Container> //
constexpr auto ASPARTAME_PREFIX(concat)(const In &in, const Container &other) {
  return details::container::concat<In, Container, In>(in, other);
}

template <typename In, typename Function> //
constexpr auto ASPARTAME_PREFIX(map)(const In &in, Function &&f) {
  return details::container2::map<In, Function, ASPARTAME_OUT_TYPE>(in, f);
}

template <typename In, typename Function> //
constexpr auto ASPARTAME_PREFIX(collect)(const In &in, Function &&function) {
  return details::container2::collect<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In, typename Predicate> //
constexpr auto ASPARTAME_PREFIX(filter)(const In &in, Predicate &&predicate) {
  return details::container2::filter<In, Predicate, ASPARTAME_OUT_TYPE>(in, predicate);
}

template <typename In, typename Function> //
constexpr auto ASPARTAME_PREFIX(bind)(const In &in, Function &&function) {
  return details::container2::bind<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In> //
constexpr auto ASPARTAME_PREFIX(flatten)(const In &in) {
  return details::container2::flatten<In, ASPARTAME_OUT_TYPE>(in);
}

template <typename In, typename Predicate> //
constexpr auto ASPARTAME_PREFIX(exists)(const In &in, Predicate &&predicate) {
  return details::container::exists<In, Predicate>(in, predicate);
}

template <typename In, typename Predicate> //
constexpr auto ASPARTAME_PREFIX(forall)(const In &in, Predicate &&predicate) {
  return details::container::forall<In, Predicate>(in, predicate);
}

template <typename In, typename Function> //
constexpr auto ASPARTAME_PREFIX(reduce)(const In &in, Function &&function) {
  return details::container2::reduce<In, Function>(in, function);
}

template <typename In, typename Function> //
constexpr auto ASPARTAME_PREFIX(tap_each)(const In &in, Function &&function) {
  return details::container::tap_each<In, Function>(in, function);
}

template <typename In, typename Function> //
constexpr auto ASPARTAME_PREFIX(for_each)(const In &in, Function &&function) {
  details::container::for_each<In, Function>(in, function);
}

template <typename In, typename Predicate> //
constexpr auto ASPARTAME_PREFIX(partition)(const In &in, Predicate &&predicate) {
  return details::container::partition<In, Predicate, In>(in, predicate);
}

template <typename In, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
constexpr auto ASPARTAME_PREFIX(group_map_reduce)(const In &in, GroupFunction &&group, MapFunction &&map, ReduceFunction &&reduce) {
  return details::container::group_map_reduce<In, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename In, typename GroupFunction, typename MapFunction> //
constexpr auto ASPARTAME_PREFIX(group_map)(const In &in, GroupFunction &&group, MapFunction &&map) {
  return details::container2::group_map<In, GroupFunction, MapFunction, ASPARTAME_OUT_TYPE_C1>(in, group, map);
}

template <typename In, typename Function> //
constexpr auto ASPARTAME_PREFIX(group_by)(const In &in, Function &&function) {
  return details::container2::group_by<In, Function, ASPARTAME_OUT_TYPE_C1>(in, function);
}

template <typename In> //
constexpr auto ASPARTAME_PREFIX(to_vector)(const In &in) {
  using T = typename In::value_type;
  using U = std::pair<std::remove_const_t<typename T::first_type>, typename T::second_type>;
  return std::vector<U>{in.begin(), in.end()};
}

} // namespace aspartame