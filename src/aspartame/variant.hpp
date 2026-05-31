#pragma once

#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename... Ts> struct enable_pipe<std::variant<Ts...>> : std::true_type {};

namespace details {

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

template <typename, typename = void> constexpr bool has_typed_call_op = false;
template <typename F> constexpr bool has_typed_call_op<F, std::void_t<decltype(&F::operator())>> = true;

template <typename F> using arg1_t = decltype(arg1_impl(&F::operator()));

} // namespace details

template <typename T, typename... Cs> [[nodiscard]] constexpr auto get_maybe(const std::variant<Cs...> &o, tag = {}) -> std::optional<T> {
  static_assert(details::alternatives<Cs...>::template contains<T>, "type is not part of the variant");
  if (std::holds_alternative<T>(o)) return std::get<T>(o);
  else return std::nullopt;
}

template <typename T, typename... Cs> [[nodiscard]] constexpr auto holds(const std::variant<Cs...> &o, tag = {}) -> bool {
  static_assert(details::alternatives<Cs...>::template contains<T>, "type is not part of the variant");
  return std::holds_alternative<T>(o);
}

template <typename... T, typename... Cs> [[nodiscard]] constexpr auto holds_any(const std::variant<Cs...> &o, tag = {}) -> bool {
  static_assert((details::alternatives<Cs...>::template contains<T> && ...), "one or more types is not part of the variant");
  return (std::holds_alternative<T>(o) || ...);
}

template <typename... T, typename... Cs>
[[nodiscard]] constexpr auto narrow(const std::variant<Cs...> &o, tag = {}) -> std::optional<std::variant<T...>> {
  static_assert((details::alternatives<Cs...>::template contains<T> && ...),
                "one or more types in target variant are not present in the source variant");
  return std::visit(
      [](auto &&arg) -> std::optional<std::variant<T...>> {
        using U = std::decay_t<decltype(arg)>;
        if constexpr ((std::is_same_v<U, T> || ...)) return std::variant<T...>{std::in_place_type<U>, std::forward<decltype(arg)>(arg)};
        else return std::nullopt;
      },
      o);
}

namespace details {
template <bool total, typename... Cs, typename... Fs> [[nodiscard]] constexpr auto fold_impl(const std::variant<Cs...> &o, Fs... f) {
  static_assert((has_typed_call_op<Fs> && ...),
                "fold/foreach cases must be typed lambdas like [](const T &){...}, generic auto&& lambdas are not supported");
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
      r = f(std::get<T>(o));
      return true;
    }
    return false;
  }() || ...);
  if constexpr (total) return std::move(*r);
  else return r;
}
} // namespace details

template <typename... Cs, typename... Fs> [[nodiscard]] constexpr auto fold_total(tag, const std::variant<Cs...> &o, Fs... f) {
  return details::fold_impl<true>(o, std::move(f)...);
}

template <typename... Cs, typename... Fs> [[nodiscard]] constexpr auto fold_partial(tag, const std::variant<Cs...> &o, Fs... f) {
  return details::fold_impl<false>(o, std::move(f)...);
}

namespace details {
template <bool total, typename... Cs, typename... Fs> constexpr auto foreach_impl(const std::variant<Cs...> &o, Fs... f) -> void {
  static_assert((has_typed_call_op<Fs> && ...),
                "fold/foreach cases must be typed lambdas like [](const T &){...}, generic auto&& lambdas are not supported");
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
} // namespace details

template <typename... Cs, typename... Fs> constexpr auto foreach_total(tag, const std::variant<Cs...> &o, Fs... f) -> void {
  details::foreach_impl<true>(o, std::move(f)...);
}

template <typename... Cs, typename... Fs> constexpr auto foreach_partial(tag, const std::variant<Cs...> &o, Fs... f) -> void {
  details::foreach_impl<false>(o, std::move(f)...);
}

} // namespace aspartame
