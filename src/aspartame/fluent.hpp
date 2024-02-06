#pragma once

#include "details/base.hpp"
#include <sstream>

#define ASPARTAME_FINALISED 1

namespace aspartame {

#define ASPARTAME_NOOP(T)                                                                                                                  \
  if constexpr (false) { details::assert_fail<T>(); }

#ifndef ASPARTAME_SET
  #define ASPARTAME_SET(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_UNORDERED_SET
  #define ASPARTAME_UNORDERED_SET(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_MAP
  #define ASPARTAME_MAP(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_UNORDERED_MAP
  #define ASPARTAME_UNORDERED_MAP(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_OPTIONAL
  #define ASPARTAME_OPTIONAL(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_VECTOR
  #define ASPARTAME_VECTOR(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_ARRAY
  #define ASPARTAME_ARRAY(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_DEQUE
  #define ASPARTAME_DEQUE(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_LIST
  #define ASPARTAME_LIST(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_STRING
  #define ASPARTAME_STRING(T, op, ...) ASPARTAME_NOOP(T)
#endif
#ifndef ASPARTAME_ITERABLE
  #define ASPARTAME_ITERABLE(T, op, ...) ASPARTAME_NOOP(T)
#endif

#define ASPARTAME_MK_FUNCTOR_0(name)                                                                                                       \
  (const auto &o) {                                                                                                                        \
    using O = std::decay_t<decltype(o)>;                                                                                                   \
    ASPARTAME_OPTIONAL(O, name, o)               /*NOLINT(*-branch-clone)*/                                                                \
    else ASPARTAME_VECTOR(O, name, o)            /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_ARRAY(O, name, o)         /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_DEQUE(O, name, o)         /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_LIST(O, name, o)          /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_SET(O, name, o)           /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_MAP(O, name, o)           /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_UNORDERED_SET(O, name, o) /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_UNORDERED_MAP(O, name, o) /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_STRING(O, name, o)        /*NOLINT(*-branch-clone)*/                                                                \
        else ASPARTAME_ITERABLE(O, name, o)      /*NOLINT(*-branch-clone)*/                                                                \
        else details::unsupported<decltype(o)>();                                                                                          \
  }

#define ASPARTAME_MK_FUNCTOR_N(name, ...)                                                                                                  \
  (const auto &o) {                                                                                                                        \
    using O = std::decay_t<decltype(o)>;                                                                                                   \
    ASPARTAME_OPTIONAL(O, name, o, __VA_ARGS__)               /*NOLINT(*-branch-clone)*/                                                   \
    else ASPARTAME_VECTOR(O, name, o, __VA_ARGS__)            /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_ARRAY(O, name, o, __VA_ARGS__)         /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_DEQUE(O, name, o, __VA_ARGS__)         /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_LIST(O, name, o, __VA_ARGS__)          /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_SET(O, name, o, __VA_ARGS__)           /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_MAP(O, name, o, __VA_ARGS__)           /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_UNORDERED_SET(O, name, o, __VA_ARGS__) /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_UNORDERED_MAP(O, name, o, __VA_ARGS__) /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_STRING(O, name, o, __VA_ARGS__)        /*NOLINT(*-branch-clone)*/                                                   \
        else ASPARTAME_ITERABLE(O, name, o, __VA_ARGS__)                   /*NOLINT(*-branch-clone)*/                                                   \
        else details::unsupported<decltype(o)>();                                                                                          \
  }

constexpr auto show_string = [](auto x) {
  std::stringstream out;
  out << x;
  return out.str();
};

namespace range {

template <typename N> struct inclusive {
  N from_inclusive, to_inclusive, step = 1;

  template <template <typename, typename...> typename C, typename F> //
  [[nodiscard]] constexpr auto tabulate(F &&f) {
    C<std::invoke_result_t<F, N>> xs;
    for (N i = from_inclusive; i <= to_inclusive; i += step)
      if constexpr (details::has_push_back<decltype(xs)>) xs.push_back(f(i));
      else xs.insert(xs.end(), f(i));
    return xs;
  }
  template <template <typename, typename...> typename C, typename F> //
  [[nodiscard]] constexpr auto fill(F &&f) {
    C<std::invoke_result_t<F>> xs;
    for (N i = from_inclusive; i <= to_inclusive; i += step)
      if constexpr (details::has_push_back<decltype(xs)>) xs.push_back(f());
      else xs.insert(xs.end(), f());
    return xs;
  }
};
template <typename T> inclusive(T, T) -> inclusive<T>;

template <typename N> struct exclusive {
  N from_inclusive, to_exclusive, step = 1;

  template <template <typename, typename...> typename C, typename F> //
  [[nodiscard]] constexpr auto tabulate(F &&f) {
    C<std::invoke_result_t<F, N>> xs;
    for (N i = from_inclusive; i < to_exclusive; i += step)
      if constexpr (details::has_push_back<decltype(xs)>) xs.push_back(f(i));
      else xs.insert(xs.end(), f(i));

    return xs;
  }
  template <template <typename, typename...> typename C, typename F> //
  [[nodiscard]] constexpr auto fill(F &&f) {
    C<std::invoke_result_t<F>> xs;
    for (N i = from_inclusive; i < to_exclusive; i += step)
      if constexpr (details::has_push_back<decltype(xs)>) xs.push_back(f());
      else xs.insert(xs.end(), f());
    return xs;
  }
};
template <typename T> exclusive(T, T) -> exclusive<T>;

} // namespace range

// ====== [containers common] ======

// Container<T>, std::string_view, (T -> std::string) -> std::string
template <typename Show> //
[[nodiscard]] constexpr auto mk_string(const std::string_view &separator, const std::string_view &prefix, const std::string_view &suffix,
                                       Show &&show) {
  return [&] ASPARTAME_MK_FUNCTOR_N(mk_string, separator, prefix, suffix, show);
}
template <typename Show> //
[[nodiscard]] constexpr auto mk_string(const std::string_view &separator, Show &&show) {
  return [&] ASPARTAME_MK_FUNCTOR_N(mk_string, separator, "", "", show);
}
// Container<T>, std::string_view -> std::string
[[nodiscard]] constexpr auto mk_string(const std::string_view &separator = "", const std::string_view &prefix = "",
                                       const std::string_view &suffix = "") { //
  return [&] ASPARTAME_MK_FUNCTOR_N(mk_string, separator, prefix, suffix, show_string);
}
// ITER
// Container<T>, T -> Container<T>
// Map<K, V>, std::pair<K, V> -> Map<K, V>
template <typename T>                                   //
[[nodiscard]] constexpr auto append(const T &element) { //
  return [&] ASPARTAME_MK_FUNCTOR_N(append, element);
}
// ITER
// Container<T>, Container<T> -> Container<T>
// Map<K, V>, Map<K, V> -> Map<K, V>
template <typename Container> //
[[nodiscard]] constexpr auto concat(const Container &other) {
  return [&] ASPARTAME_MK_FUNCTOR_N(concat, other);
}
// ITER
// Container<T>, (T -> U) -> Container<U>
// Map<K, V>, ((K, V) -> std::pair<L, W>) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto map(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(map, function);
}
// OUT_MUT ITER
// Container<T>, (T -> std::optional<U>) -> Container<U>
// Map<K, V>, ((K, V) -> std::optional<std::pair<L, W>>) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto collect(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(collect, function);
}
// OUT_MUT ITER
// Container<T>, (T -> bool) -> Container<T>
// Map<K, V>, ((K, V) -> bool) -> Map<K, V>
template <typename Predicate> //
[[nodiscard]] constexpr auto filter(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(filter, predicate);
}
// ITER
// Container<T>, (T -> Container<U>) -> Container<U>
// Map<K, V>, ((K, V) -> Map<L, W>) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto bind(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(bind, function);
}
// ITER
// Container<Container<T>> -> Container<T>
// Map<K, Map<L, W>> -> Map<L, W>
[[nodiscard]] constexpr auto flatten() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(flatten);
}
// Container<T> -> (T -> U) -> Container<T>
template <typename Function> //
[[nodiscard]] constexpr auto distinct_by(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(distinct_by, function);
}
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto distinct() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(distinct);
}
// Container<T> -> (T -> bool) -> size_t
template <typename Predicate> //
[[nodiscard]] constexpr auto count(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(count, predicate);
}
// Container<T>, (T -> bool) -> bool
// Map<K, V>, ((K, V) -> bool) -> bool
template <typename Predicate> //
[[nodiscard]] constexpr auto exists(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(exists, predicate);
}
// Container<T>, (T -> bool) -> bool
// Map<K, V>, ((K, V) -> bool) -> bool
template <typename Predicate> //
[[nodiscard]] constexpr auto forall(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(forall, predicate);
}
// Container<T>, ((T, T) -> U) -> std::optional<U>
// Map<K, V>, ((std::pair<K, V>, std::pair<K, V>) -> U) -> std::optional<U>
template <typename Function> //
[[nodiscard]] constexpr auto reduce(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(reduce, function);
}
// Container<T>, (T -> void) -> Container<T>
// Map<K, V>, ((K, V) -> void) -> Map<K, V>
template <typename Action> //
[[nodiscard]] constexpr auto tap_each(Action &&action) {
  return [&] ASPARTAME_MK_FUNCTOR_N(tap_each, action);
}
// Container<T>, (T -> void) -> void
// Map<K, V>, ((K, V) -> void) -> void
template <typename Action> //
[[nodiscard]] constexpr auto for_each(Action &&action) {
  return [&] ASPARTAME_MK_FUNCTOR_N(for_each, action);
}
// OUT_MUT
// Container<T>, (T -> bool) -> std::pair<Container<T>, Container<T>>
// Map<K, V>, ((K, V) -> bool) -> std::pair<Map<K, V>, Map<K, V>>
template <typename Predicate> //
[[nodiscard]] constexpr auto partition(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(partition, predicate);
}
// Container<T>, (T -> U), (T -> V), ((V, V) -> W) -> Map<U, W>
// Map<K, V>, ((K, V) -> U), ((K, V) -> V), ((V, V) -> W) -> Map<U, W>
template <typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(GroupFunction &&group, MapFunction &&map, ReduceFunction &&reduce) {
  return [&] ASPARTAME_MK_FUNCTOR_N(group_map_reduce, group, map, reduce);
}
// Container<T>, (T -> U), (T -> V) -> Map<U, Container<T>>
template <typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(GroupFunction &&group, MapFunction &&map) {
  return [&] ASPARTAME_MK_FUNCTOR_N(group_map, group, map);
}
// Container<T>, (T -> U) -> Map<U, Container<T>>
template <typename Function> //
[[nodiscard]] constexpr auto group_by(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(group_by, function);
}
// Container<T> -> std::vector<T>
// Container<K, V> -> std::vector<std::pair<K, V>>
[[nodiscard]] constexpr auto to_vector() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(to_vector);
}

