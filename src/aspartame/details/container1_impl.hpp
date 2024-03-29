#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "base.hpp"

namespace aspartame::details::container1 {

template <typename In, typename Function> //
[[nodiscard]] auto mk_string(const In &o, const std::string_view &prefix, const std::string_view &sep, const std::string_view &suffix,
                             Function f) {
  static_assert(std::is_convertible_v<decltype(details::ap(f, *o.begin())), std::string>, "function does not return a string type");
  std::string out{prefix};
  auto it = o.begin();
  auto end = o.end();
  if (it != end) {
    out += details::ap(f, *it);
    ++it;
  }
  while (it != end) {
    out += sep;
    out += details::ap(f, *it);
    ++it;
  }
  out += suffix;
  return out;
}

template <typename In, typename T, typename Out> //
[[nodiscard]] constexpr Out append(const In &in, const T &t) {
  auto ys = Out{in.begin(), in.end()};
  ys.insert(ys.end(), t);
  return ys;
}

template <typename In, typename Container, typename Out> //
[[nodiscard]] constexpr Out concat(const In &in, const Container &container) {
  Out ys = Out{in.begin(), in.end()};
  if constexpr (has_associative_insert<Out>) ys.insert(container.begin(), container.end());
  else ys.insert(ys.end(), container.begin(), container.end());
  return ys;
}

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto map(const In &in, Function f) {
  using T = decltype(details::ap(f, *in.begin()));
  if constexpr (details::assert_non_void<T>()) {}
  if constexpr (std::is_default_constructible_v<T> && has_assignable_iterator<typename Out<T>::iterator>) {
    Out<T> ys(in.size());
    std::transform(in.begin(), in.end(), ys.begin(), [&](auto &&x) { return details::ap(f, x); });
    return ys;
  } else {
    Out<T> ys;
    if constexpr (has_reserve<Out<T>>) ys.reserve(in.size());
    const auto map = [&](auto &&x) { return details::ap(f, x); };
    if constexpr (has_push_back<Out<T>>) //
      std::transform(in.begin(), in.end(), std::back_inserter(ys), map);
    else std::transform(in.begin(), in.end(), std::inserter(ys, ys.begin()), map);
    return ys;
  }
}

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto collect(const In &in, Function f) {
  using T = decltype(details::ap(f, *in.begin()));
  static_assert(is_optional<T>, "collect function should return an optional");
  Out<typename T::value_type> ys;
  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
    if (auto y = details::ap(f, *it); y) ys.insert(ys.end(), *y);
  }
  return ys;
}

template <typename In, typename Predicate, template <typename...> typename Out> //
[[nodiscard]] constexpr auto filter(const In &in, Predicate p) {
  using T = typename In::value_type;
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  Out<T> ys;
  const auto filter = [&](auto &&x) { return details::ap(p, x); };
  if constexpr (has_push_back<Out<T>>) //
    std::copy_if(in.begin(), in.end(), std::back_inserter(ys), filter);
  else std::copy_if(in.begin(), in.end(), std::inserter(ys, ys.begin()), filter);
  return ys;
}

template <typename In, typename Function, template <typename...> typename Out> //
[[nodiscard]] constexpr auto bind(const In &in, Function f) {
  static_assert(is_iterable<decltype(details::ap(f, *in.begin()))>, "bind function should return an iterable type");
  using T = typename decltype(details::ap(f, *in.begin()))::value_type;
  Out<T> ys;
  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
    auto zs = details::ap(f, *it);
    if constexpr (has_associative_insert<Out<T>>) ys.insert(zs.begin(), zs.end());
    else ys.insert(ys.end(), zs.begin(), zs.end());
  }
  return ys;
}

template <typename In, template <typename...> typename Out> //
[[nodiscard]] constexpr auto flatten(const In &in) {
  static_assert(is_iterable<typename In::value_type>, "not a nested type that is iterable");
  using T = typename In::value_type::value_type;
  Out<T> ys;
  for (auto &&x : in)
    if constexpr (has_associative_insert<Out<T>>) ys.insert(x.begin(), x.end());
    else ys.insert(ys.end(), x.begin(), x.end());
  return ys;
}

template <typename In, typename Function, typename Out> //
[[nodiscard]] constexpr auto distinct_by(const In &in, Function f) {
  using T = typename In::value_type;
  using V = decltype(details::ap(f, *in.begin()));
  static_assert(is_hashable<V>, "distinct type must be hashable");
  static_assert(is_comparable<V>, "distinct type must be comparable");

  if (in.size() == 0) return Out{};
  if (in.size() == 1) return Out{*in.begin()};
  const auto hash = [&](const auto &x) { return std::hash<V>()(details::ap(f, x)); };
  const auto eq = [&](const auto &l, const auto &r) { return details::ap(f, l) == details::ap(f, r); };
  std::unordered_set<T, decltype(hash), decltype(eq)> set{0, hash, eq};
  auto unique = [&](auto &&x) { return set.insert(x).second; };
  Out ys;
  if constexpr (has_push_back<Out>) //
    std::copy_if(in.begin(), in.end(), std::back_inserter(ys), unique);
  else std::copy_if(in.begin(), in.end(), std::inserter(ys, ys.begin()), unique);
  return ys;
}

