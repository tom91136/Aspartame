#pragma once

#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "fluent.hpp"
#include "traits.hpp"

#if __has_include(<version>)
  #include <version>
#endif

#if defined(__cpp_lib_expected)
  #include <expected>
#endif

namespace aspartame {

#if defined(__cpp_lib_expected)
template <typename V, typename E> struct expected_traits<std::expected<V, E>> {
  using value_type = V;
  using error_type = E;
  template <typename U> using rebind_value = std::expected<U, E>;
  template <typename E2> using rebind_error = std::expected<V, E2>;
  static constexpr bool engaged(const std::expected<V, E> &e) noexcept { return e.has_value(); }
  static constexpr const V &get(const std::expected<V, E> &e) noexcept { return *e; }
  static constexpr const E &error(const std::expected<V, E> &e) noexcept { return e.error(); }
  template <typename U, typename Arg> static constexpr rebind_value<U> make_value(Arg &&v) { return rebind_value<U>{std::forward<Arg>(v)}; }
  template <typename U> static constexpr rebind_value<U> propagate_error(const E &e) { return rebind_value<U>{std::unexpect, e}; }
  template <typename E2, typename Arg> static constexpr rebind_error<E2> propagate_value(Arg &&v) {
    return rebind_error<E2>{std::forward<Arg>(v)};
  }
  template <typename E2, typename Arg> static constexpr rebind_error<E2> make_error(Arg &&e) {
    return rebind_error<E2>{std::unexpect, std::forward<Arg>(e)};
  }
};
#endif

template <typename T> struct enable_pipe<T, std::enable_if_t<details::has_expected_traits_v<T>>> : std::true_type {};

namespace details {
template <typename C> using exp_value = typename expected_traits<std::decay_t<C>>::value_type;
template <typename C> using exp_error = typename expected_traits<std::decay_t<C>>::error_type;
template <typename C, typename U> using exp_rebind_value = typename expected_traits<std::decay_t<C>>::template rebind_value<U>;
template <typename C, typename E2> using exp_rebind_error = typename expected_traits<std::decay_t<C>>::template rebind_error<E2>;
template <typename C> using exp_same = exp_rebind_value<C, exp_value<C>>;
template <typename C> constexpr bool exp_engaged(const C &c) noexcept { return expected_traits<std::decay_t<C>>::engaged(c); }
template <typename C> constexpr const exp_value<C> &exp_get(const C &c) noexcept { return expected_traits<std::decay_t<C>>::get(c); }
template <typename C> constexpr const exp_error<C> &exp_err(const C &c) noexcept { return expected_traits<std::decay_t<C>>::error(c); }
} // namespace details

#define ASPARTAME_EXP_TPL(...) typename C, __VA_ARGS__, std::enable_if_t<details::has_expected_traits_v<C>, int> = 0
#define ASPARTAME_EXP_TPL_NOTYPE typename C, std::enable_if_t<details::has_expected_traits_v<C>, int> = 0

template <typename L, typename Op, std::enable_if_t<details::has_expected_traits_v<std::decay_t<L>>, int> = 0>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const L &, tag>
#endif
auto operator|(const L &l, const Op &r) {
  return r(l, tag{});
}

template <ASPARTAME_EXP_TPL(typename Function)> [[nodiscard]] constexpr auto map(const C &in, Function &&f, tag = {}) {
  using OT = expected_traits<std::decay_t<C>>;
  using U = std::decay_t<decltype(details::ap(f, details::exp_get(in)))>;
  if constexpr (details::assert_non_void<U>()) {}
  if (details::exp_engaged(in)) return OT::template make_value<U>(details::ap(f, details::exp_get(in)));
  return OT::template propagate_error<U>(details::exp_err(in));
}

template <ASPARTAME_EXP_TPL(typename Function)> [[nodiscard]] constexpr auto map_error(const C &in, Function &&g, tag = {}) {
  using OT = expected_traits<std::decay_t<C>>;
  using E2 = std::decay_t<decltype(g(details::exp_err(in)))>;
  if constexpr (details::assert_non_void<E2>()) {}
  if (!details::exp_engaged(in)) return OT::template make_error<E2>(g(details::exp_err(in)));
  return OT::template propagate_value<E2>(details::exp_get(in));
}

template <ASPARTAME_EXP_TPL(typename ValueFn, typename ErrorFn)>
[[nodiscard]] constexpr auto bimap(const C &in, ValueFn vfn, ErrorFn efn, tag = {}) {
  using OT = expected_traits<std::decay_t<C>>;
  using U = std::decay_t<decltype(vfn(details::exp_get(in)))>;
  using E2 = std::decay_t<decltype(efn(details::exp_err(in)))>;
  if constexpr (details::assert_non_void<U>() && details::assert_non_void<E2>()) {}
  using Result = typename expected_traits<typename OT::template rebind_value<U>>::template rebind_error<E2>;
  using ResultOT = expected_traits<Result>;
  if (details::exp_engaged(in)) return ResultOT::template make_value<U>(vfn(details::exp_get(in)));
  return ResultOT::template make_error<E2>(efn(details::exp_err(in)));
}

template <ASPARTAME_EXP_TPL(typename Predicate, typename ErrorFn)>
[[nodiscard]] constexpr auto ensure(const C &in, Predicate &&p, ErrorFn &&efn, tag = {}) {
  if (!details::exp_engaged(in)) return in;
  if (details::ap(p, details::exp_get(in))) return in;
  using OT = expected_traits<std::decay_t<C>>;
  return OT::template make_error<typename OT::error_type>(efn(details::exp_get(in)));
}

template <ASPARTAME_EXP_TPL(typename Predicate, typename ErrorFn)>
[[nodiscard]] constexpr auto filter(const C &in, Predicate &&p, ErrorFn &&efn, tag = {}) {
  return ensure(in, std::forward<Predicate>(p), std::forward<ErrorFn>(efn), tag{});
}

template <ASPARTAME_EXP_TPL_NOTYPE> [[nodiscard]] constexpr auto flatten(const C &in, tag = {}) {
  using Inner = details::exp_value<C>;
  static_assert(details::has_expected_traits_v<Inner>, "flatten requires the inner value to be an expected-like type");
  static_assert(std::is_same_v<details::exp_error<Inner>, details::exp_error<C>>, "flatten requires inner and outer error types to match");
  using InnerOT = expected_traits<std::decay_t<Inner>>;
  using E = details::exp_error<C>;
  if (details::exp_engaged(in)) return details::exp_get(in);
  return InnerOT::template make_error<E>(details::exp_err(in));
}

template <ASPARTAME_EXP_TPL(typename Other)> [[nodiscard]] constexpr auto concat(const C &in, const Other &other, tag = {}) {
  static_assert(details::has_expected_traits_v<std::decay_t<Other>>, "other must be an expected-like type");
  static_assert(std::is_same_v<details::exp_value<Other>, details::exp_value<C>>, "concat both sides must share value type");
  static_assert(std::is_same_v<details::exp_error<Other>, details::exp_error<C>>, "concat both sides must share error type");
  using V = details::exp_value<C>;
  static_assert(details::has_monoid_traits_v<V>, "concat requires a monoid_traits specialisation for the value type");
  using OT = expected_traits<std::decay_t<C>>;
  if (!details::exp_engaged(in)) return in;
  if (!details::exp_engaged(other)) return other;
  return OT::template make_value<V>(monoid_traits<V>::combine(details::exp_get(in), details::exp_get(other)));
}

template <ASPARTAME_EXP_TPL(typename Function)> [[nodiscard]] constexpr auto flat_map(const C &in, Function &&f, tag = {}) {
  using R = std::decay_t<decltype(details::ap(f, details::exp_get(in)))>;
  static_assert(details::has_expected_traits_v<R>, "flat_map function must return an expected-like type");
  static_assert(std::is_same_v<details::exp_error<R>, details::exp_error<C>>, "flat_map result must have the same error type");
  using OT = expected_traits<std::decay_t<C>>;
  using U = details::exp_value<R>;
  if (details::exp_engaged(in)) return details::ap(f, details::exp_get(in));
  return OT::template propagate_error<U>(details::exp_err(in));
}

template <ASPARTAME_EXP_TPL(typename Function)> [[nodiscard]] constexpr auto flat_map_error(const C &in, Function &&g, tag = {}) {
  using R = std::decay_t<decltype(g(details::exp_err(in)))>;
  static_assert(details::has_expected_traits_v<R>, "flat_map_error function must return an expected-like type");
  static_assert(std::is_same_v<details::exp_value<R>, details::exp_value<C>>, "flat_map_error result must have the same value type");
  using OT = expected_traits<std::decay_t<C>>;
  using E2 = details::exp_error<R>;
  if (details::exp_engaged(in)) return OT::template propagate_value<E2>(details::exp_get(in));
  return g(details::exp_err(in));
}

template <ASPARTAME_EXP_TPL(typename Function)> [[nodiscard]] constexpr auto or_else(const C &in, Function &&g, tag = {}) {
  using R = std::decay_t<decltype(g(details::exp_err(in)))>;
  static_assert(details::has_expected_traits_v<R>, "or_else function must return an expected-like type");
  static_assert(std::is_same_v<details::exp_value<R>, details::exp_value<C>>, "or_else result must have the same value type");
  using OT = expected_traits<std::decay_t<C>>;
  using E2 = details::exp_error<R>;
  if (details::exp_engaged(in)) return OT::template propagate_value<E2>(details::exp_get(in));
  return g(details::exp_err(in));
}

template <ASPARTAME_EXP_TPL(typename Other)> [[nodiscard]] constexpr auto or_else_maybe(const C &in, const Other &other, tag = {}) {
  static_assert(details::has_expected_traits_v<std::decay_t<Other>>, "other must be an expected-like type");
  static_assert(std::is_same_v<details::exp_value<Other>, details::exp_value<C>>, "or_else_maybe other must have the same value type");
  static_assert(std::is_same_v<details::exp_error<Other>, details::exp_error<C>>,
                "or_else_maybe other must have the same error type as the receiver");
  return details::exp_engaged(in) ? in : other;
}

template <ASPARTAME_EXP_TPL(typename T)> [[nodiscard]] constexpr auto get_or_else(const C &in, const T &default_value, tag = {}) {
  using V = details::exp_value<C>;
  static_assert(std::is_convertible_v<T, V>, "default_value type does not unify with value_type");
  return details::exp_engaged(in) ? details::exp_get(in) : static_cast<V>(default_value);
}

template <ASPARTAME_EXP_TPL_NOTYPE> [[nodiscard]] constexpr auto get_or_default(const C &in, tag = {}) {
  using V = details::exp_value<C>;
  static_assert(std::is_default_constructible_v<V>, "value_type is not default constructible");
  return details::exp_engaged(in) ? details::exp_get(in) : V{};
}

template <ASPARTAME_EXP_TPL(typename ValueFn, typename ErrorFn)>
[[nodiscard]] constexpr auto fold(const C &in, ValueFn vfn, ErrorFn efn, tag = {}) {
  using TV = decltype(vfn(details::exp_get(in)));
  using TE = decltype(efn(details::exp_err(in)));
  static_assert(std::is_convertible_v<TE, TV>, "value_fn and error_fn return types do not unify");
  if (details::exp_engaged(in)) return TV{vfn(details::exp_get(in))};
  return static_cast<TV>(efn(details::exp_err(in)));
}

template <ASPARTAME_EXP_TPL(typename Predicate)> [[nodiscard]] constexpr auto exists(const C &in, Predicate &&p, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(p, details::exp_get(in)))>()) {}
  return details::exp_engaged(in) && details::ap(p, details::exp_get(in));
}

