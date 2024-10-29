#pragma once

#include "details/base.hpp"
#include <cstddef>
#include <sstream>

#define ASPARTAME_FINALISED 1

namespace aspartame {

constexpr auto show_string = [](auto &&x) {
  std::stringstream out;
  out << x;
  return out.str();
};

// ====== [containers common] ======

// Container<T>, std::string_view, (T -> std::string) -> std::string
template <typename Show>                                                  //
[[nodiscard]] constexpr auto mk_string(const std::string_view &prefix,    //
                                       const std::string_view &separator, //
                                       const std::string_view &suffix,    //
                                       Show &&show) {
  return [&](auto &&o) { return mk_string(o, prefix, separator, suffix, show, tag{}); };
}
// Container<T>, std::string_view -> std::string
[[nodiscard]] constexpr auto mk_string(const std::string_view &prefix,    //
                                       const std::string_view &separator, //
                                       const std::string_view &suffix) {  //
  return [&](auto &&o) { return mk_string(o, prefix, separator, suffix, show_string, tag{}); };
}

[[nodiscard]] constexpr auto mk_string(const std::string_view &separator = "") { //
  return [&](auto &&o) { return mk_string(o, "", separator, "", show_string, tag{}); };
}
template <typename Show> //
[[nodiscard]] constexpr auto mk_string(const std::string_view &separator, Show &&show) {
  return [&](auto &&o) { return mk_string(o, "", separator, "", show, tag{}); };
}

// ITER
// Container<T>, T -> Container<T>
// Map<K, V>, std::pair<K, V> -> Map<K, V>
template <typename T>                                   //
[[nodiscard]] constexpr auto append(const T &element) { //
  return [&](auto &&o) { return append(o, element, tag{}); };
}
// ITER
// Container<T>, Container<T> -> Container<T>
// Map<K, V>, Map<K, V> -> Map<K, V>
template <typename Container> //
[[nodiscard]] constexpr auto concat(const Container &other) {
  return [&](auto &&o) { return concat(o, other, tag{}); };
}
// ITER
// Container<T>, (T -> U) -> Container<U>
// Map<K, V>, ((K, V) -> std::pair<L, W>) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto map(Function &&function) {
  return [&](auto &&o) { return map(o, function, tag{}); };
}
// OUT_MUT ITER
// Container<T>, (T -> std::optional<U>) -> Container<U>
// Map<K, V>, ((K, V) -> std::optional<std::pair<L, W>>) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto collect(Function &&function) {
  return [&](auto &&o) { return collect(o, function, tag{}); };
}
// OUT_MUT ITER
// Container<T>, (T -> bool) -> Container<T>
// Map<K, V>, ((K, V) -> bool) -> Map<K, V>
template <typename Predicate> //
[[nodiscard]] constexpr auto filter(Predicate &&predicate) {
  return [&](auto &&o) { return filter(o, predicate, tag{}); };
}
// ITER
// Container<T>, (T -> Container<U>) -> Container<U>
// Map<K, V>, ((K, V) -> Map<L, W>) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto bind(Function &&function) {
  return [&](auto &&o) { return bind(o, function, tag{}); };
}
// ITER
// Container<Container<T>> -> Container<T>
// Map<K, Map<L, W>> -> Map<L, W>
[[nodiscard]] constexpr auto flatten() { //
  return [&](auto &&o) { return flatten(o, tag{}); };
}
// Container<T> -> (T -> U) -> Container<T>
template <typename Function> //
[[nodiscard]] constexpr auto distinct_by(Function &&function) {
  return [&](auto &&o) { return distinct_by(o, function, tag{}); };
}
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto distinct() { //
  return [&](auto &&o) { return distinct(o, tag{}); };
}
// Container<T> -> (T -> bool) -> size_t
template <typename Predicate> //
[[nodiscard]] constexpr auto count(Predicate &&predicate) {
  return [&](auto &&o) { return count(o, predicate, tag{}); };
}
// Container<T>, (T -> bool) -> bool
// Map<K, V>, ((K, V) -> bool) -> bool
template <typename Predicate> //
[[nodiscard]] constexpr auto exists(Predicate &&predicate) {
  return [&](auto &&o) { return exists(o, predicate, tag{}); };
}
// Container<T>, (T -> bool) -> bool
// Map<K, V>, ((K, V) -> bool) -> bool
template <typename Predicate> //
[[nodiscard]] constexpr auto forall(Predicate &&predicate) {
  return [&](auto &&o) { return forall(o, predicate, tag{}); };
}
// Container<T>, (T -> bool) -> std::optional<T>
// Map<K, V>, ((K, V) -> bool) -> std::optional<std::pair<K,V>>
template <typename Predicate> //
[[nodiscard]] constexpr auto find(Predicate &&predicate) {
  return [&](auto &&o) { return find(o, predicate, tag{}); };
}
// Container<T>, ((T, T) -> U) -> std::optional<U>
// Map<K, V>, ((std::pair<K, V>, std::pair<K, V>) -> U) -> std::optional<U>
template <typename Function> //
[[nodiscard]] constexpr auto reduce(Function &&function) {
  return [&](auto &&o) { return reduce(o, function, tag{}); };
}
// Container<T>, (T -> void) -> Container<T>
// Map<K, V>, ((K, V) -> void) -> Map<K, V>
template <typename Action> //
[[nodiscard]] constexpr auto tap_each(Action &&action) {
  return [&](auto &&o) { return tap_each(o, action, tag{}); };
}
// Container<T>, (T -> void) -> void
// Map<K, V>, ((K, V) -> void) -> void
template <typename Action> //
[[nodiscard]] constexpr auto for_each(Action &&action) {
  return [&](auto &&o) { return for_each(o, action, tag{}); };
}
// OUT_MUT
// Container<T>, (T -> bool) -> std::pair<Container<T>, Container<T>>
// Map<K, V>, ((K, V) -> bool) -> std::pair<Map<K, V>, Map<K, V>>
template <typename Predicate> //
[[nodiscard]] constexpr auto partition(Predicate &&predicate) {
  return [&](auto &&o) { return partition(o, predicate, tag{}); };
}
// Container<T>, (T -> U), (T -> V), ((V, V) -> W) -> Map<U, W>
// Map<K, V>, ((K, V) -> U), ((K, V) -> V), ((V, V) -> W) -> Map<U, W>
template <typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(GroupFunction &&group, MapFunction &&map, ReduceFunction &&reduce) {
  return [&](auto &&o) { return group_map_reduce(o, group, map, reduce, tag{}); };
}
// Container<T>, (T -> U), (T -> V) -> Map<U, Container<T>>
template <typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(GroupFunction &&group, MapFunction &&map) {
  return [&](auto &&o) { return group_map(o, group, map, tag{}); };
}
// Container<T>, (T -> U) -> Map<U, Container<T>>
template <typename Function> //
[[nodiscard]] constexpr auto group_by(Function &&function) {
  return [&](auto &&o) { return group_by(o, function, tag{}); };
}
// Container<T> -> std::vector<T>
// Container<K, V> -> std::vector<std::pair<K, V>>
[[nodiscard]] constexpr auto to_vector() { //
  return [&](auto &&o) { return to_vector(o, tag{}); };
}
// Container<T>, C -> C<T>
// Container<K, V> C -> C<K, V>
template <template<  typename ...> typename C> [[nodiscard]] constexpr auto to() { //
  return [&](auto &&o) { return to<C>(o, tag{}); };
}