template <typename In, typename Out, bool set_like> //
[[nodiscard]] constexpr auto distinct(const In &in) {
  if constexpr (set_like) return in;
  else {
    auto id = [](auto &&x) { return x; };
    return distinct_by<In, decltype(id), Out>(in, id);
  }
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr size_t count(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  return std::count_if(in.begin(), in.end(), [&](auto &&x) { return details::ap(p, x); });
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr bool exists(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  return std::any_of(in.begin(), in.end(), [&](auto &&x) { return details::ap(p, x); });
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr bool forall(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  return std::all_of(in.begin(), in.end(), [&](auto &&x) { return details::ap(p, x); });
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto find(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  auto it = std::find_if(in.begin(), in.end(), [&](auto &&x) { return details::ap(p, x); });
  if (it == in.end()) return std::optional<typename In::value_type>{std::nullopt};
  else return std::optional<typename In::value_type>{*it};
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto contains(const In &in, const T &t) {
  if constexpr (is_set_like_impl<In>) return in.find(t) != in.end();
  else return std::find(in.begin(), in.end(), t) != in.end();
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto reduce(const In &in, Function f) {
  using T = typename In::value_type;
  static_assert(std::is_invocable_v<Function, T, T>, "function must be invocable with two value types of the container");
  static_assert(std::is_convertible_v<std::invoke_result_t<Function, T, T>, T>,
                "function must return something that is convertable to value type");
  auto it = in.begin();
  if (it == in.end()) return std::optional<T>(std::nullopt);
  T first = *it;
  ++it;
  for (; it != in.end(); ++it)
    first = f(std::move(first), *it);
  return std::optional<T>{first};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr In tap_each(const In &in, Function f) {
  if constexpr (details::assert_void<decltype(details::ap(f, *in.begin()))>()) {}
  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
    details::ap(f, *it);
  }
  return in;
}

template <typename In, typename Function> //
constexpr void for_each(const In &in, Function f) {
  if constexpr (details::assert_void<decltype(details::ap(f, *in.begin()))>()) {}
  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
    details::ap(f, *it);
  }
}

template <typename In, typename Predicate, typename Out> //
[[nodiscard]] constexpr std::pair<Out, Out> partition(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  Out xs, ys;
  for (const auto &x : in) {
    if (details::ap(p, x)) xs.insert(xs.end(), x);
    else ys.insert(ys.end(), x);
  }
  return {xs, ys};
}

template <typename In, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(const In &in, GroupFunction &&group, MapFunction &&map, ReduceFunction &&reduce) {
  using K = decltype(details::ap(group, *in.begin()));
  using V = decltype(details::ap(map, *in.begin()));
  using R = std::invoke_result_t<ReduceFunction, V, V>;
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  if constexpr (details::assert_non_void<R>()) {}
  std::unordered_map<K, R> ys;
  for (const auto &x : in) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) it->second = reduce(it->second, details::ap(map, x));
    else ys.emplace(k, details::ap(map, x));
  }
  return ys;
}

template <typename In, typename GroupFunction, typename MapFunction, template <typename...> typename Out> //
[[nodiscard]] constexpr auto group_map(const In &in, GroupFunction &&group, MapFunction &&map) {
  using K = decltype(details::ap(group, *in.begin()));
  using V = decltype(details::ap(map, *in.begin()));
  if constexpr (details::assert_non_void<K>() && details::assert_non_void<V>()) {}
  std::unordered_map<K, Out<V>> ys;
  for (const auto &x : in) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) it->second.insert(it->second.end(), details::ap(map, x));
    else ys.emplace(k, Out<V>{details::ap(map, x)});
  }
  return ys;
}

template <typename In, typename GroupFunction, template <typename...> typename Out> //
[[nodiscard]] constexpr auto group_by(const In &in, GroupFunction &&group) {
  using K = decltype(details::ap(group, *in.begin()));
  if constexpr (details::assert_non_void<K>()) {}

  using V = typename In::value_type;
  std::unordered_map<K, Out<V>> ys;
  for (const auto &x : in) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) it->second.insert(it->second.end(), x);
    else ys.emplace(k, Out<V>{x});
  }
  return ys;
}

} // namespace aspartame::details::container1