#pragma once

#include "base.hpp"
#include "container1_impl.hpp"

#include "iterators/append_prepend.hpp"
#include "iterators/bind.hpp"
#include "iterators/concat.hpp"
#include "iterators/filter.hpp"
#include "iterators/map.hpp"

namespace aspartame {

template <typename In, typename Function> //
[[nodiscard]] auto ASPARTAME_PREFIX(mk_string)(const In &in, const std::string_view &sep, const std::string_view &prefix,
                                               const std::string_view &suffix, Function &&f) {
  return details::container::mk_string<In, Function>(in, sep, prefix, suffix, f);
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(append)(const In &in, const T &t) {
  return view(details::append_prepend_iterator<typename In::const_iterator>( //
      in.begin(), in.end(), details::append_prepend_iterator_mode::append, t));
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(concat)(const In &in, Container &&other) {
  return view(details::concat_iterator<typename In::const_iterator,                     //
                                       typename std::decay_t<Container>::const_iterator //
                                       >(in.begin(), in.end(), other.begin(), other.end()));
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(map)(const In &in, Function &&function) {
  auto applied = [&](auto x) { return details::ap(function, x); };
  return view(details::map_iterator<typename In::const_iterator, decltype(applied)>(in.begin(), in.end(), applied));
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(collect)(const In &in, Function &&function) {
  details::unsupported<In>();
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(filter)(const In &in, Predicate &&predicate) {
  auto applied = [&](auto x) { return details::ap(predicate, x); };
  return view(details::filter_iterator<typename In::const_iterator, decltype(applied)>(in.begin(), in.end(), applied));
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(bind)(const In &in, Function &&function) {
  auto applied = [&](auto x) { return details::ap(function, x); };
  return view(details::bind_iterator<typename In::const_iterator, decltype(applied)>(in.begin(), in.end(), applied));
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(flatten)(const In &in) {
  auto identity = [](auto x) { return x; };
  return view(details::bind_iterator<typename In::const_iterator, decltype(identity)>(in.begin(), in.end(), identity));
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(distinct_by)(const In &in, Function &&function) {
  details::unsupported<In>();
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(distinct)(const In &in) {
  details::unsupported<In>();
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
  return details::container::tap_each(in, function);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(for_each)(const In &in, Function &&function) {
  details::container::for_each<In, Function>(in, function);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(partition)(const In &in, Predicate &&predicate) {
  details::unsupported<In>();
}

template <typename In, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_map_reduce)(const In &in, GroupFunction &&group, MapFunction &&map,
                                                                ReduceFunction &&reduce) {
  return details::container::group_map_reduce<In, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename In, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_map)(const In &in, GroupFunction &&group, MapFunction &&map) {
  return details::container::group_map<In, GroupFunction, MapFunction, std::vector>(in, group, map);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_by)(const In &in, Function &&function) {
  return details::container::group_by<In, Function, std::vector>(in, function);
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(to_vector)(const In &in) {
  if constexpr (is_vector<In>) return in;
  else return std::vector<typename In::value_type>{in.begin(), in.end()};
}

} // namespace aspartame
