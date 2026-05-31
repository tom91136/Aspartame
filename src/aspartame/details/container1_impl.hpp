#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../traits.hpp"

namespace aspartame::details::container1 {

template <typename In, typename Function>                                              //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto mk_string(const In &o,                    //
                                                       const std::string_view &prefix, //
                                                       const std::string_view &sep,    //
                                                       const std::string_view &suffix, //
                                                       Function f) {
  static_assert(std::is_convertible_v<decltype(details::ap(f, *o.begin())), std::string>, "function does not return a string type");
  std::string out;
  if constexpr (has_size<In>) {
    const size_t n = o.size();
    out.reserve(prefix.size() + suffix.size() + (n > 0 ? sep.size() * (n - 1) : 0));
  }
  out += prefix;
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

template <typename In, typename Out, typename T> //
[[nodiscard]] constexpr Out append(const In &in, const T &t) {
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(in.size() + 1);
  for (auto &&x : seq_view(in))
    push(ys, x);
  push(ys, t);
  return ys;
}

template <typename In, typename Out, typename Container> //
[[nodiscard]] constexpr Out concat(const In &in, const Container &container) {
  Out ys;
  if constexpr (has_reserve<Out>) {
    if constexpr (has_size<Container>) ys.reserve(in.size() + container.size());
    else ys.reserve(in.size());
  }
  for (auto &&x : seq_view(in))
    push(ys, x);
  for (auto &&x : container)
    push(ys, x);
  return ys;
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto map(const In &in, Function f) {
  using T = decltype(details::ap(f, *seq_view(in).begin()));
  if constexpr (details::assert_non_void<T>()) {}
  if constexpr (has_tuple_size<Out> && !is_set_like_impl<Out>) {
    Out ys{};
    std::transform(seq_view(in).begin(), seq_view(in).end(), ys.begin(), [&](auto &&x) { return details::ap(f, x); });
    return ys;
  } else if constexpr (std::is_default_constructible_v<T> && std::is_constructible_v<Out, size_t> && !is_set_like_impl<Out> &&
                       has_size<In> && has_assignable_iterator<typename Out::iterator>) {
    Out ys(in.size());
    std::transform(seq_view(in).begin(), seq_view(in).end(), ys.begin(), [&](auto &&x) { return details::ap(f, x); });
    return ys;
  } else {
    Out ys;
    if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
    for (auto &&x : seq_view(in))
      push(ys, details::ap(f, x));
    return ys;
  }
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto collect(const In &in, Function f) {
  using T = decltype(details::ap(f, *seq_view(in).begin()));
  static_assert(is_optional<T>, "collect function should return an optional");
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
    if (auto y = details::ap(f, *it); y) push(ys, *y);
  }
  return ys;
}

template <typename In, typename Out, typename Predicate> //
[[nodiscard]] constexpr auto filter(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
  for (auto &&x : seq_view(in))
    if (details::ap(p, x)) push(ys, x);
  return ys;
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] constexpr auto flat_map(const In &in, Function f) {
  static_assert(is_iterable<decltype(details::ap(f, *seq_view(in).begin()))>, "flat_map function should return an iterable type");
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
    auto zs = details::ap(f, *it);
    for (auto &&z : zs)
      push(ys, z);
  }
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr auto flatten(const In &in) {
  static_assert(is_iterable<typename In::value_type>, "not a nested type that is iterable");
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
  for (auto &&x : seq_view(in))
    for (auto &&y : x)
      push(ys, y);
  return ys;
}

template <typename In, typename Out, typename Function> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto distinct_by(const In &in, Function f) {
  using V = std::decay_t<decltype(details::ap(f, *seq_view(in).begin()))>;
  static_assert(is_hashable<V>, "distinct type must be hashable");
  static_assert(is_comparable<V>, "distinct type must be comparable");

  std::unordered_set<V> seen;
  if constexpr (has_size<In>) {
    if (in.size() == 0) return Out{};
    if (in.size() == 1) return Out{*seq_view(in).begin()};
    seen.reserve(in.size());
  }
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
  for (auto &&x : seq_view(in))
    if (seen.insert(details::ap(f, x)).second) push(ys, x);
  return ys;
}

template <typename In, typename Out, bool set_like> //
[[nodiscard]] constexpr auto distinct(const In &in) {
  if constexpr (set_like) return in;
  else {
    auto id = [](auto &&x) { return x; };
    return distinct_by<In, Out, decltype(id)>(in, id);
  }
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr size_t count(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  return std::count_if(seq_view(in).begin(), seq_view(in).end(), [&](auto &&x) { return details::ap(p, x); });
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr bool exists(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  return std::any_of(seq_view(in).begin(), seq_view(in).end(), [&](auto &&x) { return details::ap(p, x); });
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr bool forall(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  return std::all_of(seq_view(in).begin(), seq_view(in).end(), [&](auto &&x) { return details::ap(p, x); });
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto find(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  auto it = std::find_if(seq_view(in).begin(), seq_view(in).end(), [&](auto &&x) { return details::ap(p, x); });
  if (it == seq_view(in).end()) return std::optional<typename In::value_type>{std::nullopt};
  else return std::optional<typename In::value_type>{*it};
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto contains(const In &in, const T &t) {
  if constexpr (is_set_like_impl<In>) {
    if constexpr (has_contains<In, T>) return in.contains(t);
    else if constexpr (has_find<In, T>) return in.find(t) != seq_view(in).end();
    else if constexpr (has_count<In, T>) return in.count(t) != 0;
    else return std::find(seq_view(in).begin(), seq_view(in).end(), t) != seq_view(in).end();
  } else return std::find(seq_view(in).begin(), seq_view(in).end(), t) != seq_view(in).end();
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto reduce(const In &in, Function f) {
  using T = typename In::value_type;
  static_assert(std::is_invocable_v<Function, T, T>, "function must be invocable with two value types of the container");
  static_assert(std::is_convertible_v<std::invoke_result_t<Function, T, T>, T>,
                "function must return something that is convertable to value type");
  auto it = seq_view(in).begin();
  if (it == seq_view(in).end()) return std::optional<T>(std::nullopt);
  T first = *it;
  ++it;
  for (; it != seq_view(in).end(); ++it)
    first = f(std::move(first), *it);
  return std::optional<T>{std::move(first)};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr In tap_each(const In &in, Function f) {
  if constexpr (details::assert_void<decltype(details::ap(f, *seq_view(in).begin()))>()) {}
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
    details::ap(f, *it);
  }
  return in;
}

template <typename In, typename Function> //
constexpr void for_each(const In &in, Function f) {
  if constexpr (details::assert_void<decltype(details::ap(f, *seq_view(in).begin()))>()) {}
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
    details::ap(f, *it);
  }
}

template <typename In, typename Out, typename Predicate> //
[[nodiscard]] constexpr std::pair<Out, Out> partition(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  Out xs, ys;
  if constexpr (has_reserve<Out> && has_size<In>) {
    xs.reserve(in.size());
    ys.reserve(in.size());
  }
  for (const auto &x : seq_view(in)) {
    if (details::ap(p, x)) push(xs, x);
    else push(ys, x);
  }
  return {xs, ys};
}

template <typename In, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_map_reduce(const In &in,                   //
                                                              GroupFunction &&group,          //
                                                              MapFunction &&map,              //
                                                              ReduceFunction &&reduce) {
  using K = decltype(details::ap(group, *seq_view(in).begin()));
  using V = decltype(details::ap(map, *seq_view(in).begin()));
  using R = std::invoke_result_t<ReduceFunction, V, V>;
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  if constexpr (details::assert_non_void<R>()) {}
  std::unordered_map<K, R> ys;
  for (const auto &x : seq_view(in)) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) it->second = reduce(it->second, details::ap(map, x));
    else ys.emplace(k, details::ap(map, x));
  }
  return ys;
}

template <typename In, typename Inner, typename GroupFunction, typename MapFunction> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_map(const In &in, GroupFunction &&group, MapFunction &&map) {
  using K = decltype(details::ap(group, *seq_view(in).begin()));
  using V = decltype(details::ap(map, *seq_view(in).begin()));
  if constexpr (details::assert_non_void<K>() && details::assert_non_void<V>()) {}
  std::unordered_map<K, Inner> ys;
  for (const auto &x : seq_view(in)) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) push(it->second, details::ap(map, x));
    else ys.emplace(k, Inner{details::ap(map, x)});
  }
  return ys;
}

template <typename In, typename Inner, typename GroupFunction> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_by(const In &in, GroupFunction &&group) {
  using K = decltype(details::ap(group, *seq_view(in).begin()));
  if constexpr (details::assert_non_void<K>()) {}
  std::unordered_map<K, Inner> ys;
  for (const auto &x : seq_view(in)) {
    auto k = details::ap(group, x);
    if (auto it = ys.find(k); it != ys.end()) push(it->second, x);
    else ys.emplace(k, Inner{x});
  }
  return ys;
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> min(const In &in) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  T best = *it;
  for (++it; it != end; ++it)
    if (*it < best) best = *it;
  return std::optional<T>{best};
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> max(const In &in) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  T best = *it;
  for (++it; it != end; ++it)
    if (best < *it) best = *it;
  return std::optional<T>{best};
}

template <typename In, typename Function, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> min_by(const In &in, Function f) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  T best = *it;
  auto best_key = details::ap(f, *it);
  for (++it; it != end; ++it)
    if (auto k = details::ap(f, *it); k < best_key) best_key = k, best = *it;
  return std::optional<T>{best};
}

template <typename In, typename Function, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> max_by(const In &in, Function f) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  T best = *it;
  auto best_key = details::ap(f, *it);
  for (++it; it != end; ++it)
    if (auto k = details::ap(f, *it); best_key < k) best_key = k, best = *it;
  return std::optional<T>{best};
}

template <typename In> //
[[nodiscard]] constexpr std::optional<size_t> index_of_min(const In &in) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  auto best = *it;
  size_t best_idx = 0, i = 0;
  for (++it, ++i; it != end; ++it, ++i)
    if (*it < best) {
      best = *it;
      best_idx = i;
    }
  return std::optional<size_t>{best_idx};
}

