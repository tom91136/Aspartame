#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "details/base.hpp"

#ifdef ASPARInAME_FINALISED
  #error "Inhis header must be included before aspartame/fluent.hpp"
#endif

#define ASPARTAME_OPTIONAL(In, op, ...)                                                                                                    \
  if constexpr (is_optional<In>) { return optional_##op(__VA_ARGS__); }

namespace aspartame {

template <typename In, typename Op> auto operator^(const std::optional<In> &l, const Op &r) { return r(l); }

// ---
#define ASPARTAME_PREFIX(name) optional_##name

template <typename In, typename Function> //
[[nodiscard]] auto ASPARTAME_PREFIX(mk_string)(const In &in, const std::string_view &, const std::string_view &prefix,
                                               const std::string_view &suffix, Function &&function) {
  static_assert(std::is_convertible_v<decltype(details::ap(function, *in)), std::string>, "function does not return a string type");
  std::string out{prefix};
  if (in) out += details::ap(function, *in);
  out += suffix;
  return out;
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(append)(const In &in, const T &t) {
  using U = typename In::value_type;
  static_assert(std::is_convertible_v<std::decay_t<T>, U>, "value type does not unify with optional's value type");
  return in ? std::vector<U>{*in, t} : std::vector<U>{t};
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(concat)(const In &in, const Container &other) {
  using U = typename In::value_type;
  static_assert(is_optional<std::decay_t<Container>>, "container is not an optional type");
  static_assert(std::is_convertible_v<typename std::decay_t<Container>::value_type, U>,
                "value type does not unify with optional's value type");
  if (!in) return other ? std::vector<U>{*other} : std::vector<U>{};
  else {
    if constexpr (std::is_default_constructible_v<U>) {
      std::vector<U> ys(1 + (other ? 1 : 0));
      ys[0] = *in;
      if (other) ys[1] = *other;
      return ys;
    } else {
      std::vector<U> ys;
      ys.reserve(1 + (other ? 1 : 0));
      ys.push_back(*in);
      if (other) ys.push_back(*other);
      return ys;
    }
  }
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(map)(const In &in, Function &&function) {
  using RV = decltype(details::ap(function, *in));
  if constexpr (details::assert_non_void<RV>()) {};
  if (in) return std::optional<RV>{details::ap(function, *in)};
  else return std::optional<RV>{};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(collect)(const In &in, Function &&function) {
  static_assert(is_optional<decltype(details::ap(function, *in))>, "collect function should return an optional");
  return in ? details::ap(function, *in) : std::nullopt;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(filter)(const In &in, Predicate &&predicate) {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  if (in) { return details::ap(predicate, *in) ? in : std::nullopt; }
  else return In{};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(bind)(const In &in, Function &&function) {
  using R = decltype(details::ap(function, *in));
  static_assert(is_optional<decltype(details::ap(function, *in))>, "f should return an optional");
  return in ? R{details::ap(function, *in)} : R{};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(flatten)(const In &in) -> typename std::decay_t<In>::value_type {
  static_assert(is_optional<typename In::value_type>, "not a nested optional type");
  if (in) return *in;
  else return std::nullopt;
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(distinct_by)(const In &in, Function &&) {
  return in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(distinct)(const In &in) {
  return in;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr size_t ASPARTAME_PREFIX(count)(const In &in, Predicate &&predicate) {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  return in && details::ap(predicate, *in) ? 1 : 0;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(exists)(const In &in, Predicate &&predicate) {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  return in && details::ap(predicate, *in);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(forall)(const In &in, Predicate p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in))>()) {};
  return !in || details::ap(p, *in);
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(reduce)(const In &in, Function &&) {
  return in;
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(tap_each)(const In &in, Function &&function) {
  if constexpr (details::assert_void<decltype(details::ap(function, *in))>()) {}
  if (in) details::ap(function, *in);
  return in;
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(for_each)(const In &in, Function &&function) {
  if constexpr (details::assert_void<decltype(details::ap(function, *in))>()) {}
  if (in) details::ap(function, *in);
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(partition)(const In &in, Predicate &&predicate) -> std::pair<In, In> {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {}
  return (in && details::ap(predicate, *in)) ? std::pair{in, In{}} : std::pair{In{}, in};
}

template <typename In, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_map_reduce)(const In &in, GroupFunction &&group, MapFunction &&map,
                                                                ReduceFunction &&reduce) {
  using K = decltype(details::ap(group, *in));
  using V = decltype(details::ap(map, *in));
  using R = decltype(details::ap(reduce, std::forward_as_tuple(std::declval<V>(), std::declval<V>())));
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  if constexpr (details::assert_non_void<R>()) {}
  return in ? std::unordered_map<K, V>{{details::ap(group, *in), details::ap(map, *in)}} : std::unordered_map<K, V>{};
}

template <typename In, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_map)(const In &in, GroupFunction &&group, MapFunction &&map) {
  using K = decltype(details::ap(group, *in));
  using V = decltype(details::ap(map, *in));
  if constexpr (details::assert_non_void<K>()) {}
  if constexpr (details::assert_non_void<V>()) {}
  return in ? std::unordered_map<K, std::optional<V>>{{details::ap(group, *in), std::optional{details::ap(map, *in)}}}
            : std::unordered_map<K, std::optional<V>>{};
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(group_by)(const In &in, Function &&function) {
  using K = decltype(details::ap(function, *in));
  using V = typename In::value_type;
  if constexpr (details::assert_non_void<K>()) {}
  return in ? std::unordered_map<K, std::optional<V>>{{details::ap(function, *in), std::optional{*in}}}
            : std::unordered_map<K, std::optional<V>>{};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(to_vector)(const In &o) {
  using T = typename In::value_type;
  return o ? std::vector<T>{*o} : std::vector<T>{};
}

// =======

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(prepend)(const In &in, const T &t) {
  using U = typename In::value_type;
  static_assert(std::is_convertible_v<std::decay_t<T>, U>, "value type does not unify with optional's value type");
  return in ? std::vector<U>{t, *in} : std::vector<U>{t};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(head_maybe)(const In &in) -> In {
  return in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(last_maybe)(const In &in) -> In {
  return in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(init)(const In &) -> In {
  return {};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(tail)(const In &) -> In {
  return {};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(at_maybe)(const In &o, size_t idx) -> In {
  return idx == 0 ? o : In{};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(slice)(const In &in, size_t from_inclusive, size_t to_exclusive) {
  return (!in || from_inclusive > 0 || to_exclusive == 0) ? In{} : in;
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_of_slice)(const In &in, const Container &other) -> std::make_signed_t<size_t> {
  static_assert(is_optional<Container>, "other type is not an optional");
  static_assert(std::is_same_v<typename Container::value_type, typename In::value_type>, "cannot unify value type on both sides");
  return in == other ? 0 : -1;
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(contains_slice)(const In &in, const Container &other) {
  return optional_index_of_slice(in, other) != -1;
}

template <typename In, typename U> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_of)(const In &o, const U &u) -> std::make_signed_t<size_t> {
  using V = std::decay_t<typename In::value_type>;
  static_assert(std::is_convertible_v<std::decay_t<U>, V>, "type does not match optional's value type");
  return o && *o == u ? 0 : -1;
}

template <typename In, typename U> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(contains)(const In &o, const U &u) -> bool {
  return optional_index_of(o, u) != -1;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(index_where)(const In &o, Predicate &&predicate) -> std::make_signed_t<size_t> {
  return o && details::ap(predicate, *o) ? 0 : -1;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(zip_with_index)(const In &in) {
  using R = std::optional<std::pair<std::decay_t<decltype(*in)>, size_t>>;
  return in ? R{{*in, 0}} : R{};
}

template <typename In, typename Container> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(zip)(const In &in, Container &&container) {
  static_assert(is_optional<Container>, "other value is not an optional type");
  using R = std::optional<std::pair<std::decay_t<decltype(*in)>, typename std::decay_t<Container>::value_type>>;
  return in && container ? R{{*in, *container}} : R{};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(transpose)(const In &in) {
  static_assert(is_optional<typename In::value_type>, "not a nested optional type");
  return in && *in ? in : In{};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(reverse)(const In &in) -> In {
  return in;
}

template <typename In, typename URBG> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(shuffle)(const In &in, URBG &) -> In {
  return in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort)(const In &in) -> In {
  return in;
}

template <typename In, typename Compare> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort)(const In &in, Compare &&) -> In {
  return in;
}

template <typename In, typename Select> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sort_by)(const In &in, Select &&) -> In {
  return in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(split_at)(const In &in, size_t idx) {
  if (in) return idx == 0 ? std::pair<In, In>{{}, in} : std::pair<In, In>{in, {}};
  else return std::pair<In, In>{{}, {}};
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take)(const In &in, size_t n) -> In {
  return n == 0 ? In{} : in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop)(const In &in, size_t n) -> In {
  return n != 0 ? In{} : in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take_right)(const In &in, size_t n) -> In {
  return n == 0 ? In{} : in;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop_right)(const In &in, size_t n) -> In {
  return n != 0 ? In{} : in;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(take_while)(const In &in, Predicate &&predicate) {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  return in && details::ap(predicate, *in) ? in : In{};
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(drop_while)(const In &in, Predicate &&predicate) {
  if constexpr (details::assert_predicate<decltype(details::ap(predicate, *in))>()) {};
  return in && !details::ap(predicate, *in) ? in : In{};
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold_left)(const In &in, Accumulator &&init, Function &&function) {
  return in ? details::ap(function, std::forward_as_tuple(init, *in)) : init;
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold_right)(const In &in, Accumulator &&init, Function &&function) {
  return in ? details::ap(function, std::forward_as_tuple(*in, init)) : init;
}

template <typename In> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(sliding)(const In &in, size_t size, size_t step) {
  if (size == 0 || step == 0)
    throw std::range_error("cannot apply windowing with zero size or step, size=" + std::to_string(size) = " step=" + std::to_string(step));
  return in ? std::optional<std::optional<typename In::value_type>>{*in}
            : std::optional<std::optional<typename In::value_type>>{std::nullopt};
}

// optional-specific

template <typename In, typename Function, typename FunctionEmpty> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(fold)(const In &o, Function f, FunctionEmpty empty) {
  using T = decltype(details::ap(f, *o));
  static_assert(std::is_convertible_v<decltype(empty()), T>, "f and empty does not unify");
  if (o) return details::ap(f, *o);
  else return static_cast<T>(empty());
}

template <typename In, typename T> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(get_or_else)(const In &o, const T &default_value) -> typename In::value_type {
  static_assert(std::is_convertible_v<T, typename In::value_type>, "optional value type and default_value type does not unify");
  return o ? *o : static_cast<typename In::value_type>(default_value);
}

template <typename In, typename U> //
[[nodiscard]] constexpr auto ASPARTAME_PREFIX(or_else)(const In &o, const U &empty) -> In {
  static_assert(is_optional<U>, "other value is not an optional type");
  if constexpr (std::is_same_v<std::decay_t<U>, std::nullopt_t>) return o;
  else {
    static_assert(std::is_convertible_v<typename U::value_type, typename In::value_type>,
                  "optional value type and other's value type does not unify");
    return o ? o : empty;
  }
}

#include "details/nop/map_template.hpp"
#include "details/nop/string_template.hpp"

} // namespace aspartame

#undef ASPARTAME_PREFIX
