#pragma once

#include "fluent.hpp"

#include "details/base.hpp"
#include "details/container1_impl.hpp"
#include "details/iterators/append_prepend.hpp"
#include "details/iterators/flat_map.hpp"
#include "details/iterators/collect.hpp"
#include "details/iterators/concat.hpp"
#include "details/iterators/distinct.hpp"
#include "details/iterators/drop_take.hpp"
#include "details/iterators/filter.hpp"
#include "details/iterators/iterate.hpp"
#include "details/iterators/map.hpp"
#include "details/iterators/slice.hpp"
#include "details/iterators/sliding.hpp"
#include "details/iterators/tap_each.hpp"
#include "details/iterators/zip.hpp"
#include "details/sequence1_impl.hpp"

#include <limits>

namespace aspartame {

template <typename T> using owning = std::unique_ptr<T>;
template <typename T> using sharing = std::shared_ptr<T>;
class non_owning {};

namespace details {
template <typename> constexpr bool is_owning = false;
template <typename T> constexpr bool is_owning<owning<T>> = true;
template <typename> constexpr bool is_sharing = false;
template <typename T> constexpr bool is_sharing<sharing<T>> = true;
template <typename T> constexpr bool is_non_owning = std::is_same_v<T, non_owning>;
} // namespace details

template <typename Iterator, typename Storage = non_owning> class view {
public:
  Storage storage;
  Iterator begin_, end_;
  using iterator_category = std::forward_iterator_tag;
  using value_type = details::value_type_of_t<Iterator>;
  using const_iterator = Iterator;
  constexpr view() = default;

  explicit view(Iterator begin, Iterator end = {}) : storage(), begin_(std::move(begin)), end_(std::move(end)) {
    static_assert(std::is_default_constructible_v<view>);
    static_assert(std::is_copy_constructible_v<view>);
    static_assert(std::is_copy_assignable_v<view>);
    static_assert(std::is_move_constructible_v<view>);
    static_assert(std::is_move_assignable_v<view>);
    static_assert(std::is_destructible_v<view>);
  }

  explicit view(Storage storage, Iterator begin, Iterator end = {})
      : storage(std::move(storage)), begin_(std::move(begin)), end_(std::move(end)) {}

  template <typename C> explicit view(C c) : storage(std::move(c)), begin_(std::move(storage->begin())), end_(std::move(storage->end())) {}

  [[nodiscard]] constexpr Iterator begin() const { return begin_; }
  [[nodiscard]] constexpr Iterator end() const { return end_; }
  [[nodiscard]] constexpr bool empty() const { return begin_ == end_; }
  [[nodiscard]] constexpr bool operator==(const view &rhs) const { return begin_ == rhs.begin_ && end_ == rhs.end_; }
  [[nodiscard]] constexpr bool operator!=(const view &rhs) const { return !rhs.operator==(*this); }

  template <typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
    requires std::invocable<Op, view &, tag>
#endif
  auto operator|(const Op &r) {
    return r(*this, tag{});
  }
};

template <typename Iterator> //
view(Iterator, Iterator) -> view<Iterator, non_owning>;

template <typename C> //
view(C) -> view<typename C::const_iterator, C>;

template <typename Iterator, typename Storage> //
view(Storage &, Iterator, Iterator) -> view<Iterator, Storage>;

namespace details {
template <typename Storage, typename F> auto use_shared(Storage &storage, F f) {
  using S = std::decay_t<Storage>;
  if constexpr (details::is_non_owning<S> || details::is_sharing<S>) {
    return f(storage);
  } else if constexpr (details::is_owning<S>) {
    sharing<typename S::element_type> s = std::move(storage);
    return f(s);
  } else static_assert(!sizeof(Storage), "unhandled ownership of storage, this is a bug");
}

template <typename C, typename Storage, typename Iter> auto make_unique_view(view<C, Storage> &in, Iter &&it) {
  using S = std::decay_t<Storage>;
  if constexpr (details::is_non_owning<S> || details::is_sharing<S>) {
    return view(in.storage, std::forward<Iter &&>(it));
  } else if constexpr (details::is_owning<S>) {
    return view(std::move(in.storage), std::forward<Iter &&>(it));
  } else static_assert(!sizeof(Storage), "unhandled ownership of storage, this is a bug");
}
} // namespace details

template <typename T> auto repeat(T &&t) { //
  auto next = [](auto &&x) { return x; };
  return view<details::iterate_iterator<std::decay_t<T>, decltype(next)>, non_owning>({t, next});
}

template <typename T, typename F> auto iterate(T &&init, F &&next) { //
  return view<details::iterate_iterator<std::decay_t<T>, F>, non_owning>({init, next});
}

template <typename N> auto iota(const N &origin) { //
  auto next = [](auto &x) { return x + 1; };
  return view<details::iterate_iterator<std::decay_t<N>, decltype(next)>, non_owning>({origin, next});
}

template <typename N> auto iota(const N &origin, const N &count) { //
  return iota<N>(origin) | take(count);
}

template <typename N> auto exclusive(const N &from_inclusive, const N &to_exclusive, const N &step = 1) { //
  if (step == N{}) details::raise<std::range_error>("step cannot be empty (0)");
  auto next = [=](auto &&x) { return x + step; };
  return view<details::iterate_iterator<N, decltype(next)>, non_owning>({from_inclusive, next}) |
         take_while(std::move([=](auto &&x) { return x < to_exclusive; }));
}

template <typename N> auto inclusive(const N &from_inclusive, const N &to_inclusive, const N &step = 1) { //
  return exclusive(from_inclusive, to_inclusive + 1, step);
}

namespace details {
template <typename> constexpr bool is_view_impl = false;
template <typename T, typename U> constexpr bool is_view_impl<view<T, U>> = true;
} // namespace details
template <typename T> constexpr bool is_view = details::is_view_impl<std::decay_t<T>>;

template <typename Iterable, //
          typename Op,       //
          typename = std::enable_if_t<is_iterable<Iterable> && !is_view<Iterable>>>
auto operator|(Iterable &&l, const Op &r) {
  if constexpr (!std::is_rvalue_reference_v<Iterable &&>) return r(view(l.begin(), l.end()), tag{});
  else if constexpr (details::has_const_iterator<Iterable>)
    return r(view<typename Iterable::const_iterator, owning<Iterable>>(std::make_unique<Iterable>(std::forward<Iterable &&>(l))), tag{});
  else return r(view<decltype(l.begin()), owning<Iterable>>(std::make_unique<Iterable>(std::forward<Iterable &&>(l))), tag{});
}

// == container

template <typename C, typename Storage, typename Function> //
[[nodiscard]] auto mk_string(view<C, Storage> &in, const std::string_view &prefix, const std::string_view &sep,
                             const std::string_view &suffix, Function &&f, tag = {}) {
  return details::container1::mk_string<view<C, Storage>, Function>(in, prefix, sep, suffix, f);
}

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto append(view<C, Storage> &in, const T &t, tag = {}) {
  return details::make_unique_view( //
      in, details::append_prepend_iterator<details::append_prepend_iterator_mode::append, C>(in.begin(), in.end(), t));
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto concat(view<C, Storage> &in, Container &&other, tag = {}) {
  return details::make_unique_view( //
      in, details::concat_iterator(in.begin(), in.end(), other.begin(), other.end()));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto map(view<C, Storage> &in, Function &&function, tag = {}) {
  auto applied = [function](auto &&x) { return details::ap(function, x); };
  return details::make_unique_view( //
      in, details::map_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto collect(view<C, Storage> &in, Function &&function, tag = {}) {
  auto applied = [function](auto &&x) { return details::ap(function, x); };
  return details::make_unique_view( //
      in, details::collect_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto collect_first(view<C, Storage> &in, Function &&function, tag = {}) {
  auto applied = [function](auto &&x) { return details::ap(function, x); };
  return details::make_unique_view( //
      in, details::collect_iterator(in.begin(), in.end(), applied)) | head_maybe();
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto filter(view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  auto applied = [predicate](auto &&x) { return details::ap(predicate, x); };
  return details::make_unique_view( //
      in, details::filter_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto flat_map(view<C, Storage> &in, Function &&function, tag = {}) {
  auto applied = [function](auto &&x) { return details::ap(function, x); };
  return details::make_unique_view( //
      in, details::flat_map_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto flatten(view<C, Storage> &in, tag = {}) {
  auto identity = [](auto &&x) { return x; };
  return details::make_unique_view( //
      in, details::flat_map_iterator(in.begin(), in.end(), identity));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto distinct_by(view<C, Storage> &in, Function &&function, tag = {}) {
  auto applied = [function](auto &&x) { return details::ap(function, x); };
  return details::make_unique_view( //
      in, details::distinct_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto distinct(view<C, Storage> &in, tag = {}) {
  return distinct_by(in, [](auto &&x) { return x; });
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto count(const view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  return details::container1::count<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto exists(const view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  return details::container1::exists<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto forall(const view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  return details::container1::forall<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto find(const view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  return details::container1::find<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto contains(const view<C, Storage> &in, const T &t, tag = {}) {
  return details::container1::contains<view<C, Storage>, T>(in, t);
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto reduce(const view<C, Storage> &in, Function &&function, tag = {}) {
  return details::container1::reduce<view<C, Storage>, Function>(in, function);
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto tap_each(view<C, Storage> &in, Function &&function, tag = {}) {
  auto applied = [function](auto &&x) { details::ap(function, x); };
  return details::make_unique_view( //
      in, details::tap_each_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto for_each(const view<C, Storage> &in, Function &&function, tag = {}) {
  details::container1::for_each<view<C, Storage>, Function>(in, function);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto partition(view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  return details::use_shared(in.storage, [&](auto &&s) {
    return std::pair{view(s, details::filter_iterator(in.begin(), in.end(), [predicate](auto &&x) { return details::ap(predicate, x); })),
                     view(s, details::filter_iterator(in.begin(), in.end(), [predicate](auto &&x) { return !details::ap(predicate, x); }))};
  });
}

template <typename C, typename Storage, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] constexpr auto group_map_reduce(const view<C, Storage> &in, GroupFunction &&group, MapFunction &&map, ReduceFunction &&reduce,
                                              tag = {}) {
  return details::container1::group_map_reduce<view<C, Storage>, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename C, typename Storage, typename GroupFunction, typename MapFunction> //
[[nodiscard]] constexpr auto group_map(const view<C, Storage> &in, GroupFunction &&group, MapFunction &&map, tag = {}) {
  return details::container1::group_map<view<C, Storage>, GroupFunction, MapFunction, std::vector>(in, group, map);
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto group_by(const view<C, Storage> &in, Function &&function, tag = {}) {
  return details::container1::group_by<view<C, Storage>, Function, std::vector>(in, function);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto to_vector(const view<C, Storage> &in, tag = {}) {
  return std::vector<typename view<C, Storage>::value_type>{in.begin(), in.end()};
}

template <template <typename...> typename Cs, typename C, typename Storage> //
[[nodiscard]] constexpr auto to(const view<C, Storage> &in, tag = {}) {
  using T = typename view<C, Storage>::value_type;
  if constexpr (is_pair<T> && !is_unary_instantiable<Cs, T>) {
    return Cs<typename T::first_type, typename T::second_type>{in.begin(), in.end()};
  } else {
    return Cs<T>{in.begin(), in.end()};
  }
}

// == sequence

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto prepend(view<C, Storage> &in, const T &t, tag = {}) {
  return details::make_unique_view(                                                            //
      in, details::append_prepend_iterator<details::append_prepend_iterator_mode::prepend, C>( //
              in.begin(), in.end(), t));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto head_maybe(const view<C, Storage> &in, tag = {}) {
  return details::sequence1::head_maybe<view<C, Storage>>(in);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto last_maybe(const view<C, Storage> &in, tag = {}) {
  return details::sequence1::last_maybe<view<C, Storage>>(in);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto init(const view<C, Storage> &, tag = {}) {
  static_assert(details::is_supported<C>,
                "init cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto tail(view<C, Storage> &in, tag = {}) {
  return details::make_unique_view( //
      in, details::slice_iterator(in.begin(), in.end(), 1, std::numeric_limits<size_t>::max()));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto at_maybe(const view<C, Storage> &in, size_t idx, tag = {}) {
  return details::sequence1::at_maybe<view<C, Storage>>(in, idx);
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto slice(view<C, Storage> &in, size_t from_inclusive, size_t to_exclusive, tag = {}) {
  return details::make_unique_view( //
      in, details::slice_iterator(in.begin(), in.end(), from_inclusive, to_exclusive));
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto index_of_slice(const view<C, Storage> &in, const Container &other, tag = {}) {
  return details::sequence1::index_of_slice<view<C, Storage>, Container>(in, other);
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto contains_slice(const view<C, Storage> &in, const Container &other, tag = {}) {
  return details::sequence1::index_of_slice<view<C, Storage>, Container>(in, other) != -1;
}

template <typename C, typename Storage, typename T> //
[[nodiscard]] constexpr auto index_of(const view<C, Storage> &in, const T &t, tag = {}) {
  return details::sequence1::index_of<view<C, Storage>>(in, t);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto find_last(const view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::find_last<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto index_where(const view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::index_where<view<C, Storage>, Predicate>(in, predicate);
}

template <typename C, typename Storage, typename N> //
[[nodiscard]] constexpr auto zip_with_index(view<C, Storage> &in, N from, tag = {}) {
  auto idx = iterate(from, [](auto &&x) { return x + 1; });
  return details::make_unique_view( //
      in, details::zip_iterator(in.begin(), in.end(), idx.begin(), idx.end()));
}

template <typename C, typename Storage, typename Container> //
[[nodiscard]] constexpr auto zip(view<C, Storage> &in, const Container &other, tag = {}) {
  return details::make_unique_view( //
      in, details::zip_iterator(in.begin(), in.end(), other.begin(), other.end()));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto transpose(const view<C, Storage> &, tag = {}) {
  static_assert(details::is_supported<C>,
                "transpose cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto sequence(const view<C, Storage> &, tag = {}) {
  static_assert(details::is_supported<C>,
                "sequence cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto reverse(const view<C, Storage> &, tag = {}) {
  static_assert(details::is_supported<C>,
                "reverse cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage, typename URBG> //
[[nodiscard]] constexpr auto shuffle(const view<C, Storage> &, URBG &&, tag = {}) {
  static_assert(details::is_supported<C>,
                "shuffle cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto sort(const view<C, Storage> &, tag = {}) {
  static_assert(details::is_supported<C>,
                "sort cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage, typename Compare> //
[[nodiscard]] constexpr auto sort(const view<C, Storage> &, Compare &&, tag = {}) {
  static_assert(details::is_supported<C>,
                "sort cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage, typename Select> //
[[nodiscard]] constexpr auto sort_by(const view<C, Storage> &, Select &&, tag = {}) {
  static_assert(details::is_supported<C>,
                "sort_by cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto split_at(view<C, Storage> &in, size_t idx, tag = {}) {
  return details::use_shared(in.storage, [&](auto &&s) {
    return std::pair{view(s, details::slice_iterator(in.begin(), in.end(), 0, idx)),
                     view(s, details::slice_iterator(in.begin(), in.end(), idx, std::numeric_limits<size_t>::max()))};
  });
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto take_right(const view<C, Storage> &, size_t, tag = {}) {
  static_assert(details::is_supported<C>,
                "take_right cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto drop_right(const view<C, Storage> &, size_t, tag = {}) {
  static_assert(details::is_supported<C>,
                "drop_right cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto take(view<C, Storage> &in, size_t n, tag = {}) {
  return details::make_unique_view( //
      in, details::slice_iterator(in.begin(), in.end(), 0, n));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto drop(view<C, Storage> &in, size_t n, tag = {}) {
  return details::make_unique_view( //
      in, details::slice_iterator(in.begin(), in.end(), n, std::numeric_limits<size_t>::max()));
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto take_while(view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  auto applied = [predicate](auto &&x) { return details::ap(predicate, x); };
  return details::make_unique_view( //
      in,
      details::drop_take_iterator<details::drop_take_iterator_mode::take_while_true, C, decltype(applied)>(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto drop_while(view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  auto applied = [predicate](auto &&x) { return details::ap(predicate, x); };
  return details::make_unique_view( //
      in,
      details::drop_take_iterator<details::drop_take_iterator_mode::drop_while_true, C, decltype(applied)>(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Predicate> //
[[nodiscard]] constexpr auto span(view<C, Storage> &in, Predicate &&predicate, tag = {}) {
  auto applied = [predicate](auto &&x) { return details::ap(predicate, x); };
  return std::pair{details::make_unique_view( //
                       in, details::drop_take_iterator<details::drop_take_iterator_mode::take_while_true, C, decltype(applied)>(
                               in.begin(), in.end(), applied)),
                   details::make_unique_view( //
                       in, details::drop_take_iterator<details::drop_take_iterator_mode::drop_while_true, C, decltype(applied)>(
                               in.begin(), in.end(), applied))};
}

template <typename C, typename Storage, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_left(const view<C, Storage> &in, Accumulator &&init, Function &&function, tag = {}) {
  return details::sequence1::fold_left<view<C, Storage>, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C, typename Storage, typename Accumulator, typename Function> //
[[nodiscard]] constexpr auto fold_right(const view<C, Storage> &, Accumulator &&, Function &&, tag = {}) {
  static_assert(details::is_supported<C>,
                "fold_right cannot be implemented optimally and may not terminate, consider converting to a container first");
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto sliding(view<C, Storage> &in, size_t size, size_t step, tag = {}) {
  return details::make_unique_view( //
      in, details::sliding_iterator<C, view>(in.begin(), in.end(), size, step));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto keys(view<C, Storage> &in, tag = {}) {
  static_assert(is_pair<typename details::value_type_of_t<C>>, "keys operation requires a pair for the value type");
  return details::make_unique_view( //
      in, details::map_iterator(in.begin(), in.end(), [](auto &&x) { return x.first; }));
}

template <typename C, typename Storage> //
[[nodiscard]] constexpr auto values(view<C, Storage> &in, tag = {}) {
  static_assert(is_pair<typename details::value_type_of_t<C>>, "values operation requires a pair for the value type");
  return details::make_unique_view( //
      in, details::map_iterator(in.begin(), in.end(), [](auto &&x) { return x.second; }));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto map_keys(view<C, Storage> &in, Function &&function, tag = {}) {
  static_assert(is_pair<typename details::value_type_of_t<C>>, "map_keys operation requires a pair for the value type");
  auto applied = [function](auto &&x) { return details::ap(function, x.first); };
  return details::make_unique_view( //
      in, details::map_iterator(in.begin(), in.end(), applied));
}

template <typename C, typename Storage, typename Function> //
[[nodiscard]] constexpr auto map_values(view<C, Storage> &in, Function &&function, tag = {}) {
  static_assert(is_pair<typename details::value_type_of_t<C>>, "map_values operation requires a pair for the value type");
  auto applied = [function](auto &&x) { return details::ap(function, x.second); };
  return details::make_unique_view( //
      in, details::map_iterator(in.begin(), in.end(), applied));
}

} // namespace aspartame
