#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename T> struct optional_traits<std::optional<T>> {
  using value_type = T;
  template <typename U> using rebind = std::optional<U>;
  static constexpr bool engaged(const std::optional<T> &o) noexcept { return o.has_value(); }
  static constexpr const T &get(const std::optional<T> &o) noexcept { return *o; }
};

template <typename T> struct enable_pipe<T, std::enable_if_t<details::has_optional_traits_v<T>>> : std::true_type {};

namespace details {
template <typename C> using opt_value = typename optional_traits<std::decay_t<C>>::value_type;
template <typename C, typename U> using opt_rebind = typename optional_traits<std::decay_t<C>>::template rebind<U>;
template <typename C> using opt_same = opt_rebind<C, opt_value<C>>;
template <typename C> constexpr bool opt_engaged(const C &c) noexcept { return optional_traits<std::decay_t<C>>::engaged(c); }
template <typename C> constexpr const opt_value<C> &opt_get(const C &c) noexcept { return optional_traits<std::decay_t<C>>::get(c); }
} // namespace details

#define ASPARTAME_OPT_TPL(...) typename C, __VA_ARGS__, std::enable_if_t<details::has_optional_traits_v<C>, int> = 0
#define ASPARTAME_OPT_TPL_NOTYPE typename C, std::enable_if_t<details::has_optional_traits_v<C>, int> = 0

template <typename L, typename Op, std::enable_if_t<details::has_optional_traits_v<std::decay_t<L>>, int> = 0>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const L &, tag>
#endif
auto operator|(const L &l, const Op &r) {
  return r(l, tag{});
}

template <ASPARTAME_OPT_TPL(typename Function)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto mk_string(const C &in, const std::string_view &prefix, const std::string_view &,
                                                       const std::string_view &suffix, Function &&function, tag = {}) {
  static_assert(std::is_convertible_v<decltype(details::ap(function, details::opt_get(in))), std::string>,
                "function does not return a string type");
  std::string out{prefix};
  if (details::opt_engaged(in)) out += details::ap(function, details::opt_get(in));
  out += suffix;
  return out;
}

template <ASPARTAME_OPT_TPL(typename T)> [[nodiscard]] constexpr auto append(const C &in, const T &t, tag = {}) {
  using V = details::opt_value<C>;
  static_assert(std::is_convertible_v<std::decay_t<T>, V>, "value type does not unify with optional's value type");
  return details::opt_engaged(in) ? std::vector<V>{details::opt_get(in), t} : std::vector<V>{t};
}

template <ASPARTAME_OPT_TPL(typename Container)> [[nodiscard]] constexpr auto concat(const C &in, const Container &other, tag = {}) {
  using V = details::opt_value<C>;
  static_assert(details::has_optional_traits_v<std::decay_t<Container>>, "other is not an optional-like type");
  static_assert(std::is_convertible_v<details::opt_value<Container>, V>, "value type does not unify");
  if (!details::opt_engaged(in)) return details::opt_engaged(other) ? std::vector<V>{details::opt_get(other)} : std::vector<V>{};
  std::vector<V> ys;
  ys.reserve(1 + (details::opt_engaged(other) ? 1 : 0));
  ys.push_back(details::opt_get(in));
  if (details::opt_engaged(other)) ys.push_back(details::opt_get(other));
  return ys;
}

template <ASPARTAME_OPT_TPL(typename Function)> [[nodiscard]] constexpr auto map(const C &in, Function &&function, tag = {}) {
  using T = decltype(details::ap(function, details::opt_get(in)));
  if constexpr (details::assert_non_void<T>()) {}
  using Out = details::opt_rebind<C, T>;
  if (details::opt_engaged(in)) return Out{details::ap(function, details::opt_get(in))};
  else return Out{};
}

template <ASPARTAME_OPT_TPL(typename Function)> [[nodiscard]] constexpr auto collect(const C &in, Function &&function, tag = {}) {
  using R = decltype(details::ap(function, details::opt_get(in)));
  static_assert(is_optional<R>, "collect function should return an optional");
  return details::opt_engaged(in) ? details::ap(function, details::opt_get(in)) : R{};
}

