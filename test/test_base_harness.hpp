#pragma once

#include <initializer_list>
#include <type_traits>

#include "fixtures.hpp"

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef __FILE_NAME__
  #define __FILE_NAME__ __FILE__
#endif

namespace harness_detail {

template <typename T, typename = void> constexpr bool has_begin_end_v = false;
template <typename T>
constexpr bool has_begin_end_v<T, std::void_t<decltype(std::declval<T &>().begin()), decltype(std::declval<T &>().end())>> = true;

template <typename T, typename = void> constexpr bool has_key_type_v = false;
template <typename T> constexpr bool has_key_type_v<T, std::void_t<typename T::key_type>> = true;

template <typename C, typename = void> constexpr bool has_value_type_v = false;
template <typename C> constexpr bool has_value_type_v<C, std::void_t<typename C::value_type>> = true;

template <typename T, typename = void> constexpr bool is_lt_orderable_v = false;
template <typename T>
constexpr bool is_lt_orderable_v<T, std::void_t<decltype(std::declval<const T &>() < std::declval<const T &>())>> = true;

template <typename C, typename = void> constexpr bool wants_sort_compare_v = false;
template <typename C>
constexpr bool wants_sort_compare_v<
    C, std::void_t<typename C::value_type, decltype(std::declval<C &>().begin()), decltype(std::declval<C &>().end())>> =
    !has_key_type_v<C> && is_lt_orderable_v<typename C::value_type>;

template <typename C, typename T> C build_from_il(std::initializer_list<T> il) {
  if constexpr (std::is_constructible_v<C, std::initializer_list<T>>) {
    return C(il);
  } else if constexpr (std::is_constructible_v<C, decltype(il.begin()), decltype(il.end())>) {
    return C(il.begin(), il.end());
  } else if constexpr (std::is_default_constructible_v<C> && std::is_constructible_v<C, T>) {
    return il.size() == 0 ? C{} : C{*il.begin()};
  } else {
    static_assert(sizeof(C) == 0, "from_il: no viable construction of C from initializer_list<T>");
  }
}
} // namespace harness_detail

template <typename C, typename T = typename C::value_type, std::enable_if_t<harness_detail::has_value_type_v<C>, int> = 0>
C from_il(std::initializer_list<T> il) {
  return harness_detail::build_from_il<C, T>(il);
}
template <typename C, std::enable_if_t<!harness_detail::has_value_type_v<C>, int> = 0> C from_il(const C &v) { return v; }