template <ASPARTAME_EXP_TPL(typename Predicate)> [[nodiscard]] constexpr auto forall(const C &in, Predicate &&p, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(p, details::exp_get(in)))>()) {}
  return !details::exp_engaged(in) || details::ap(p, details::exp_get(in));
}

template <ASPARTAME_EXP_TPL(typename Predicate)> [[nodiscard]] constexpr auto none_match(const C &in, Predicate &&p, tag = {}) -> bool {
  if constexpr (details::assert_predicate<decltype(details::ap(p, details::exp_get(in)))>()) {}
  return !(details::exp_engaged(in) && details::ap(p, details::exp_get(in)));
}

template <ASPARTAME_EXP_TPL(typename Predicate)> [[nodiscard]] constexpr auto count(const C &in, Predicate &&p, tag = {}) -> size_t {
  if constexpr (details::assert_predicate<decltype(details::ap(p, details::exp_get(in)))>()) {}
  return details::exp_engaged(in) && details::ap(p, details::exp_get(in)) ? 1 : 0;
}

template <ASPARTAME_EXP_TPL(typename U)> [[nodiscard]] constexpr auto contains(const C &in, const U &u, tag = {}) -> bool {
  using V = details::exp_value<C>;
  static_assert(std::is_convertible_v<std::decay_t<U>, V>, "value to contain does not match value_type");
  return details::exp_engaged(in) && details::exp_get(in) == u;
}