template <ASPARTAME_OPT_TPL(typename Function)> [[nodiscard]] constexpr auto collect_first(const C &in, Function &&function, tag = {}) {
  using R = decltype(details::ap(function, details::opt_get(in)));
  static_assert(is_optional<R>, "collect function should return an optional");
  return details::opt_engaged(in) ? details::ap(function, details::opt_get(in)) : R{};
}

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto filter(const C &in, Predicate &&predicate, tag = {}) -> details::opt_same<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in) && details::ap(predicate, details::opt_get(in))) return in;
  return details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL(typename Function)> [[nodiscard]] constexpr auto flat_map(const C &in, Function &&function, tag = {}) {
  using R = decltype(details::ap(function, details::opt_get(in)));
  static_assert(is_optional<R>, "f should return an optional");
  return details::opt_engaged(in) ? R{details::ap(function, details::opt_get(in))} : R{};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto flatten(const C &in, tag = {}) -> details::opt_value<C> {
  using Inner = details::opt_value<C>;
  static_assert(is_optional<Inner>, "not a nested optional type");
  return details::opt_engaged(in) ? details::opt_get(in) : Inner{};
}

template <ASPARTAME_OPT_TPL(typename Function)>
[[nodiscard]] constexpr auto distinct_by(const C &in, Function &&, tag = {}) -> details::opt_same<C> {
  return in;
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto distinct(const C &in, tag = {}) -> details::opt_same<C> { return in; }

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto ensure(const C &in, Predicate &&p, tag = {}) -> details::opt_same<C> {
  if (!details::opt_engaged(in)) return in;
  return details::ap(p, details::opt_get(in)) ? in : details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto count(const C &in, Predicate &&predicate, tag = {}) -> size_t {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  return details::opt_engaged(in) && details::ap(predicate, details::opt_get(in)) ? 1 : 0;
}

template <ASPARTAME_OPT_TPL(typename Predicate)> [[nodiscard]] constexpr auto exists(const C &in, Predicate &&predicate, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  return details::opt_engaged(in) && details::ap(predicate, details::opt_get(in));
}

template <ASPARTAME_OPT_TPL(typename Predicate)> [[nodiscard]] constexpr auto forall(const C &in, Predicate p, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(p, details::opt_get(in)))>()) {}
  return !details::opt_engaged(in) || details::ap(p, details::opt_get(in));
}

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto find(const C &in, Predicate p, tag = {}) -> details::opt_same<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(p, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in) && details::ap(p, details::opt_get(in))) return in;
  return details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL(typename Function)>
