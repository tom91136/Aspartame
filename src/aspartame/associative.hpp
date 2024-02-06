#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "details/base.hpp"

#define ASPARTAME_ASSOCIATIVE(T, op, ...)                                                                                                  \
  if constexpr (is_map_like<T>) { return associative_##op(__VA_ARGS__); }

namespace aspartame {

namespace details {

template <typename, typename = void> constexpr bool is_map_like_impl = false;

template <typename T>
constexpr bool is_map_like_impl<T, std::void_t<typename T::key_type, typename T::mapped_type,
                                               decltype(std::declval<T &>()[std::declval<const typename T::key_type &>()])>> = true;
} // namespace details

template <typename T> constexpr bool is_map_like = details::is_map_like_impl<std::decay_t<T>>;

// --

namespace {
template <typename operation> constexpr bool operation_on = false;
constexpr auto unsupported_op = [](auto type) -> auto {
  static_assert(operation_on<decltype(type)>,
                "not supported for associative containers, convert to a sequenced container (eg. to_vector) first");
};
} // namespace

template <typename O> //
constexpr auto associative_reverse(const O &o) {
  return unsupported_op(o);
}

template <typename O> //
constexpr auto associative_sort(const O &o) {
  return unsupported_op(o);
}

template <typename O, typename Compare> //
constexpr auto associative_sort(const O &o, Compare c) {
  return unsupported_op(o);
}

template <typename O, typename Select> //
constexpr auto associative_sort_by(const O &o, Select s) {
  return unsupported_op(o);
}

template <typename O> //
constexpr auto associative_head_maybe(const O &o) {
  return unsupported_op(o);
}

template <typename O> //
constexpr auto associative_last_maybe(const O &o) {
  return unsupported_op(o);
}

template <typename O> //
constexpr auto associative_at_maybe(const O &o, size_t &&idx) {
  return unsupported_op(o);
}

template <typename O, typename U> //
constexpr auto associative_index_of(const O &o, U &&u) -> std::make_signed_t<size_t> {
  return unsupported_op(o);
}

template <typename O, typename Predicate> //
constexpr auto associative_index_where(const O &o, Predicate p) -> std::make_signed_t<size_t> {
  return unsupported_op(o);
}

template <typename O, typename Function> auto associative_mk_string(const O &o, const std::string_view &sep, Function f) {
  static_assert(false);
}

template <typename O, typename Function> //
constexpr auto associative_collect(const O &o, Function f) {
  static_assert(false);
}

template <typename O, typename Predicate> //
constexpr auto associative_filter(const O &o, Predicate p) {
  static_assert(false);
}

template <typename O, typename Function> //
constexpr auto associative_bind(const O &o, Function f) {
  static_assert(false);
}

template <typename O, typename Predicate> //
constexpr auto associative_exists(const O &o, Predicate p) {
  static_assert(false);
}

template <typename O, typename Predicate> //
constexpr auto associative_forall(const O &o, Predicate p) {
  static_assert(false);
}

template <typename O> //
constexpr auto associative_zip_with_index(const O &o) {
  return unsupported_op(o);
}

template <typename O, typename Function> //
constexpr auto associative_map(const O &o, Function f) {
  static_assert(false);
}

template <typename O, typename Function> //
constexpr auto associative_tap_each(const O &o, Function f) {
  static_assert(false);
}

template <typename O, typename Function> //
constexpr auto associative_for_each(const O &o, Function f) {
  static_assert(false);
}

template <typename O> //
constexpr auto associative_flatten(const O &o) {
  static_assert(false);
}

template <typename O, typename Function0, typename Function> //
constexpr auto associative_fold(const O &o, Function0 f, Function empty) {
  details::unsupported(o);
}

template <typename O, typename T> //
constexpr auto associative_or_else(const O &o, T &&empty) {
  details::unsupported(o);
}

template <typename O> //
constexpr auto associative_to_vector(const O &o) {
  //  std::unordered_map<int, std::string>::
  std::vector<std::pair<typename O::key_type, typename O::mapped_type>> xs(o.size());
  std::copy(o.begin(), o.end(), xs.begin());
  return xs;
}

} // namespace aspartame