#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include "details/base.hpp"

namespace aspartame {

template <typename, typename = void> struct enable_pipe : std::false_type {};

template <typename> struct enable_string_ops : std::false_type {};

template <typename T> struct sequence_traits;
template <typename T> struct map_traits;
template <typename T> struct optional_traits;
template <typename T> struct expected_traits;
template <typename T, typename = void> struct monoid_traits;

template <typename T> struct monoid_traits<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
  static constexpr T empty() noexcept { return T{}; }
  static constexpr T combine(T a, T b) { return a + b; }
};

template <typename M> struct map_access {
  static const M &entries(const M &m) { return m; }
  template <typename K, typename V> static void put(M &out, K &&k, V &&v) { out.insert({std::forward<K>(k), std::forward<V>(v)}); }
  template <typename Key> static std::optional<typename M::mapped_type> lookup(const M &m, const Key &key) {
    if (auto it = m.find(key); it != m.end()) return it->second;
    else return std::nullopt;
  }
};

template <typename C> struct sequence_access {
  static constexpr const C &iterate(const C &c) { return c; }
};

namespace details {
template <typename C> constexpr decltype(auto) seq_view(const C &c) { return sequence_access<C>::iterate(c); }

// XXX prefer over `typename C::value_type` so trait-supplied value_types (e.g. llvm::iterator_range) unify with member-supplied ones
template <typename C>
using seq_iter_value_type_t = typename std::iterator_traits<decltype(seq_view(std::declval<const C &>()).begin())>::value_type;
template <typename C> using seq_value_type_t = std::remove_cv_t<seq_iter_value_type_t<C>>;
} // namespace details

namespace details {

template <typename T, typename = void> constexpr bool has_sequence_traits_v = false;
template <typename T> constexpr bool has_sequence_traits_v<T, std::void_t<typename sequence_traits<T>::value_type>> = true;

template <typename T, typename = void> constexpr bool has_map_traits_v = false;
template <typename T>
constexpr bool has_map_traits_v<T, std::void_t<typename map_traits<T>::key_type, typename map_traits<T>::mapped_type>> = true;

template <typename T, typename = void> constexpr bool has_optional_traits_v = false;
template <typename T> constexpr bool has_optional_traits_v<T, std::void_t<typename optional_traits<T>::value_type>> = true;

template <typename T, typename = void> constexpr bool has_expected_traits_v = false;
template <typename T>
constexpr bool has_expected_traits_v<T, std::void_t<typename expected_traits<T>::value_type, typename expected_traits<T>::error_type>> =
    true;

template <typename T, typename = void> constexpr bool has_monoid_traits_v = false;
template <typename T>
constexpr bool has_monoid_traits_v<
    T, std::void_t<decltype(monoid_traits<T>::empty()), decltype(monoid_traits<T>::combine(std::declval<T>(), std::declval<T>()))>> = true;

template <typename T, typename = void> constexpr bool has_value_insert_v = false;
template <typename T>
constexpr bool has_value_insert_v<T, std::void_t<decltype(std::declval<T &>().insert(std::declval<const typename T::value_type &>()))>> =
    true;

template <typename Out, typename V> constexpr void push(Out &out, V &&v) {
  if constexpr (has_push_back<Out>) out.push_back(std::forward<V>(v));
  else if constexpr (has_value_insert_v<Out>) out.insert(std::forward<V>(v));
  else out.insert(out.end(), std::forward<V>(v));
}

template <typename Out> constexpr void reserve_if(Out &out, size_t n) {
  if constexpr (has_reserve<Out>) out.reserve(n);
  else (void)n;
}

} // namespace details

} // namespace aspartame
