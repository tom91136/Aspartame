#pragma once

#include "details/base.hpp"
#include "details/container1_impl.hpp"
#include "details/iterators/append_prepend.hpp"
#include "details/iterators/bind.hpp"
#include "details/iterators/collect.hpp"
#include "details/iterators/concat.hpp"
#include "details/iterators/distinct.hpp"
#include "details/iterators/drop_take.hpp"
#include "details/iterators/filter.hpp"
#include "details/iterators/map.hpp"
#include "details/iterators/slice.hpp"
#include "details/iterators/sliding.hpp"
#include "details/iterators/tap_each.hpp"

#include <limits>

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp; \
consider adding a new line between this include and others to prevent reording by formatters."
#endif

namespace aspartame {

template <typename T> using owning = std::unique_ptr<T>;
class non_owning {};

template <typename Iterator, typename Container = non_owning> class view {
public:
  Container underlying;
  Iterator begin_, end_;
  using iterator_category = typename Iterator::iterator_category;
  using value_type = typename Iterator::value_type;
  using const_iterator = Iterator;
  constexpr view() = default;

  explicit view(Iterator begin, Iterator end = {}) : underlying(), begin_(std::move(begin)), end_(std::move(end)) {
    static_assert(std::is_default_constructible_v<view>);
    static_assert(std::is_copy_constructible_v<view>);
    static_assert(std::is_copy_assignable_v<view>);
    static_assert(std::is_move_constructible_v<view>);
    static_assert(std::is_move_assignable_v<view>);
    static_assert(std::is_destructible_v<view>);
  }

  template <typename Ignore>
  explicit view(view<Ignore, Container> &prev, Iterator begin, Iterator end = {})
      : underlying(std::move(prev.underlying)), begin_(std::move(begin)), end_(std::move(end)) {}

  template <typename C>
  explicit view(C c) : underlying(std::move(c)), begin_(std::move(underlying->begin())), end_(std::move(underlying->end())) {}

  [[nodiscard]] constexpr Iterator begin() const { return begin_; }
  [[nodiscard]] constexpr Iterator end() const { return end_; }
  [[nodiscard]] constexpr bool empty() const { return begin_ == end_; }
  [[nodiscard]] constexpr bool operator==(const view &rhs) const { return begin_ == rhs.begin_ && end_ == rhs.end_; }
  [[nodiscard]] constexpr bool operator!=(const view &rhs) const { return !rhs.operator==(*this); }

  template <typename Op> auto operator|(const Op &r) { return r(*this); }
};

template <typename Iterator> //
view(Iterator, Iterator) -> view<Iterator, non_owning>;

template <typename C> //
view(C) -> view<typename C::const_iterator, C>;

template <typename Iterator, typename Storage, typename Ignore> //
view(view<Ignore, Storage>, Iterator, Iterator) -> view<Ignore, Storage>;

namespace details {
template <typename> constexpr bool is_view_impl = false;
template <typename T, typename U> constexpr bool is_view_impl<view<T, U>> = true;
}; // namespace details
template <typename T> constexpr bool is_view = details::is_view_impl<std::decay_t<T>>;

template <typename Iterable, //
          typename Op,       //
          typename = std::enable_if_t<is_iterable<Iterable> && !is_view<Iterable>>>
auto operator|(Iterable &&l, const Op &r) {
  if constexpr (!std::is_rvalue_reference_v<Iterable &&>) return r(view(l.begin(), l.end()));
  else return r(view<typename Iterable::const_iterator, owning<Iterable>>(std::make_unique<Iterable>(std::forward<Iterable &&>(l))));
}

// == container

template <typename C, typename Storage, typename Function> //
[[nodiscard]] auto mk_string(view<C, Storage> &in, const std::string_view &sep, const std::string_view &prefix,
                             const std::string_view &suffix, Function &&f) {
  return details::container::mk_string<view<C, Storage>, Function>(in, sep, prefix, suffix, f);
}

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto append(view<C, Storage> &in, const T &t) {
  return view(in, details::append_prepend_iterator<details::append_prepend_iterator_mode::append, C>( //
                      in.begin(), in.end(), t));
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto concat(view<C, Storage> &in, Container &&other) {
  return view(in, details::concat_iterator(in.begin(), in.end(), other.begin(), other.end()));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto map(view<C, Storage> &in, Function &&function) {
  auto applied = [&](auto &&x) { return details::ap(function, x); };
  return view(in, details::map_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto collect(view<C, Storage> &in, Function &&function) {
  auto applied = [&](auto &&x) { return details::ap(function, x); };
  return view(in, details::collect_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto filter(view<C, Storage> &in, Predicate &&predicate) {
  auto applied = [&](auto &&x) { return details::ap(predicate, x); };
  return view(in, details::filter_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto bind(view<C, Storage> &in, Function &&function) {
  auto applied = [&](auto &&x) { return details::ap(function, x); };
  return view(in, details::bind_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto flatten(view<C, Storage> &in) {
  auto identity = [](auto &&x) { return x; };
  return view(in, details::bind_iterator(in.begin(), in.end(), identity));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto distinct_by(view<C, Storage> &in, Function &&function) {
  auto applied = [&](auto &&x) { return details::ap(function, x); };
  return view(in, details::distinct_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto distinct(view<C, Storage> &in) {
  return distinct_by(in, [](auto x) { return x; });
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto count(const view<C, Storage> &in, Predicate &&predicate) {
  return details::container::count<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto exists(const view<C, Storage> &in, Predicate &&predicate) {
  return details::container::exists<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto forall(const view<C, Storage> &in, Predicate &&predicate) {
  return details::container::forall<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto reduce(const view<C, Storage> &in, Function &&function) {
  return details::container::reduce<view<C, Storage>, Function>(in, function);
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto tap_each(view<C, Storage> &in, Function &&function) {
  auto applied = [&](auto &&x) { details::ap(function, x); };
  return view(in, details::tap_each_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto for_each(const view<C, Storage> &in, Function &&function) {
  details::container::for_each<view<C, Storage>, Function>(in, function);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto partition(view<C, Storage> &in, Predicate &&predicate) {
  return std::pair{view(in, details::filter_iterator(in.begin(), in.end(), [&](auto &&x) { return details::ap(predicate, x); })),
                   view(in, details::filter_iterator(in.begin(), in.end(), [&](auto &&x) { return !details::ap(predicate, x); }))};
}

template <typename C, typename Storage, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(const view<C, Storage> &in, GroupFunction &&group, MapFunction &&map,
                                              ReduceFunction &&reduce) {
  return details::container::group_map_reduce<view<C, Storage>, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename C, typename Storage, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(const view<C, Storage> &in, GroupFunction &&group, MapFunction &&map) {
  return details::container::group_map<view<C, Storage>, GroupFunction, MapFunction, std::vector>(in, group, map);
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto group_by(const view<C, Storage> &in, Function &&function) {
  return details::container::group_by<view<C, Storage>, Function, std::vector>(in, function);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto to_vector(const view<C, Storage> &in) {
  return std::vector<typename view<C, Storage>::value_type>{in.begin(), in.end()};
}

// == sequence

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto prepend(view<C, Storage> &in, const T &t) {
  return view(in, details::append_prepend_iterator<details::append_prepend_iterator_mode::prepend, C>( //
                      in.begin(), in.end(), t));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto head_maybe(const view<C, Storage> &in) {
  return details::sequence::head_maybe<view<C, Storage>>(in);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto last_maybe(const view<C, Storage> &in) {
  return details::sequence::last_maybe<view<C, Storage>>(in);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto init(const view<C, Storage> &in) {
  return details::sequence::init<view<C, Storage>, view<C, Storage>>(in);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto tail(const view<C, Storage> &in) {
  return details::sequence::tail<view<C, Storage>, view<C, Storage>>(in);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto at_maybe(const view<C, Storage> &in, size_t idx) {
  return details::sequence::at_maybe<view<C, Storage>>(in, idx);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto slice(view<C, Storage> &in, size_t from_inclusive, size_t to_exclusive) {
  return view(in, details::slice_iterator(in.begin(), in.end(), from_inclusive, to_exclusive));
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto index_of_slice(const view<C, Storage> &in, const Container &other) {
  return details::sequence::index_of_slice<view<C, Storage>, Container>(in, other);
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto contains_slice(const view<C, Storage> &in, const Container &other) {
  return details::sequence::index_of_slice<view<C, Storage>, Container>(in, other) != -1;
}

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto index_of(const view<C, Storage> &in, const T &t) {
  return details::sequence::index_of<view<C, Storage>>(in, t);
}

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto contains(const view<C, Storage> &in, const T &t) {
  return details::sequence::index_of<view<C, Storage>>(in, t) != -1;
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto index_where(const view<C, Storage> &in, Predicate &&predicate) {
  return details::sequence::index_where<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto zip_with_index(const view<C, Storage> &in) {
  return details::sequence::zip_with_index<view<C, Storage>, std::vector>(in);
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto zip(const view<C, Storage> &in, const Container &other) {
  return details::sequence::zip<view<C, Storage>, Container, std::vector>(in, other);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto transpose(const view<C, Storage> &in) {
  return details::sequence::transpose<view<C, Storage>, std::vector>(in);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto reverse(const view<C, Storage> &in) {
  return details::sequence::reverse<view<C, Storage>, view<C, Storage>>(in);
}

template <typename C, typename Storage, typename URBG> //
[[nodiscard]] constexpr auto shuffle(const view<C, Storage> &in, URBG &&urbg) {
  return details::sequence::shuffle<view<C, Storage>, URBG, view<C, Storage>>(in, std::forward<URBG &&>(urbg));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto sort(const view<C, Storage> &in) {
  return details::sequence::sort<view<C, Storage>, view<C, Storage>>(in);
}

template <typename C, typename Storage, typename Compare> //
[[nodiscard]] constexpr auto sort(const view<C, Storage> &in, Compare &&compare) {
  return details::sequence::sort<view<C, Storage>, Compare, view<C, Storage>>(in, compare);
}

template <typename C, typename Storage, typename Select> //
[[nodiscard]] constexpr auto sort_by(const view<C, Storage> &in, Select &&select) {
  return details::sequence::sort_by<view<C, Storage>, Select, view<C, Storage>>(in, select);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto split_at(const view<C, Storage> &in, size_t idx) {
  return details::sequence::split_at<view<C, Storage>, view<C, Storage>>(in, idx);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto take_right(const view<C, Storage> &in, size_t n) {
  return details::sequence::take_right<view<C, Storage>, view<C, Storage>>(in, n);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto drop_right(const view<C, Storage> &in, size_t n) {
  return details::sequence::drop_right<view<C, Storage>, view<C, Storage>>(in, n);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto take(view<C, Storage> &in, size_t n) {
  return view(in, details::slice_iterator(in.begin(), in.end(), 0, n));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto drop(view<C, Storage> &in, size_t n) {
  return view(in, details::slice_iterator(in.begin(), in.end(), n, std::numeric_limits<size_t>::max()));
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto take_while(view<C, Storage> &in, Predicate &&predicate) {
  auto applied = [&](auto &&x) { return details::ap(predicate, x); };
  return view(in, details::drop_take_iterator<details::drop_take_iterator_mode::take_while_true, C, decltype(applied)>( //
                      in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto drop_while(view<C, Storage> &in, Predicate &&predicate) {
  auto applied = [&](auto &&x) { return details::ap(predicate, x); };
  return view(in, details::drop_take_iterator<details::drop_take_iterator_mode::drop_while_true, C, decltype(applied)>( //
                      in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(const view<C, Storage> &in, Accumulator &&init, Function &&function) {
  return details::sequence::fold_left<view<C, Storage>, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C, typename Storage, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(const view<C, Storage> &in, Accumulator &&init, Function &&function) {
  details::unsupported<C>(in, init, function);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto sliding(view<C, Storage> &in, size_t size, size_t step) {
  return view(in, details::sliding_iterator<C, view>(in.begin(), in.end(), size, step));
}

} // namespace aspartame