template <ASPARTAME_EXP_TPL(typename Function)> [[nodiscard]] constexpr auto tap_each(const C &in, Function &&f, tag = {}) {
  if constexpr (details::assert_void<decltype(details::ap(f, details::exp_get(in)))>()) {}
  if (details::exp_engaged(in)) details::ap(f, details::exp_get(in));
  return in;
}

template <ASPARTAME_EXP_TPL(typename Function)> [[nodiscard]] constexpr auto tap_error(const C &in, Function &&g, tag = {}) {
  if constexpr (details::assert_void<decltype(g(details::exp_err(in)))>()) {}
  if (!details::exp_engaged(in)) g(details::exp_err(in));
  return in;
}

template <ASPARTAME_EXP_TPL(typename Function)> constexpr auto for_each(const C &in, Function &&f, tag = {}) -> void {
  if constexpr (details::assert_void<decltype(details::ap(f, details::exp_get(in)))>()) {}
  if (details::exp_engaged(in)) details::ap(f, details::exp_get(in));
}

template <ASPARTAME_EXP_TPL_NOTYPE> [[nodiscard]] constexpr auto to_optional(const C &in, tag = {}) {
  using V = details::exp_value<C>;
  return details::exp_engaged(in) ? std::optional<V>{details::exp_get(in)} : std::optional<V>{};
}

template <ASPARTAME_EXP_TPL_NOTYPE> [[nodiscard]] constexpr auto to_optional_error(const C &in, tag = {}) {
  using E = details::exp_error<C>;
  return details::exp_engaged(in) ? std::optional<E>{} : std::optional<E>{details::exp_err(in)};
}

template <ASPARTAME_EXP_TPL_NOTYPE> [[nodiscard]] constexpr auto to_vector(const C &in, tag = {}) {
  using V = details::exp_value<C>;
  return details::exp_engaged(in) ? std::vector<V>{details::exp_get(in)} : std::vector<V>{};
}

#undef ASPARTAME_EXP_TPL
#undef ASPARTAME_EXP_TPL_NOTYPE

} // namespace aspartame
