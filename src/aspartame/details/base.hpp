#pragma once

#if __cplusplus >= 202002L
  #define ASPARTAME_USE_CONCEPTS
#endif

#include <functional>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#ifdef ASPARTAME_USE_CONCEPTS
  #include <concepts>
#endif

namespace aspartame {

namespace details {
template <typename> constexpr bool is_tuple_impl = false;
template <typename... Args> constexpr bool is_tuple_impl<std::tuple<Args...>> = true;

template <typename> constexpr bool is_pair_impl = false;
template <typename T1, typename T2> constexpr bool is_pair_impl<std::pair<T1, T2>> = true;

template <typename> constexpr bool is_optional_impl = false;
template <typename T> constexpr bool is_optional_impl<std::optional<T>> = true;

template <typename> constexpr bool is_vector_impl = false;
template <typename T> constexpr bool is_vector_impl<std::vector<T>> = true;

template <typename T, typename = void> constexpr bool is_iterable_impl = false;
template <typename T>
constexpr bool is_iterable_impl<T, std::void_t<decltype(std::begin(std::declval<T &>())), decltype(std::end(std::declval<T &>()))>> = true;

template <typename, typename = void> constexpr bool is_map_like_impl = false;
template <typename T>
constexpr bool is_map_like_impl<T, std::void_t<typename T::key_type, typename T::mapped_type,
                                               decltype(std::declval<T &>()[std::declval<const typename T::key_type &>()])>> = true;

template <typename, typename = void> constexpr bool is_set_like_impl = false;
template <typename T>
constexpr bool is_set_like_impl<T, std::void_t<typename T::key_type, typename T::value_type,
                                               decltype(std::declval<T &>().emplace(std::declval<const typename T::value_type &&>()))>> =
    true;

template <typename T, typename = void> constexpr bool is_hashable_impl = false;
template <typename T> constexpr bool is_hashable_impl<T, std::void_t<decltype(std::declval<std::hash<T>>()(std::declval<T>()))>> = true;

template <typename T, typename = void> constexpr bool is_comparable_impl = false;
template <typename T> constexpr bool is_comparable_impl<T, std::void_t<decltype(std::declval<T &>() == std::declval<T &>())>> = true;

template <template <typename...> class, typename, typename = void> constexpr bool is_unary_instantiable = false;
template <template <typename...> class C, typename T> constexpr bool is_unary_instantiable<C, T, std::void_t<C<T>>> = true;

template <typename T> constexpr bool is_supported = false;

} // namespace details
template <typename T> constexpr bool is_tuple = details::is_tuple_impl<std::decay_t<T>>;
template <typename T> constexpr bool is_pair = details::is_pair_impl<std::decay_t<T>>;
template <typename T>
constexpr bool is_optional = std::is_same_v<std::decay_t<T>, std::nullopt_t> || details::is_optional_impl<std::decay_t<T>>;
template <typename T> constexpr bool is_vector = details::is_vector_impl<std::decay_t<T>>;
template <typename T> constexpr bool is_iterable = details::is_iterable_impl<std::decay_t<T>>;
template <typename T> constexpr bool is_map_like = details::is_map_like_impl<std::decay_t<T>>;
template <typename T> constexpr bool is_hashable = details::is_hashable_impl<std::decay_t<T>>;
template <typename T> constexpr bool is_comparable = details::is_comparable_impl<std::decay_t<T>>;

template <template <typename...> class C, typename T>
constexpr bool is_unary_instantiable = details::is_unary_instantiable<C, std::decay_t<T>>;

struct tag {};

namespace details {

template <typename T> struct unsupported1 {};

template <typename K, typename V> struct unsupported2 {};

template <typename F, typename Args> constexpr auto ap(F f, Args &&t) {
  using U = std::decay_t<Args>;
  if constexpr (is_tuple<Args> || is_pair<Args>) {
    if constexpr (std::is_invocable_v<F, U>) return f(std::forward<Args &&>(t));
    else return std::apply(f, std::forward<Args &&>(t));
  } else return f(std::forward<Args &&>(t));
}

template <typename In, typename... Args> constexpr auto unsupported(Args...) {
  static_assert(is_supported<In>, "operation is unsupported for type");
  return true;
}

template <typename T> constexpr bool assert_predicate() {
  static_assert(std::is_convertible_v<T, bool>, "predicate does not return a bool");
  return true;
}
template <typename T> constexpr bool assert_non_void() {
  static_assert(!std::is_same_v<T, void>, "function should return a non-void type");
  return true;
}
template <typename T> constexpr bool assert_void() {
  static_assert(std::is_same_v<T, void>, "function should return void type");
  return true;
}

template <typename T, typename = void> constexpr bool has_reserve = false;
template <typename T> constexpr bool has_reserve<T, std::void_t<decltype(std::declval<T>().reserve(std::declval<size_t>()))>> = true;

template <typename T, typename = void> constexpr bool has_sort = false;
template <typename T> constexpr bool has_sort<T, std::void_t<decltype(std::declval<T>().sort())>> = true;

template <typename T, typename = void> constexpr bool has_reverse = false;
template <typename T> constexpr bool has_reverse<T, std::void_t<decltype(std::declval<T>().reverse())>> = true;

template <typename T, typename = void> constexpr bool has_push_back = false;
template <typename T>
constexpr bool has_push_back<T, std::void_t<decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))>> = true;

template <typename T, typename = void> constexpr bool has_size = false;
template <typename T> constexpr bool has_size<T, std::void_t<decltype(std::declval<T>().size())>> = true;

template <typename T, typename = void> constexpr bool has_associative_insert = false;
template <typename T>
constexpr bool has_associative_insert<T, std::void_t<decltype(std::declval<T>().insert(std::declval<typename T::const_iterator>(),
                                                                                       std::declval<typename T::const_iterator>()))>> =
    true;

template <typename T, typename = void> constexpr bool has_assignable_iterator = false;
template <typename T>
constexpr bool has_assignable_iterator<T, std::void_t<decltype(*std::declval<T>() = *std::declval<T>())>> =
    std::is_assignable_v<decltype(*std::declval<T>()), decltype(*std::declval<T>())>;

template <typename T, typename = void> constexpr bool has_rbegin = false;
template <typename T> constexpr bool has_rbegin<T, std::void_t<decltype(std::declval<T>().rbegin())>> = true;

template <typename T, typename = void> constexpr bool has_rend = false;
template <typename T> constexpr bool has_rend<T, std::void_t<decltype(std::declval<T>().rend())>> = true;

template <typename T, typename = void> constexpr bool has_const_iterator = false;
template <typename T> constexpr bool has_const_iterator<T, std::void_t<typename T::const_iterator>> = true;

template <typename E, typename M> [[noreturn]] void raise(const M &message) {
#if __cpp_exceptions == 199711
  throw E(message);
#else
  std::fprintf(stderr, "%s\n", message.c_str());
  std::abort();
#endif
}

} // namespace details

} // namespace aspartame