[[nodiscard]] constexpr auto reduce(const C &in, Function &&, tag = {}) -> details::opt_same<C> {
  return in;
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto min(const C &in, tag = {}) -> details::opt_same<C> { return in; }
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto max(const C &in, tag = {}) -> details::opt_same<C> { return in; }
template <ASPARTAME_OPT_TPL(typename Function)>
[[nodiscard]] constexpr auto min_by(const C &in, Function &&, tag = {}) -> details::opt_same<C> {
  return in;
}
template <ASPARTAME_OPT_TPL(typename Function)>
[[nodiscard]] constexpr auto max_by(const C &in, Function &&, tag = {}) -> details::opt_same<C> {
  return in;
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto sum(const C &in, tag = {}) -> details::opt_value<C> {
  using V = details::opt_value<C>;
  return details::opt_engaged(in) ? details::opt_get(in) : V{};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto product(const C &in, tag = {}) -> details::opt_value<C> {
  using V = details::opt_value<C>;
  static_assert(std::is_arithmetic_v<V>, "product requires an arithmetic element type");
  return details::opt_engaged(in) ? details::opt_get(in) : static_cast<V>(1);
}
template <ASPARTAME_OPT_TPL(typename Function)> [[nodiscard]] constexpr auto sum_by(const C &in, Function &&f, tag = {}) {
  using U = std::decay_t<decltype(details::ap(f, details::opt_get(in)))>;
  return details::opt_engaged(in) ? details::ap(f, details::opt_get(in)) : U{};
}
template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto none_match(const C &in, Predicate &&predicate, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  return !(details::opt_engaged(in) && details::ap(predicate, details::opt_get(in)));
}
template <ASPARTAME_OPT_TPL(typename U)>
[[nodiscard]] constexpr auto last_index_of(const C &in, const U &u, tag = {}) -> std::make_signed_t<size_t> {
  using V = details::opt_value<C>;
  static_assert(std::is_convertible_v<std::decay_t<U>, V>, "type does not match optional's value type");
  return details::opt_engaged(in) && details::opt_get(in) == u ? 0 : -1;
}
template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto last_index_where(const C &in, Predicate &&predicate, tag = {}) -> std::make_signed_t<size_t> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  return details::opt_engaged(in) && details::ap(predicate, details::opt_get(in)) ? 0 : -1;
}

template <ASPARTAME_OPT_TPL(typename Function)>
[[nodiscard]] constexpr auto tap_each(const C &in, Function &&function, tag = {}) -> details::opt_same<C> {
  if constexpr (details::assert_void<decltype(details::ap(function, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in)) details::ap(function, details::opt_get(in));
  return in;
}

template <ASPARTAME_OPT_TPL(typename Function)> constexpr auto for_each(const C &in, Function &&function, tag = {}) -> void {
  if constexpr (details::assert_void<decltype(details::ap(function, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in)) details::ap(function, details::opt_get(in));
}

template <ASPARTAME_OPT_TPL(typename Predicate)> [[nodiscard]] constexpr auto partition(const C &in, Predicate &&predicate, tag = {}) {
  using Same = details::opt_same<C>;
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  return (details::opt_engaged(in) && details::ap(predicate, details::opt_get(in))) ? std::pair{Same{in}, Same{}}
                                                                                    : std::pair{Same{}, Same{in}};
}

template <ASPARTAME_OPT_TPL(typename GroupFunction, typename MapFunction, typename ReduceFunction)>
[[nodiscard]] constexpr auto group_map_reduce(const C &in, GroupFunction &&group, MapFunction &&map, ReduceFunction &&, tag = {}) {
  using K = decltype(details::ap(group, details::opt_get(in)));
  using V = decltype(details::ap(map, details::opt_get(in)));
  using R = std::invoke_result_t<ReduceFunction, V, V>;
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  if constexpr (details::assert_non_void<R>()) {}
  return details::opt_engaged(in)
             ? std::unordered_map<K, V>{{details::ap(group, details::opt_get(in)), details::ap(map, details::opt_get(in))}}
             : std::unordered_map<K, V>{};
}

template <ASPARTAME_OPT_TPL(typename GroupFunction, typename MapFunction)>
[[nodiscard]] constexpr auto group_map(const C &in, GroupFunction &&group, MapFunction &&map, tag = {}) {
  using K = decltype(details::ap(group, details::opt_get(in)));
  using V = decltype(details::ap(map, details::opt_get(in)));
  using Vc = details::opt_rebind<C, V>;
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  return details::opt_engaged(in)
             ? std::unordered_map<K, Vc>{{details::ap(group, details::opt_get(in)), Vc{details::ap(map, details::opt_get(in))}}}
             : std::unordered_map<K, Vc>{};
}

template <ASPARTAME_OPT_TPL(typename Function)> [[nodiscard]] constexpr auto group_by(const C &in, Function &&function, tag = {}) {
  using K = decltype(details::ap(function, details::opt_get(in)));
  using Same = details::opt_same<C>;
  if constexpr (details::assert_non_void<K>()) {}
  return details::opt_engaged(in) ? std::unordered_map<K, Same>{{details::ap(function, details::opt_get(in)), Same{in}}}
                                  : std::unordered_map<K, Same>{};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto to_vector(const C &in, tag = {}) {
  using V = details::opt_value<C>;
  return details::opt_engaged(in) ? std::vector<V>{details::opt_get(in)} : std::vector<V>{};
}

template <template <typename...> typename Cs, ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto to(const C &in, tag = {}) {
  using V = details::opt_value<C>;
  if constexpr (is_pair<V> && !is_unary_instantiable<Cs, V>) {
    using Cs2 = Cs<typename V::first_type, typename V::second_type>;
    return details::opt_engaged(in) ? Cs2{details::opt_get(in)} : Cs2{};
  } else {
    return details::opt_engaged(in) ? Cs<V>{details::opt_get(in)} : Cs<V>{};
  }
}

template <ASPARTAME_OPT_TPL(typename T)> [[nodiscard]] constexpr auto prepend(const C &in, const T &t, tag = {}) {
  using V = details::opt_value<C>;
  static_assert(std::is_convertible_v<std::decay_t<T>, V>, "value type does not unify with optional's value type");
  return details::opt_engaged(in) ? std::vector<V>{t, details::opt_get(in)} : std::vector<V>{t};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto head_maybe(const C &in, tag = {}) -> details::opt_same<C> { return in; }
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto last_maybe(const C &in, tag = {}) -> details::opt_same<C> { return in; }
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto init(const C &, tag = {}) -> details::opt_same<C> {
  return details::opt_same<C>{};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto tail(const C &, tag = {}) -> details::opt_same<C> {
  return details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto at_maybe(const C &in, size_t idx, tag = {}) -> details::opt_same<C> {
  return idx == 0 ? details::opt_same<C>{in} : details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL_NOTYPE>
[[nodiscard]] constexpr auto slice(const C &in, size_t from_inclusive, size_t to_exclusive, tag = {}) -> details::opt_same<C> {
  return (!details::opt_engaged(in) || from_inclusive > 0 || to_exclusive == 0) ? details::opt_same<C>{} : details::opt_same<C>{in};
}

template <ASPARTAME_OPT_TPL(typename Container)>
[[nodiscard]] constexpr auto index_of_slice(const C &in, const Container &other, tag = {}) -> std::make_signed_t<size_t> {
  static_assert(details::has_optional_traits_v<std::decay_t<Container>>, "other type is not an optional-like");
  static_assert(std::is_same_v<details::opt_value<Container>, details::opt_value<C>>, "cannot unify value type on both sides");
  if (!details::opt_engaged(in) && !details::opt_engaged(other)) return 0;
  if (details::opt_engaged(in) && details::opt_engaged(other) && details::opt_get(in) == details::opt_get(other)) return 0;
  if (details::opt_engaged(in) && !details::opt_engaged(other)) return 0;
  return -1;
}

template <ASPARTAME_OPT_TPL(typename Container)>
[[nodiscard]] constexpr auto contains_slice(const C &in, const Container &other, tag = {}) -> bool {
  return index_of_slice(in, other) != -1;
}

template <ASPARTAME_OPT_TPL(typename U)>
[[nodiscard]] constexpr auto index_of(const C &in, const U &u, tag = {}) -> std::make_signed_t<size_t> {
  using V = details::opt_value<C>;
  static_assert(std::is_convertible_v<std::decay_t<U>, V>, "type does not match optional's value type");
  return details::opt_engaged(in) && details::opt_get(in) == u ? 0 : -1;
}

template <ASPARTAME_OPT_TPL(typename U)> [[nodiscard]] constexpr auto contains(const C &in, const U &u, tag = {}) -> bool {
  return index_of(in, u) != -1;
}

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto find_last(const C &in, Predicate p, tag = {}) -> details::opt_same<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(p, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in) && details::ap(p, details::opt_get(in))) return in;
  return details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto index_where(const C &in, Predicate &&predicate, tag = {}) -> std::make_signed_t<size_t> {
  return details::opt_engaged(in) && details::ap(predicate, details::opt_get(in)) ? 0 : -1;
}

template <ASPARTAME_OPT_TPL(typename N)> [[nodiscard]] constexpr auto zip_with_index(const C &in, N from, tag = {}) {
  using V = std::decay_t<details::opt_value<C>>;
  using R = details::opt_rebind<C, std::pair<V, N>>;
  return details::opt_engaged(in) ? R{{details::opt_get(in), from}} : R{};
}

template <ASPARTAME_OPT_TPL(typename Container)> [[nodiscard]] constexpr auto zip(const C &in, Container &&container, tag = {}) {
  using OC = std::decay_t<Container>;
  static_assert(details::has_optional_traits_v<OC>, "other value is not an optional-like type");
  using V = std::decay_t<details::opt_value<C>>;
  using W = details::opt_value<OC>;
  using R = details::opt_rebind<C, std::pair<V, W>>;
  return details::opt_engaged(in) && details::opt_engaged(container) ? R{{details::opt_get(in), details::opt_get(container)}} : R{};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto transpose(const C &in, tag = {}) -> details::opt_same<C> {
  using V = details::opt_value<C>;
  static_assert(is_optional<V>, "not a nested optional type");
  if (details::opt_engaged(in) && details::opt_engaged(details::opt_get(in))) return in;
  return details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto cartesian_product(const C &in, tag = {}) -> details::opt_same<C> {
  using V = details::opt_value<C>;
  static_assert(is_optional<V>, "not a nested optional type");
  if (details::opt_engaged(in) && details::opt_engaged(details::opt_get(in))) return in;
  return details::opt_same<C>{V{}};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto reverse(const C &in, tag = {}) -> details::opt_same<C> { return in; }

template <ASPARTAME_OPT_TPL(typename URBG)> [[nodiscard]] constexpr auto shuffle(const C &in, URBG &, tag = {}) -> details::opt_same<C> {
  return in;
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto sort(const C &in, tag = {}) -> details::opt_same<C> { return in; }
template <ASPARTAME_OPT_TPL(typename Compare)>
[[nodiscard]] constexpr auto sort(const C &in, Compare &&, tag = {}) -> details::opt_same<C> {
  return in;
}
template <ASPARTAME_OPT_TPL(typename Select)>
[[nodiscard]] constexpr auto sort_by(const C &in, Select &&, tag = {}) -> details::opt_same<C> {
  return in;
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto split_at(const C &in, size_t idx, tag = {}) {
  using Same = details::opt_same<C>;
  if (details::opt_engaged(in)) return idx == 0 ? std::pair<Same, Same>{Same{}, Same{in}} : std::pair<Same, Same>{Same{in}, Same{}};
  return std::pair<Same, Same>{Same{}, Same{}};
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto take(const C &in, size_t n, tag = {}) -> details::opt_same<C> {
  return n == 0 ? details::opt_same<C>{} : details::opt_same<C>{in};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto drop(const C &in, size_t n, tag = {}) -> details::opt_same<C> {
  return n != 0 ? details::opt_same<C>{} : details::opt_same<C>{in};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto take_right(const C &in, size_t n, tag = {}) -> details::opt_same<C> {
  return n == 0 ? details::opt_same<C>{} : details::opt_same<C>{in};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto drop_right(const C &in, size_t n, tag = {}) -> details::opt_same<C> {
  return n != 0 ? details::opt_same<C>{} : details::opt_same<C>{in};
}

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto take_while(const C &in, Predicate &&predicate, tag = {}) -> details::opt_same<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in) && details::ap(predicate, details::opt_get(in))) return in;
  return details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] constexpr auto drop_while(const C &in, Predicate &&predicate, tag = {}) -> details::opt_same<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in) && !details::ap(predicate, details::opt_get(in))) return in;
  return details::opt_same<C>{};
}

template <ASPARTAME_OPT_TPL(typename Predicate)> [[nodiscard]] constexpr auto span(const C &in, Predicate &&predicate, tag = {}) {
  using Same = details::opt_same<C>;
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, details::opt_get(in)))>()) {}
  if (details::opt_engaged(in) && details::ap(predicate, details::opt_get(in))) return std::pair{Same{in}, Same{}};
  return std::pair{Same{}, Same{in}};
}

template <ASPARTAME_OPT_TPL(typename Accumulator, typename Function)>
[[nodiscard]] constexpr Accumulator fold_left(const C &in, Accumulator init, Function &&function, tag = {}) {
  return details::opt_engaged(in) ? function(std::move(init), details::opt_get(in)) : init;
}

template <ASPARTAME_OPT_TPL(typename Accumulator, typename Function)>
[[nodiscard]] constexpr Accumulator fold_right(const C &in, Accumulator init, Function &&function, tag = {}) {
  return details::opt_engaged(in) ? function(details::opt_get(in), std::move(init)) : init;
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto sliding(const C &in, size_t size, size_t step, tag = {}) {
  using Same = details::opt_same<C>;
  using Out = details::opt_rebind<C, Same>;
  if (size == 0 || step == 0)
    details::raise<std::range_error>("cannot apply windowing with zero size or step, size=" + std::to_string(size) +
                                     " step=" + std::to_string(step));
  return details::opt_engaged(in) ? Out{Same{in}} : Out{};
}

template <ASPARTAME_OPT_TPL(typename Function, typename FunctionEmpty)>
[[nodiscard]] constexpr auto fold(const C &in, Function f, FunctionEmpty empty, tag = {}) {
  using T = decltype(details::ap(f, details::opt_get(in)));
  static_assert(std::is_convertible_v<decltype(empty()), T>, "f and empty does not unify");
  return details::opt_engaged(in) ? details::ap(f, details::opt_get(in)) : static_cast<T>(empty());
}

template <ASPARTAME_OPT_TPL(typename FunctionEmpty)> [[nodiscard]] constexpr auto fold(const C &in, FunctionEmpty empty, tag = {}) {
  using V = details::opt_value<C>;
  static_assert(std::is_convertible_v<decltype(empty()), V>, "empty does not unify with value type");
  return details::opt_engaged(in) ? details::opt_get(in) : static_cast<V>(empty());
}

template <ASPARTAME_OPT_TPL(typename T)> [[nodiscard]] constexpr auto get_or_else(const C &in, const T &default_value, tag = {}) {
  using V = details::opt_value<C>;
  static_assert(std::is_convertible_v<T, V>, "optional value type and default_value type does not unify");
  return details::opt_engaged(in) ? details::opt_get(in) : static_cast<V>(default_value);
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto get_or_default(const C &in, tag = {}) {
  using V = details::opt_value<C>;
  static_assert(std::is_default_constructible_v<V>, "type is not default constructible");
  return details::opt_engaged(in) ? details::opt_get(in) : V{};
}

template <ASPARTAME_OPT_TPL(typename U)>
[[nodiscard]] constexpr auto or_else_maybe(const C &in, const U &empty, tag = {}) -> details::opt_same<C> {
  using V = details::opt_value<C>;
  if constexpr (std::is_same_v<std::decay_t<U>, std::nullopt_t>) return in;
  else {
    static_assert(details::has_optional_traits_v<std::decay_t<U>>, "other value is not an optional-like type");
    static_assert(std::is_convertible_v<details::opt_value<U>, V>, "value types do not unify");
    if (details::opt_engaged(in)) return in;
    return details::opt_engaged(empty) ? details::opt_same<C>{details::opt_get(empty)} : details::opt_same<C>{};
  }
}

template <ASPARTAME_OPT_TPL(typename F)>
[[nodiscard]] constexpr auto or_else(const C &in, const F &empty, tag = {}) -> details::opt_same<C> {
  using R = std::decay_t<decltype(empty())>;
  using V = details::opt_value<C>;
  if constexpr (std::is_same_v<R, std::nullopt_t>) return in;
  else {
    static_assert(details::has_optional_traits_v<R>, "return type of other functor is not an optional-like");
    static_assert(std::is_convertible_v<details::opt_value<R>, V>, "value types do not unify");
    if (details::opt_engaged(in)) return in;
    auto r = empty();
    return details::opt_engaged(r) ? details::opt_same<C>{details::opt_get(r)} : details::opt_same<C>{};
  }
}

template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto stride(const C &in, size_t n, tag = {}) -> details::opt_same<C> {
  return n > 0 ? details::opt_same<C>{in} : details::opt_same<C>{};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto pairwise(const C &, tag = {}) {
  using V = details::opt_value<C>;
  return details::opt_rebind<C, std::pair<V, V>>{};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto top_k(const C &in, size_t k, tag = {}) -> details::opt_same<C> {
  return k > 0 ? details::opt_same<C>{in} : details::opt_same<C>{};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto bottom_k(const C &in, size_t k, tag = {}) -> details::opt_same<C> {
  return k > 0 ? details::opt_same<C>{in} : details::opt_same<C>{};
}
template <ASPARTAME_OPT_TPL(typename URBG)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sample(const C &in, size_t k, URBG &&, tag = {}) -> details::opt_same<C> {
  return k > 0 ? details::opt_same<C>{in} : details::opt_same<C>{};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto is_sorted(const C &, tag = {}) -> bool { return true; }
template <ASPARTAME_OPT_TPL(typename Function)> [[nodiscard]] constexpr auto is_sorted_by(const C &, Function &&, tag = {}) -> bool {
  return true;
}
template <ASPARTAME_OPT_TPL(typename Other)>
[[nodiscard]] constexpr auto intersect(const C &in, const Other &other, tag = {}) -> details::opt_same<C> {
  if (!details::opt_engaged(in)) return details::opt_same<C>{};
  for (auto &&y : other)
    if (details::opt_get(in) == y) return in;
  return details::opt_same<C>{};
}
template <ASPARTAME_OPT_TPL(typename Other)>
[[nodiscard]] constexpr auto diff(const C &in, const Other &other, tag = {}) -> details::opt_same<C> {
  if (!details::opt_engaged(in)) return details::opt_same<C>{};
  for (auto &&y : other)
    if (details::opt_get(in) == y) return details::opt_same<C>{};
  return in;
}
template <ASPARTAME_OPT_TPL(typename Other)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto symmetric_difference(const C &in, const Other &other, tag = {}) {
  using V = details::opt_value<C>;
  std::vector<V> ys;
  if (details::opt_engaged(in)) {
    bool matched = false;
    for (auto &&y : other)
      if (details::opt_get(in) == y) {
        matched = true;
        break;
      }
    if (!matched) ys.push_back(details::opt_get(in));
  }
  for (auto &&y : other) {
    if (!details::opt_engaged(in) || !(details::opt_get(in) == y)) ys.push_back(y);
  }
  return ys;
}
template <ASPARTAME_OPT_TPL(typename Other)> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto cross(const C &in, const Other &other, tag = {}) {
  using V = details::opt_value<C>;
  using B = typename std::decay_t<Other>::value_type;
  std::vector<std::pair<V, B>> ys;
  if (!details::opt_engaged(in)) return ys;
  for (auto &&y : other)
    ys.emplace_back(details::opt_get(in), y);
  return ys;
}
template <ASPARTAME_OPT_TPL(typename Acc, typename Function)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto scan_left(const C &in, Acc init, Function f, tag = {}) {
  using B = std::decay_t<Acc>;
  std::vector<B> ys;
  ys.push_back(init);
  if (details::opt_engaged(in)) ys.push_back(f(init, details::opt_get(in)));
  return ys;
}
template <ASPARTAME_OPT_TPL(typename Acc, typename Function)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto scan_right(const C &in, Acc init, Function f, tag = {}) {
  using B = std::decay_t<Acc>;
  std::vector<B> ys;
  if (details::opt_engaged(in)) ys.push_back(f(details::opt_get(in), init));
  ys.push_back(init);
  return ys;
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] constexpr auto unzip(const C &in, tag = {}) {
  using V = details::opt_value<C>;
  static_assert(details::is_pair_like_v<V>, "unzip requires a pair-like element");
  using A = std::decay_t<typename V::first_type>;
  using B = std::decay_t<typename V::second_type>;
  using OA = details::opt_rebind<C, A>;
  using OB = details::opt_rebind<C, B>;
  if (details::opt_engaged(in)) return std::pair<OA, OB>{details::opt_get(in).first, details::opt_get(in).second};
  return std::pair<OA, OB>{OA{}, OB{}};
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto combinations(const C &in, size_t k, tag = {}) {
  using V = details::opt_value<C>;
  std::vector<std::vector<V>> ys;
  if (k == 0) ys.push_back({});
  else if (k == 1 && details::opt_engaged(in)) ys.push_back({details::opt_get(in)});
  return ys;
}
template <ASPARTAME_OPT_TPL_NOTYPE> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto permutations(const C &in, tag = {}) {
  using V = details::opt_value<C>;
  std::vector<std::vector<V>> ys;
  if (details::opt_engaged(in)) ys.push_back({details::opt_get(in)});
  else ys.push_back({});
  return ys;
}
template <ASPARTAME_OPT_TPL(typename Predicate)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto chunk_by(const C &in, Predicate &&, tag = {}) {
  using V = details::opt_value<C>;
  std::vector<std::vector<V>> ys;
  if (details::opt_engaged(in)) ys.push_back({details::opt_get(in)});
  return ys;
}
template <ASPARTAME_OPT_TPL(typename Sep)>
[[nodiscard]] constexpr auto join_with(const C &in, const Sep &, tag = {}) -> details::opt_value<C> {
  using V = details::opt_value<C>;
  return details::opt_engaged(in) ? details::opt_get(in) : V{};
}

#undef ASPARTAME_OPT_TPL
#undef ASPARTAME_OPT_TPL_NOTYPE

} // namespace aspartame