// ====== [sequence only] ======

// ITER
// Container<T>, T -> Container<T>
template <typename T> //
[[nodiscard]] constexpr auto prepend(const T &element) {
  return [&](auto &&o) { return prepend(o, element, tag{}); };
}
// Container<T> -> std::optional<T>
[[nodiscard]] constexpr auto head_maybe() { //
  return [&](auto &&o) { return head_maybe(o, tag{}); };
}
// Container<T> -> std::optional<T>
[[nodiscard]] constexpr auto last_maybe() { //
  return [&](auto &&o) { return last_maybe(o, tag{}); };
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto init() { //
  return [&](auto &&o) { return init(o, tag{}); };
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto tail() { //
  return [&](auto &&o) { return tail(o, tag{}); };
}
// Container<T>, size_t -> std::optional<T>
[[nodiscard]] constexpr auto at_maybe(size_t idx) { //
  return [&, idx](auto &&o) { return at_maybe(o, idx, tag{}); };
}
// Container<T>, size_t, size_t -> Container<T>
[[nodiscard]] constexpr auto slice(size_t from_inclusive, size_t to_exclusive) {
  return [&, from_inclusive, to_exclusive](auto &&o) { return slice(o, from_inclusive, to_exclusive, tag{}); };
}
// Container<T>, Container<T> -> std::make_signed_t<size_t>
template <typename Container> //
[[nodiscard]] constexpr auto index_of_slice(const Container &other) {
  return [&](auto &&o) { return index_of_slice(o, other, tag{}); };
}
// Container<T>, Container<T> -> std::optional<size_t>
template <typename Container> //
[[nodiscard]] constexpr auto index_of_slice_maybe(const Container &other) {
  return [&](auto &&o) -> std::optional<size_t> {
    if (auto idx = index_of_slice(o, other, tag{}); idx != -1) return idx;
    else return std::nullopt;
  };
}
// Container<T>, Container<T> -> bool
template <typename Container> //
[[nodiscard]] constexpr auto contains_slice(const Container &other) {
  return [&](auto &&o) { return contains_slice(o, other, tag{}); };
}
// Container<T>, T -> std::make_signed_t<size_t>
template <typename T> //
[[nodiscard]] constexpr auto index_of(const T &t) {
  return [&](auto &&o) { return index_of(o, t, tag{}); };
}
// Container<T>, T -> std::optional<size_t>
template <typename T> //
[[nodiscard]] constexpr auto index_of_maybe(const T &t) {
  return [&](auto &&o) -> std::optional<size_t> {
    if (auto idx = index_of(o, t, tag{}); idx != -1) return idx;
    else return std::nullopt;
  };
}
// Container<T>, T -> bool
template <typename T> //
[[nodiscard]] constexpr auto contains(const T &t) {
  return [&](auto &&o) { return contains(o, t, tag{}); };
}
// Container<T>, (T -> bool) -> std::optional<T>
// Map<K, V>, ((K, V) -> bool) -> std::optional<std::pair<K,V>>
template <typename Predicate> //
[[nodiscard]] constexpr auto find_last(Predicate &&predicate) {
  return [&](auto &&o) { return find_last(o, predicate, tag{}); };
}
// Container<T>, (T -> bool) -> std::make_signed_t<size_t>
template <typename Predicate> //
[[nodiscard]] constexpr auto index_where(Predicate &&predicate) {
  return [&](auto &&o) { return index_where(o, predicate, tag{}); };
}
// Container<T>, (T -> bool) -> std::optional<size_t>
template <typename Predicate> //
[[nodiscard]] constexpr auto index_where_maybe(Predicate &&predicate) {
  return [&](auto &&o) -> std::optional<size_t> {
    if (auto idx = index_where(o, predicate, tag{}); idx != -1) return idx;
    else return std::nullopt;
  };
}
// Container<T> -> Container<std::pair<T, size_t>>
template <typename N = size_t>                            //
[[nodiscard]] constexpr auto zip_with_index(N from = 0) { //
  return [&, from](auto &&o) { return zip_with_index(o, from, tag{}); };
}
// Container<T>, Container<U> -> Container<std::pair<T, U>>
template <typename Container> //
[[nodiscard]] constexpr auto zip(const Container &other) {
  return [&](auto &&o) { return zip(o, other, tag{}); };
}
// ITER
// Container<Container<T>> -> Container<Container<T>>
[[nodiscard]] constexpr auto transpose() { //
  return [&](auto &&o) { return transpose(o, tag{}); };
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto reverse() { //
  return [&](auto &&o) { return reverse(o, tag{}); };
}
// ITER
// Container<T>, URBG -> Container<T>
template <typename URBG>                            //
[[nodiscard]] constexpr auto shuffle(URBG &&urbg) { //
  return [&](auto &&o) { return shuffle(o, std::forward<URBG &&>(urbg), tag{}); };
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto sort() { //
  return [&](auto &&o) { return sort(o, tag{}); };
}
// ITER
// Container<T>, ((T, T) -> bool) -> Container<T>
template <typename Compare> //
[[nodiscard]] constexpr auto sort(Compare &&compare) {
  return [&](auto &&o) { return sort(o, compare, tag{}); };
}
// ITER
// Container<T>, (T -> U) -> Container<T>
template <typename Select> //
[[nodiscard]] constexpr auto sort_by(Select &&select) {
  return [&](auto &&o) { return sort_by(o, select, tag{}); };
}
// OUT_MUT
// Container<T>, size_t -> std::pair<Container<T>, Container<T>>
[[nodiscard]] constexpr auto split_at(size_t idx) { //
  return [&, idx](auto &&o) { return split_at(o, idx, tag{}); };
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto take(size_t n) { //
  return [&, n](auto &&o) { return take(o, n, tag{}); };
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto drop(size_t n) { //
  return [&, n](auto &&o) { return drop(o, n, tag{}); };
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto take_right(size_t n) { //
  return [&, n](auto &&o) { return take_right(o, n, tag{}); };
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto drop_right(size_t n) { //
  return [&, n](auto &&o) { return drop_right(o, n, tag{}); };
}
// OUT_MUT ITER
// Container<T>, (T -> bool) -> Container<T>
template <typename Predicate> //
[[nodiscard]] constexpr auto take_while(Predicate &&predicate) {
  return [&](auto &&o) { return take_while(o, predicate, tag{}); };
}
// OUT_MUT ITER
// Container<T>, (T -> bool) -> Container<T>
template <typename Predicate> //
[[nodiscard]] constexpr auto drop_while(Predicate &&predicate) {
  return [&](auto &&o) { return drop_while(o, predicate, tag{}); };
}
// Container<T>, A, ((A, T)  -> U) -> U
template <typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(Accumulator &&init, Function &&function) {
  return [&](auto &&o) { return fold_left(o, std::forward<Accumulator &&>(init), function, tag{}); };
}
// Container<T>, A, ((T, A)  -> U) -> U
template <typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(Accumulator &&init, Function &&function) {
  return [&](auto &&o) { return fold_right(o, std::forward<Accumulator &&>(init), function, tag{}); };
}
// OUT_MUT
// Container<T>, size_t, size_t -> Container<Container<T>>
[[nodiscard]] constexpr auto sliding(size_t size, size_t step) { //
  return [&, size, step](auto &&o) { return sliding(o, size, step, tag{}); };
}
// OUT_MUT
// Container<T>, size_t  -> Container<Container<T>>
[[nodiscard]] constexpr auto grouped(size_t size) { //
  return sliding(size, size);
}

// ====== [map-like only] ======

// Map<K, V>, K, (K -> V) -> V
template <typename Key, typename Function> //
[[nodiscard]] constexpr auto get_or_emplace(const Key &key, Function &&function) {
  return [&](auto &&o) { return get_or_emplace(o, key, function, tag{}); };
}

// Map<K, V> -> Set<K>
[[nodiscard]] constexpr auto keys() { //
  return [&](auto &&o) { return keys(o, tag{}); };
}
// Map<K, V> -> Set<V>
[[nodiscard]] constexpr auto values() { //
  return [&](auto &&o) { return values(o, tag{}); };
}

// Map<K, V>, (K -> L) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto map_keys(Function &&function) {
  return [&](auto &&o) { return map_keys(o, function, tag{}); };
}

// Map<K, V>, (V -> W) -> Map<K, W>
template <typename Function> //
[[nodiscard]] constexpr auto map_values(Function &&function) {
  return [&](auto &&o) { return map_values(o, function, tag{}); };
}

// Map<K, V>, K -> std::optional<V>
template <typename Key> //
[[nodiscard]] constexpr auto get(const Key &key) {
  return [&](auto &&o) { return get(o, key, tag{}); };
}
// Map<K, V>, K, V -> V
template <typename K, typename V> //
[[nodiscard]] constexpr auto get_or_default(const K &key, const V &default_value) {
  return [&](auto &&o) { return get_or_default(o, key, default_value, tag{}); };
}

// ====== [variant only] ======

// std::variant<T...>, T -> T
template <typename T> //
[[nodiscard]] constexpr auto get() {
  return [&](auto &&o) { return get<T>(o, tag{}); };
}

// std::variant<T...>, ...(T -> U) -> U
template <typename... Function> //
[[nodiscard]] constexpr auto fold_total(Function &&...functions) {
  return [&](auto &&o) { return fold_total(tag{}, o, functions...); };
}

// std::variant<T...>, ...(T -> U) -> std::optional<U>
template <typename... Function> //
[[nodiscard]] constexpr auto fold_partial(Function &&...functions) {
  return [&](auto &&o) { return fold_partial(tag{}, o, functions...); };
}

// std::variant<T...>, ...(T -> void) -> void
template <typename... Function> //
[[nodiscard]] constexpr auto foreach_total(Function &&...functions) {
  return [&](auto &&o) { return foreach_total(tag{}, o, functions...); };
}

// std::variant<T...>, ...(T -> void) -> void
template <typename... Function> //
[[nodiscard]] constexpr auto foreach_partial(Function &&...functions) {
  return [&](auto &&o) { return foreach_partial(tag{}, o, functions...); };
}

// ====== [optional only] ======

// std::optional<T>, (T -> U), (() -> U) -> U
template <typename Function, typename FunctionEmpty> //
[[nodiscard]] constexpr auto fold(Function &&action, FunctionEmpty &&empty_action) {
  return [&](auto &&o) { return fold(o, action, empty_action, tag{}); };
}
// std::optional<T>, (() -> U) -> U
template <typename FunctionEmpty> //
[[nodiscard]] constexpr auto fold(FunctionEmpty &&empty_action) {
  return [&](auto &&o) { return fold(o, empty_action, tag{}); };
}
// std::optional<T>, T -> T
template <typename T> //
[[nodiscard]] constexpr auto get_or_else(const T &default_value) {
  return [&](auto &&o) { return get_or_else(o, default_value, tag{}); };
}
// std::optional<T>, std::optional<T> -> std::optional<T>
template <typename T> //
[[nodiscard]] constexpr auto or_else(const T &that) {
  return [&](auto &&o) { return or_else(o, that, tag{}); };
}

// ====== [string only] ======

// std::string -> std::string
[[nodiscard]] constexpr auto trim_leading() { //
  return [&](auto &&o) { return trim_leading(o, tag{}); };
}
// std::string -> std::string
[[nodiscard]] constexpr auto trim_trailing() { //
  return [&](auto &&o) { return trim_trailing(o, tag{}); };
}
// std::string -> std::string
[[nodiscard]] constexpr auto trim() { //
  return [&](auto &&o) { return trim(o, tag{}); };
}
// std::string -> bool
[[nodiscard]] constexpr auto is_blank() { //
  return [&](auto &&o) { return is_blank(o, tag{}); };
}
// std::string -> std::string
[[nodiscard]] constexpr auto to_upper() { //
  return [&](auto &&o) { return to_upper(o, tag{}); };
}
// std::string -> std::string
[[nodiscard]] constexpr auto to_lower() { //
  return [&](auto &&o) { return to_lower(o, tag{}); };
}
// std::string, std::string, std::string -> std::string
template <typename Needle, typename With> //
[[nodiscard]] constexpr auto replace_all(const Needle &needle, const With &with) {
  return [&](auto &&o) { return replace_all(o, needle, with, tag{}); };
}
// std::string -> std::string
[[nodiscard]] constexpr auto indent(int n) { //
  return [&, n](auto &&o) { return indent(o, n, std::decay_t<decltype(o)>{'\n'}, tag{}); };
}
template <typename NewLine>                                          //
[[nodiscard]] constexpr auto indent(int n, const NewLine &newLine) { //
  return [&, n](auto &&o) { return indent(o, n, newLine, tag{}); };
}
// std::string, std::string-> std::string
template <typename String> //
[[nodiscard]] constexpr auto starts_with(const String &that) {
  return [&](auto &&o) { return starts_with(o, that, tag{}); };
}
// std::string, std::string-> std::string
template <typename String> //
[[nodiscard]] constexpr auto ends_with(const String &that) {
  return [&](auto &&o) { return ends_with(o, that, tag{}); };
}
// std::string, std::string-> std::string
template <typename String> //
[[nodiscard]] constexpr auto contains_ignore_case(const String &that) {
  return [&](auto &&o) { return contains_ignore_case(o, that, tag{}); };
}
// std::string, std::string-> std::string
template <typename String> //
[[nodiscard]] constexpr auto equals_ignore_case(const String &that) {
  return [&](auto &&o) { return equals_ignore_case(o, that, tag{}); };
}
// std::string, char -> std::vector<std::string>
// std::string, std::string -> std::vector<std::string>
template <typename Delimiter> //
[[nodiscard]] constexpr auto split(const Delimiter &delimiter) {
  return [&](auto &&o) { return split(o, delimiter, tag{}); };
}
// std::string -> std::vector<std::string>
[[nodiscard]] constexpr auto lines() { //
  return [&](auto &&o) { return lines(o, tag{}); };
}

// ====== [all types] ======
// T, (T -> U) -> U
template <typename Function> //
[[nodiscard]] constexpr auto and_then(Function &&function) {
  return [&](auto &&o) {
    if constexpr (details::assert_non_void<decltype(details::ap(function, o))>()) {}
    return details::ap(function, o);
  };
}
// T, (T -> void) -> T
template <typename Function> //
[[nodiscard]] constexpr auto tap(Function &&function) {
  return [&](auto &&o) {
    if constexpr (details::assert_void<decltype(details::ap(function, o))>()) {}
    details::ap(function, o);
    return;
  };
}

} // namespace aspartame
