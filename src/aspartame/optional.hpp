#pragma once

#include "details/base.hpp"
#include "fluent.hpp"

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace aspartame {

template <typename T, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::optional<T> &, tag>
#endif
auto operator^(const std::optional<T> &l, const Op &r) {
  return r(l, tag{});
}

// ---

template <typename C, typename Function> //
[[nodiscard]] auto mk_string(const std::optional<C> &in, const std::string_view &prefix, const std::string_view &,
                             const std::string_view &suffix, Function &&function, tag = {}) {
  static_assert(std::is_convertible_v<decltype(details::ap(function, *in)), std::string>, "function does not return a string type");
  std::string out{prefix};
  if (in) out += details::ap(function, *in);
  out += suffix;
  return out;
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto append(const std::optional<C> &in, const T &t, tag = {}) -> std::vector<C> {
  static_assert(std::is_convertible_v<std::decay_t<T>, C>, "value type does not unify with optional's value type");
  return in ? std::vector<C>{*in, t} : std::vector<C>{t};
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto concat(const std::optional<C> &in, const Container &other, tag = {}) -> std::vector<C> {
  static_assert(is_optional<std::decay_t<Container>>, "container is not an optional type");
  static_assert(std::is_convertible_v<typename std::decay_t<Container>::value_type, C>,
                "value type does not unify with optional's value type");
  if (!in) return other ? std::vector<C>{*other} : std::vector<C>{};
  else {
    if constexpr (std::is_default_constructible_v<C>) {
      std::vector<C> ys(1 + (other ? 1 : 0));
      ys[0] = *in;
      if (other) ys[1] = *other;
      return ys;
    } else {
      std::vector<C> ys;
      ys.reserve(1 + (other ? 1 : 0));
      ys.push_back(*in);
      if (other) ys.push_back(*other);
      return ys;
    }
  }
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto map(const std::optional<C> &in, Function &&function, tag = {}) {
  using T = decltype(details::ap(function, *in));
  if constexpr (details::assert_non_void<T>()) {};
  if (in) return std::optional<T>{details::ap(function, *in)};
  else return std::optional<T>{std::nullopt};
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto collect(const std::optional<C> &in, Function &&function, tag = {}) {
  static_assert(is_optional<decltype(details::ap(function, *in))>, "collect function should return an optional");
  return in ? details::ap(function, *in) : std::nullopt;
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto collect_first(const std::optional<C> &in, Function &&function, tag = {}) {
  static_assert(is_optional<decltype(details::ap(function, *in))>, "collect function should return an optional");
  return in ? details::ap(function, *in) : std::nullopt;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto filter(const std::optional<C> &in, Predicate &&predicate, tag = {}) -> std::optional<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  if (in) {
    // XXX don't use ternary here, NVHPC breaks: https://forums.developer.nvidia.com/t/nvc-miscompiles-code-involving-std-tuple/281754
    if (details::ap(predicate, *in)) return in;
    else return std::nullopt;
  } else return std::optional<C>{std::nullopt};
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto flat_map(const std::optional<C> &in, Function &&function, tag = {}) {
  using R = decltype(details::ap(function, *in));
  static_assert(is_optional<decltype(details::ap(function, *in))>, "f should return an optional");
  return in ? R{details::ap(function, *in)} : R{};
}

template <typename C> //
[[nodiscard]] constexpr auto flatten(const std::optional<C> &in, tag = {}) -> C {
  static_assert(is_optional<C>, "not a nested optional type");
  if (in) return *in;
  else return std::nullopt;
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto distinct_by(const std::optional<C> &in, Function &&, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C> //
[[nodiscard]] constexpr auto distinct(const std::optional<C> &in, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto count(const std::optional<C> &in, Predicate &&predicate, tag = {}) -> size_t {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  return in && details::ap(predicate, *in) ? 1 : 0;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto exists(const std::optional<C> &in, Predicate &&predicate, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  return in && details::ap(predicate, *in);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto forall(const std::optional<C> &in, Predicate p, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in))>()) {};
  return !in || details::ap(p, *in);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto find(const std::optional<C> &in, Predicate p, tag = {}) -> std::optional<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in))>()) {};
  return in && details::ap(p, *in) ? in : std::nullopt;
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto reduce(const std::optional<C> &in, Function &&, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto tap_each(const std::optional<C> &in, Function &&function, tag = {}) -> std::optional<C> {
  if constexpr (details::assert_void<decltype(details::ap(function, *in))>()) {}
  if (in) details::ap(function, *in);
  return in;
}

template <typename C, typename Function> //
constexpr auto for_each(const std::optional<C> &in, Function &&function, tag = {}) -> void {
  if constexpr (details::assert_void<decltype(details::ap(function, *in))>()) {}
  if (in) details::ap(function, *in);
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto partition(const std::optional<C> &in, Predicate &&predicate, tag = {})
    -> std::pair<std::optional<C>, std::optional<C>> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {}
  return (in && details::ap(predicate, *in)) ? std::pair{in, std::optional<C>{std::nullopt}}
                                             : std::pair{std::optional<C>{std::nullopt}, in};
}

template <typename C, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(const std::optional<C> &in, GroupFunction &&group, MapFunction &&map, ReduceFunction &&,
                                              tag = {}) {
  using K = decltype(details::ap(group, *in));
  using V = decltype(details::ap(map, *in));
  using R = std::invoke_result_t<ReduceFunction, V, V>;
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  if constexpr (details::assert_non_void<R>()) {}
  return in ? std::unordered_map<K, V>{{details::ap(group, *in), details::ap(map, *in)}} : std::unordered_map<K, V>{};
}

template <typename C, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(const std::optional<C> &in, GroupFunction &&group, MapFunction &&map, tag = {}) {
  using K = decltype(details::ap(group, *in));
  using V = decltype(details::ap(map, *in));
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  return in ? std::unordered_map<K, std::optional<V>>{{details::ap(group, *in), std::optional{details::ap(map, *in)}}}
            : std::unordered_map<K, std::optional<V>>{};
}

template <typename C, typename Function> //
[[nodiscard]] constexpr auto group_by(const std::optional<C> &in, Function &&function, tag = {}) {
  using K = decltype(details::ap(function, *in));
  if constexpr (details::assert_non_void<K>()) {}
  return in ? std::unordered_map<K, std::optional<C>>{{details::ap(function, *in), std::optional{*in}}}
            : std::unordered_map<K, std::optional<C>>{};
}

template <typename C> //
[[nodiscard]] constexpr auto to_vector(const std::optional<C> &o, tag = {}) -> std::vector<C> {
  return o ? std::vector<C>{*o} : std::vector<C>{};
}

template <template <typename...> typename Cs, typename C> //
[[nodiscard]] constexpr auto to(const std::optional<C> &o, tag = {}) {
  if constexpr (is_pair<C> && !is_unary_instantiable<Cs, C>) {
    using Cs2 = Cs<typename C::first_type, typename C::second_type>;
    return o ? Cs2{*o} : Cs2{};
  } else {
    return o ? Cs<C>{*o} : Cs<C>{};
  }
}

// =======

template <typename C, typename T> //
[[nodiscard]] constexpr auto prepend(const std::optional<C> &in, const T &t, tag = {}) -> std::vector<C> {
  static_assert(std::is_convertible_v<std::decay_t<T>, C>, "value type does not unify with optional's value type");
  return in ? std::vector<C>{t, *in} : std::vector<C>{t};
}

template <typename C> //
[[nodiscard]] constexpr auto head_maybe(const std::optional<C> &in, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C> //
[[nodiscard]] constexpr auto last_maybe(const std::optional<C> &in, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C> //
[[nodiscard]] constexpr auto init(const std::optional<C> &, tag = {}) -> std::optional<C> {
  return {};
}

template <typename C> //
[[nodiscard]] constexpr auto tail(const std::optional<C> &, tag = {}) -> std::optional<C> {
  return {};
}

template <typename C> //
[[nodiscard]] constexpr auto at_maybe(const std::optional<C> &o, size_t idx, tag = {}) -> std::optional<C> {
  return idx == 0 ? o : std::optional<C>{std::nullopt};
}

template <typename C> //
[[nodiscard]] constexpr auto slice(const std::optional<C> &in, size_t from_inclusive, size_t to_exclusive, tag = {}) -> std::optional<C> {
  return (!in || from_inclusive > 0 || to_exclusive == 0) ? std::optional<C>{std::nullopt} : in;
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto index_of_slice(const std::optional<C> &in, const Container &other, tag = {}) -> std::make_signed_t<size_t> {
  static_assert(is_optional<Container>, "other type is not an optional");
  static_assert(std::is_same_v<typename Container::value_type, C>, "cannot unify value type on both sides");
  return in == other ? 0 : -1;
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto contains_slice(const std::optional<C> &in, const Container &other, tag = {}) -> bool {
  return index_of_slice(in, other) != -1;
}

template <typename C, typename U> //
[[nodiscard]] constexpr auto index_of(const std::optional<C> &o, const U &u, tag = {}) -> std::make_signed_t<size_t> {
  static_assert(std::is_convertible_v<std::decay_t<U>, C>, "type does not match optional's value type");
  return o && *o == u ? 0 : -1;
}

template <typename C, typename U> //
[[nodiscard]] constexpr auto contains(const std::optional<C> &o, const U &u, tag = {}) -> bool {
  return index_of(o, u) != -1;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto find_last(const std::optional<C> &in, Predicate p, tag = {}) -> std::optional<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in))>()) {};
  return in && details::ap(p, *in) ? in : std::nullopt;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto index_where(const std::optional<C> &o, Predicate &&predicate, tag = {}) -> std::make_signed_t<size_t> {
  return o && details::ap(predicate, *o) ? 0 : -1;
}

template <typename C, typename N> //
[[nodiscard]] constexpr auto zip_with_index(const std::optional<C> &in, N from, tag = {}) -> std::optional<std::pair<std::decay_t<C>, N>> {
  using R = std::optional<std::pair<std::decay_t<C>, N>>;
  return in ? R{{*in, from}} : R{std::nullopt};
}

template <typename C, typename Container> //
[[nodiscard]] constexpr auto zip(const std::optional<C> &in, Container &&container, tag = {}) {
  static_assert(is_optional<Container>, "other value is not an optional type");
  using R = std::optional<std::pair<std::decay_t<C>, typename std::decay_t<Container>::value_type>>;
  return in && container ? R{{*in, *container}} : R{std::nullopt};
}

template <typename C> //
[[nodiscard]] constexpr auto transpose(const std::optional<C> &in, tag = {}) -> std::optional<C> {
  static_assert(is_optional<C>, "not a nested optional type");
  return in && *in ? in : std::optional<C>{};
}

template <typename C> //
[[nodiscard]] constexpr auto sequence(const std::optional<C> &in, tag = {}) -> std::optional<C> {
  static_assert(is_optional<C>, "not a nested optional type");
  return in && *in ? in : std::optional<C>{std::optional<typename C::value_type>{}};
}

template <typename C> //
[[nodiscard]] constexpr auto reverse(const std::optional<C> &in, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C, typename URBG> //
[[nodiscard]] constexpr auto shuffle(const std::optional<C> &in, URBG &, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C> //
[[nodiscard]] constexpr auto sort(const std::optional<C> &in, tag = {}, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C, typename Compare> //
[[nodiscard]] constexpr auto sort(const std::optional<C> &in, Compare &&, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C, typename Select> //
[[nodiscard]] constexpr auto sort_by(const std::optional<C> &in, Select &&, tag = {}) -> std::optional<C> {
  return in;
}

template <typename C> //
[[nodiscard]] constexpr auto split_at(const std::optional<C> &in, size_t idx, tag = {}) {
  if (in) return idx == 0 ? std::pair<std::optional<C>, std::optional<C>>{{}, in} : std::pair<std::optional<C>, std::optional<C>>{in, {}};
  else return std::pair<std::optional<C>, std::optional<C>>{{}, {}};
}

template <typename C> //
[[nodiscard]] constexpr auto take(const std::optional<C> &in, size_t n, tag = {}) -> std::optional<C> {
  return n == 0 ? std::optional<C>{std::nullopt} : in;
}

template <typename C> //
[[nodiscard]] constexpr auto drop(const std::optional<C> &in, size_t n, tag = {}) -> std::optional<C> {
  return n != 0 ? std::optional<C>{std::nullopt} : in;
}

template <typename C> //
[[nodiscard]] constexpr auto take_right(const std::optional<C> &in, size_t n, tag = {}) -> std::optional<C> {
  return n == 0 ? std::optional<C>{std::nullopt} : in;
}

template <typename C> //
[[nodiscard]] constexpr auto drop_right(const std::optional<C> &in, size_t n, tag = {}) -> std::optional<C> {
  return n != 0 ? std::optional<C>{std::nullopt} : in;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto take_while(const std::optional<C> &in, Predicate &&predicate, tag = {}) -> std::optional<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  // XXX don't use ternary here, NVHPC breaks: https://forums.developer.nvidia.com/t/nvc-miscompiles-code-involving-std-tuple/281754
  if (in && details::ap(predicate, *in)) return in;
  else return std::nullopt;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto drop_while(const std::optional<C> &in, Predicate &&predicate, tag = {}) -> std::optional<C> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  // XXX don't use ternary here, NVHPC breaks: https://forums.developer.nvidia.com/t/nvc-miscompiles-code-involving-std-tuple/281754
  if (in && !details::ap(predicate, *in)) return in;
  else return std::nullopt;
}

template <typename C, typename Predicate> //
[[nodiscard]] constexpr auto span(const std::optional<C> &in, Predicate &&predicate, tag = {})
    -> std::pair<std::optional<C>, std::optional<C>> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  // XXX don't use ternary here, NVHPC breaks: https://forums.developer.nvidia.com/t/nvc-miscompiles-code-involving-std-tuple/281754
  if (in && details::ap(predicate, *in)) return std::pair{in, std::nullopt};
  else return std::pair{std::nullopt, in};
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(const std::optional<C> &in, Accumulator &&init, Function &&function, tag = {}) -> Accumulator {
  return in ? function(init, *in) : init;
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(const std::optional<C> &in, Accumulator &&init, Function &&function, tag = {}) -> Accumulator {
  return in ? function(*in, init) : init;
}

template <typename C> //
[[nodiscard]] constexpr auto sliding(const std::optional<C> &in, size_t size, size_t step, tag = {}) -> std::optional<std::optional<C>> {
  if (size == 0 || step == 0)
    details::raise<std::range_error>("cannot apply windowing with zero size or step, size=" + std::to_string(size) =
                                         " step=" + std::to_string(step));
  return in ? std::optional<std::optional<C>>{*in} : std::optional<std::optional<C>>{std::nullopt};
}

// optional-specific

template <typename C, typename Function, typename FunctionEmpty> //
[[nodiscard]] constexpr auto fold(const std::optional<C> &o, Function f, FunctionEmpty empty, tag = {}) {
  using T = decltype(details::ap(f, *o));
  static_assert(std::is_convertible_v<decltype(empty()), T>, "f and empty does not unify");
  return o ? details::ap(f, *o) : static_cast<T>(empty());
}

template <typename C, typename FunctionEmpty> //
[[nodiscard]] constexpr auto fold(const std::optional<C> &o, FunctionEmpty empty, tag = {}) {
  static_assert(std::is_convertible_v<decltype(empty()), C>, "empty does not unify with value type");
  return o ? *o : static_cast<C>(empty());
}

template <typename C, typename T> //
[[nodiscard]] constexpr auto get_or_else(const std::optional<C> &o, const T &default_value, tag = {}) {
  static_assert(std::is_convertible_v<T, C>, "optional value type and default_value type does not unify");
  return o ? *o : static_cast<C>(default_value);
}

template <typename C> //
[[nodiscard]] constexpr auto get_or_default(const C &o, tag = {}) {
  static_assert(std::is_default_constructible_v<typename C::value_type>, "type is not default constructible");
  return o ? *o : typename C::value_type{};
}

template <typename C, typename U> //
[[nodiscard]] constexpr auto or_else_maybe(const std::optional<C> &o, const U &empty, tag = {}) -> std::optional<C> {
  static_assert(is_optional<U>, "other value is not an optional type");
  if constexpr (std::is_same_v<std::decay_t<U>, std::nullopt_t>) return o;
  else {
    static_assert(std::is_convertible_v<typename U::value_type, C>, "optional value type and other's value type does not unify");
    return o ? o : empty;
  }
}

template <typename C, typename F> //
[[nodiscard]] constexpr auto or_else(const std::optional<C> &o, const F &empty, tag = {}) -> std::optional<C> {
  using R = std::decay_t<decltype(empty())>;
  static_assert(is_optional<R>, "return type of other value functor is not an optional type");
  if constexpr (std::is_same_v<R, std::nullopt_t>) return o;
  else {
    static_assert(std::is_convertible_v<typename R::value_type, C>, "optional value type and other's value type does not unify");
    return o ? o : empty();
  }
}

} // namespace aspartame

#define ASPARTAME_IN_TYPE1(T) std::optional<T>
#define ASPARTAME_IN_TYPE2(K, V) std::optional<std::pair<K, V>>

#include "details/nop/map_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_IN_TYPE2
#undef ASPARTAME_IN_TYPE1