template <typename In> //
[[nodiscard]] constexpr std::optional<size_t> index_of_max(const In &in) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  auto best = *it;
  size_t best_idx = 0, i = 0;
  for (++it, ++i; it != end; ++it, ++i)
    if (best < *it) {
      best = *it;
      best_idx = i;
    }
  return std::optional<size_t>{best_idx};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr std::optional<size_t> index_of_min_by(const In &in, Function f) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  auto best_key = details::ap(f, *it);
  size_t best_idx = 0, i = 0;
  for (++it, ++i; it != end; ++it, ++i)
    if (auto k = details::ap(f, *it); k < best_key) {
      best_key = k;
      best_idx = i;
    }
  return std::optional<size_t>{best_idx};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr std::optional<size_t> index_of_max_by(const In &in, Function f) {
  auto it = seq_view(in).begin(), end = seq_view(in).end();
  if (it == end) return std::nullopt;
  auto best_key = details::ap(f, *it);
  size_t best_idx = 0, i = 0;
  for (++it, ++i; it != end; ++it, ++i)
    if (auto k = details::ap(f, *it); best_key < k) {
      best_key = k;
      best_idx = i;
    }
  return std::optional<size_t>{best_idx};
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr T sum(const In &in) {
  T acc{};
  for (auto &&x : seq_view(in))
    acc = acc + x;
  return acc;
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr T product(const In &in) {
  static_assert(std::is_arithmetic_v<T>, "product requires an arithmetic element type");
  T acc = static_cast<T>(1);
  for (auto &&x : seq_view(in))
    acc = acc * x;
  return acc;
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto sum_by(const In &in, Function f) {
  using U = std::decay_t<decltype(details::ap(f, *seq_view(in).begin()))>;
  U acc{};
  for (auto &&x : seq_view(in))
    acc = acc + details::ap(f, x);
  return acc;
}

template <typename In, typename Out, typename Other> //
[[nodiscard]] constexpr Out intersect(const In &in, const Other &other) {
  using V = typename In::value_type;
  static_assert(is_hashable<V>, "intersect type must be hashable");
  static_assert(is_comparable<V>, "intersect type must be comparable");
  std::unordered_multiset<V> pool{other.begin(), other.end()};
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
  for (auto &&x : seq_view(in))
    if (auto it = pool.find(x); it != pool.end()) pool.erase(it), push(ys, x);
  return ys;
}

template <typename In, typename Out, typename Other> //
[[nodiscard]] constexpr Out diff(const In &in, const Other &other) {
  using V = typename In::value_type;
  static_assert(is_hashable<V>, "diff type must be hashable");
  static_assert(is_comparable<V>, "diff type must be comparable");
  std::unordered_multiset<V> pool{other.begin(), other.end()};
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
  for (auto &&x : seq_view(in)) {
    if (auto it = pool.find(x); it != pool.end()) pool.erase(it);
    else push(ys, x);
  }
  return ys;
}

template <typename In, typename Out, typename Other> //
[[nodiscard]] constexpr Out symmetric_difference(const In &in, const Other &other) {
  using V = typename In::value_type;
  static_assert(is_hashable<V>, "symmetric_difference type must be hashable");
  static_assert(is_comparable<V>, "symmetric_difference type must be comparable");
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In> && has_size<Other>) ys.reserve(in.size() + other.size());
  std::unordered_multiset<V> b_pool{other.begin(), other.end()};
  for (auto &&x : seq_view(in)) {
    if (auto it = b_pool.find(x); it != b_pool.end()) b_pool.erase(it);
    else push(ys, x);
  }
  std::unordered_multiset<V> a_pool{seq_view(in).begin(), seq_view(in).end()};
  for (auto &&x : other) {
    if (auto it = a_pool.find(x); it != a_pool.end()) a_pool.erase(it);
    else push(ys, x);
  }
  return ys;
}

} // namespace aspartame::details::container1
