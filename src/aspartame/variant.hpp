#pragma once

#include "details/base.hpp"
#include "fluent.hpp"

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>

namespace aspartame {

namespace {
template <typename... Ts> class alternatives {
  template <class T> struct id {
    using type = T;
  };

public:
  template <typename T, typename... Us> static constexpr bool all_unique_impl() {
    if constexpr (sizeof...(Us) == 0) return true;
    else return (!std::is_same_v<T, Us> && ...) && all_unique_impl<Us...>();
  }

  template <size_t N, typename T, typename... Us> static constexpr auto at_impl() {
    if constexpr (N == 0) return id<T>();
    else return at_impl<N - 1, Us...>();
  }

  static constexpr size_t size = sizeof...(Ts);
  template <typename T> static constexpr bool contains = (std::is_same_v<T, Ts> || ...);
  template <typename T> static constexpr bool all = (std::is_same_v<T, Ts> && ...);
  static constexpr bool all_unique = all_unique_impl<Ts...>();
  template <size_t N> using at = typename decltype(at_impl<N, Ts...>())::type;
};

template <typename F, typename R, typename A, typename... Ts> A arg1_impl(R (F::*)(A, Ts...));
template <typename F, typename R, typename A, typename... Ts> A arg1_impl(R (F::*)(A, Ts...) const);
template <typename F> using arg1_t = decltype(arg1_impl(&F::operator()));

//template <typename F> struct arg1 {
//  using type = decltype(arg1_impl(&F::operator()));
//};
//template <typename T> using arg1_t = typename arg1<T>::type;



} // namespace

template <typename... C, typename Op> auto operator^(const std::variant<C...> &l, const Op &r) { return r(l); }

// ---

template <typename T, typename... Cs> //
[[nodiscard]] constexpr auto get(const std::variant<Cs...> &o, tag = {}) -> std::optional<T> {
  static_assert(alternatives<Cs...>::template contains<T>, "one or more cases not part of the variant");
  if (std::holds_alternative<T>(o)) return std::get<T>(o);
  else return std::nullopt;
}

namespace {
template <bool total, typename... Cs, typename... Fs> //
[[nodiscard]] constexpr auto fold_impl(const std::variant<Cs...> &o, Fs... f) {
  // partial cases return the same type (arg0)
  using Ts = alternatives<std::decay_t<arg1_t<Fs>>...>;
  using Rs = alternatives<std::invoke_result_t<Fs, std::decay_t<arg1_t<Fs>>>...>;
  using R0 = typename Rs::template at<0>;
  if constexpr (total) static_assert(sizeof...(Cs) == sizeof...(Fs), "match is not total as case count is not equal to variant's size");
  static_assert((alternatives<Cs...>::template contains<std::decay_t<arg1_t<Fs>>> && ...), "one or more cases not part of the variant");
  static_assert((Rs::template all<R0>), "all cases must return the same type");
  static_assert(Ts::all_unique, "one or more cases overlap");
  static_assert(!std::is_void_v<R0>,
                "all cases should return a non-void type in fold_{total|partial}, use foreach_{total|partial} instead");
  std::optional<R0> r;
  ([&]() -> bool {
    using T = std::decay_t<arg1_t<Fs>>;
    if (std::holds_alternative<T>(o)) {
      r = std::move(f(std::get<T>(o)));
      return true;
    }
    return false;
  }() || ...);
  if constexpr (total) return std::move(*r);
  else return  (r);
}
} // namespace

template <typename... Cs, typename... Fs> //
[[nodiscard]] constexpr auto fold_total(tag, const std::variant<Cs...> &o, Fs... f) {
  return fold_impl<true>(o, f...);
}

template <typename... Cs, typename... Fs> //
[[nodiscard]] constexpr auto fold_partial(tag, const std::variant<Cs...> &o, Fs... f) {
  return fold_impl<false>(o, f...);
}

namespace {
template <bool total, typename... Cs, typename... Fs> //
constexpr auto foreach_impl(const std::variant<Cs...> &o, Fs... f) -> void {
  // total cases return void
  using Ts = alternatives<std::decay_t<arg1_t<Fs>>...>;
  using Rs = alternatives<std::invoke_result_t<Fs, std::decay_t<arg1_t<Fs>>>...>;
  using R0 = typename Rs::template at<0>;
  if constexpr (total) static_assert(sizeof...(Cs) == sizeof...(Fs), "match is not total as case count is not equal to variant's size");
  static_assert((alternatives<Cs...>::template contains<std::decay_t<arg1_t<Fs>>> && ...), "one or more cases not part of the variant");
  static_assert((Rs::template all<R0>), "all cases must return the same type");
  static_assert(Ts::all_unique, "one or more cases overlap");
  static_assert(std::is_void_v<R0>, "all cases should return void in foreach_{total|partial}, use fold_{total|partial} instead");
  ([&]() -> bool {
    using T = std::decay_t<arg1_t<Fs>>;
    if (std::holds_alternative<T>(o)) {
      f(std::get<T>(o));
      return true;
    }
    return false;
  }() || ...);
}
} // namespace

template <typename... Cs, typename... Fs> //
constexpr auto foreach_total(tag, const std::variant<Cs...> &o, Fs... f) -> void {
  foreach_impl<true>(o, f...);
}

template <typename... Cs, typename... Fs> //
constexpr auto foreach_partial(tag, const std::variant<Cs...> &o, Fs... f) -> void {
  foreach_impl<false>(o, f...);
}

// #define ASPARTAME_IN_TYPE1(T) std::basic_string<T>
// #define ASPARTAME_IN_TYPE2(K, V) std::basic_string<std::pair<K, V>>
//
// #include "details/nop/map_template.hpp"
// #include "details/nop/string_template.hpp"
//
// #undef ASPARTAME_IN_TYPE2
// #undef ASPARTAME_IN_TYPE1

} // namespace aspartame