// ====== [sequence only] ======

// ITER
// Container<T>, T -> Container<T>
template <typename T> //
[[nodiscard]] constexpr auto prepend(const T &element) {
  return [&] ASPARTAME_MK_FUNCTOR_N(prepend, element);
}
// Container<T> -> std::optional<T>
[[nodiscard]] constexpr auto head_maybe() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(head_maybe);
}
// Container<T> -> std::optional<T>
[[nodiscard]] constexpr auto last_maybe() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(last_maybe);
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto init() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(init);
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto tail() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(tail);
}
// Container<T>, size_t -> std::optional<T>
[[nodiscard]] constexpr auto at_maybe(size_t idx) { //
  return [&, idx ] ASPARTAME_MK_FUNCTOR_N(at_maybe, idx);
}
// Container<T>, size_t, size_t -> Container<T>
[[nodiscard]] constexpr auto slice(size_t from_inclusive, size_t to_exclusive) {
  return [&, from_inclusive, to_exclusive ] ASPARTAME_MK_FUNCTOR_N(slice, from_inclusive, to_exclusive);
}
// Container<T>, Container<T> -> int
template <typename Container> //
[[nodiscard]] constexpr auto index_of_slice(const Container &other) {
  return [&] ASPARTAME_MK_FUNCTOR_N(index_of_slice, other);
}
// Container<T>, Container<T> -> bool
template <typename Container> //
[[nodiscard]] constexpr auto contains_slice(const Container &other) {
  return [&] ASPARTAME_MK_FUNCTOR_N(contains_slice, other);
}
// Container<T>, T -> long
template <typename T> //
[[nodiscard]] constexpr auto index_of(const T &t) {
  return [&] ASPARTAME_MK_FUNCTOR_N(index_of, t);
}
// Container<T>, T -> bool
template <typename T> //
[[nodiscard]] constexpr auto contains(const T &t) {
  return [&] ASPARTAME_MK_FUNCTOR_N(contains, t);
}
// Container<T>, (T -> bool) -> long
template <typename Predicate> //
[[nodiscard]] constexpr auto index_where(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(index_where, predicate);
}
// Container<T> -> Container<std::pair<T, size_t>>
[[nodiscard]] constexpr auto zip_with_index() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(zip_with_index);
}
// Container<T>, Container<U> -> Container<std::pair<T, U>>
template <typename Container> //
[[nodiscard]] constexpr auto zip(const Container &other) {
  return [&] ASPARTAME_MK_FUNCTOR_N(zip, other);
}
// ITER
// Container<Container<T>> -> Container<Container<T>>
[[nodiscard]] constexpr auto transpose() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(transpose);
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto reverse() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(reverse);
}
// ITER
// Container<T>, URBG -> Container<T>
template <typename URBG>                            //
[[nodiscard]] constexpr auto shuffle(URBG &&urbg) { //
  return [&] ASPARTAME_MK_FUNCTOR_N(shuffle, std::forward<URBG &&>(urbg));
}
// ITER
// Container<T> -> Container<T>
[[nodiscard]] constexpr auto sort() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(sort);
}
// ITER
// Container<T>, ((T, T) -> bool) -> Container<T>
template <typename Compare> //
[[nodiscard]] constexpr auto sort(Compare &&compare) {
  return [&] ASPARTAME_MK_FUNCTOR_N(sort, compare);
}
// ITER
// Container<T>, (T -> U) -> Container<T>
template <typename Select> //
[[nodiscard]] constexpr auto sort_by(Select &&select) {
  return [&] ASPARTAME_MK_FUNCTOR_N(sort_by, select);
}
// OUT_MUT
// Container<T>, size_t -> std::pair<Container<T>, Container<T>>
[[nodiscard]] constexpr auto split_at(size_t idx) { //
  return [&, idx ] ASPARTAME_MK_FUNCTOR_N(split_at, idx);
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto take(size_t n) { //
  return [&, n ] ASPARTAME_MK_FUNCTOR_N(take, n);
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto drop(size_t n) { //
  return [&, n ] ASPARTAME_MK_FUNCTOR_N(drop, n);
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto take_right(size_t n) { //
  return [&, n ] ASPARTAME_MK_FUNCTOR_N(take_right, n);
}
// OUT_MUT ITER
// Container<T>, size_t -> Container<T>
[[nodiscard]] constexpr auto drop_right(size_t n) { //
  return [&, n ] ASPARTAME_MK_FUNCTOR_N(drop_right, n);
}
// OUT_MUT ITER
// Container<T>, (T -> bool) -> Container<T>
template <typename Predicate> //
[[nodiscard]] constexpr auto take_while(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(take_while, predicate);
}
// OUT_MUT ITER
// Container<T>, (T -> bool) -> Container<T>
template <typename Predicate> //
[[nodiscard]] constexpr auto drop_while(Predicate &&predicate) {
  return [&] ASPARTAME_MK_FUNCTOR_N(drop_while, predicate);
}
// Container<T>, A, ((A, T)  -> U) -> U
template <typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(Accumulator &&init, Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(fold_left, std::forward<Accumulator &&>(init), function);
}
// Container<T>, A, ((T, A)  -> U) -> U
template <typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(Accumulator &&init, Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(fold_right, std::forward<Accumulator &&>(init), function);
}
// OUT_MUT
// Container<T>, size_t, size_t -> Container<Container<T>>
[[nodiscard]] constexpr auto sliding(size_t size, size_t step) { //
  return [&, size, step ] ASPARTAME_MK_FUNCTOR_N(sliding, size, step);
}
// OUT_MUT
// Container<T>, size_t  -> Container<Container<T>>
[[nodiscard]] constexpr auto grouped(size_t size) { //
  return sliding(size, size);
}

// ====== [map-like only] ======

// Map<K, V> -> Set<K>
[[nodiscard]] constexpr auto keys() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(keys);
}
// Map<K, V> -> Set<V>
[[nodiscard]] constexpr auto values() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(values);
}

// Map<K, V>, (K -> L) -> Map<L, W>
template <typename Function> //
[[nodiscard]] constexpr auto map_keys(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(map_keys, function);
}

// Map<K, V>, (V -> W) -> Map<K, W>
template <typename Function> //
[[nodiscard]] constexpr auto map_values(Function &&function) {
  return [&] ASPARTAME_MK_FUNCTOR_N(map_values, function);
}

// Map<K, V>, K -> std::optional<V>
template <typename Key> //
[[nodiscard]] constexpr auto get(const Key &key) {
  return [&] ASPARTAME_MK_FUNCTOR_N(get, key);
}
// Map<K, V>, K, V -> V
template <typename K, typename V> //
[[nodiscard]] constexpr auto get_or_default(const K &key, const V &default_value) {
  return [&] ASPARTAME_MK_FUNCTOR_N(get_or_default, key, default_value);
}

// ====== [optional only] ======

// std::optional<T>, (T -> U), (() -> U) -> U
template <typename Function, typename FunctionEmpty> //
[[nodiscard]] constexpr auto fold(Function &&action, FunctionEmpty &&empty_action) {
  return [&] ASPARTAME_MK_FUNCTOR_N(fold, action, empty_action);
}
// std::optional<T>, T -> T
template <typename T> //
[[nodiscard]] constexpr auto get_or_else(const T &default_value) {
  return [&] ASPARTAME_MK_FUNCTOR_N(get_or_else, default_value);
}
// std::optional<T>, std::optional<T> -> std::optional<T>
template <typename T> //
[[nodiscard]] constexpr auto or_else(const T &that) {
  return [&] ASPARTAME_MK_FUNCTOR_N(or_else, that);
}

// ====== [string only] ======

// std::string -> std::string
[[nodiscard]] constexpr auto trim_leading() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(trim_leading);
}
// std::string -> std::string
[[nodiscard]] constexpr auto trim_trailing() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(trim_trailing);
}
// std::string -> std::string
[[nodiscard]] constexpr auto trim() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(trim);
}
// std::string -> bool
[[nodiscard]] constexpr auto is_blank() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(is_blank);
}
// std::string -> std::string
[[nodiscard]] constexpr auto indent(int n) { //
  return [&, n ] ASPARTAME_MK_FUNCTOR_N(indent, n);
}
// std::string -> std::string
[[nodiscard]] constexpr auto to_upper() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(to_upper);
}
// std::string -> std::string
[[nodiscard]] constexpr auto to_lower() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(to_lower);
}
// std::string, std::string, std::string -> std::string
template <typename Needle, typename With> //
[[nodiscard]] constexpr auto replace_all(const Needle &needle, const With &with) {
  return [&] ASPARTAME_MK_FUNCTOR_N(replace_all, needle, with);
}
// std::string, std::string-> std::string
template <typename String> //
[[nodiscard]] constexpr auto contains_ignore_case(const String &that) {
  return [&] ASPARTAME_MK_FUNCTOR_N(contains_ignore_case, that);
}
// std::string, std::string-> std::string
template <typename String> //
[[nodiscard]] constexpr auto equals_ignore_case(const String &that) {
  return [&] ASPARTAME_MK_FUNCTOR_N(equals_ignore_case, that);
}
// std::string, char -> std::vector<std::string>
// std::string, std::string -> std::vector<std::string>
template <typename Delimiter> //
[[nodiscard]] constexpr auto split(const Delimiter &delimeter) {
  return [&] ASPARTAME_MK_FUNCTOR_N(split, delimeter);
}
// std::string -> std::vector<std::string>
[[nodiscard]] constexpr auto lines() { //
  return [&] ASPARTAME_MK_FUNCTOR_0(lines);
}

// ====== [all types] ======
// T, (T -> U) -> U
template <typename Function> //
[[nodiscard]] constexpr auto and_then(Function &&function) {
  return [&](auto &o) {
    if constexpr (details::assert_non_void<decltype(details::ap(function, o))>()) {}
    return details::ap(function, o);
  };
}
// T, (T -> void) -> T
template <typename Function> //
[[nodiscard]] constexpr auto tap(Function &&function) {
  return [&](auto &o) {
    if constexpr (details::assert_void<decltype(details::ap(function, o))>()) {}
    details::ap(function, o);
    return;
  };
}

} // namespace aspartame
