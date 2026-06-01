#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef ASPARTAME_SHOW_STREAM
  #include <sstream>
#endif

#include "details/container1_impl.hpp"
#include "details/container2_impl.hpp"
#include "details/inplace_impl.hpp"
#include "details/map_impl.hpp"
#include "details/sequence1_impl.hpp"
#include "traits.hpp"

namespace aspartame {

struct inplace {};

template <typename Out, typename In> struct dual_op {
  Out out;
  In in;
  template <typename O> constexpr auto operator()(O &&o, tag) const -> decltype(out(std::forward<O>(o))) { return out(std::forward<O>(o)); }
  template <typename O> constexpr auto operator()(O &o, inplace) const -> decltype(in(o)) { return in(o); }
};
template <typename Out, typename In> dual_op(Out, In) -> dual_op<Out, In>;

template <typename In> struct inplace_op {
  In in;
  template <typename O> constexpr auto operator()(O &o, inplace) const -> decltype(in(o)) { return in(o); }
};
template <typename In> inplace_op(In) -> inplace_op<In>;

template <typename L, typename Op, typename = std::enable_if_t<enable_pipe<std::decay_t<L>>::value>>
constexpr auto operator^(const L &l, const Op &r) -> decltype(r(l, tag{})) {
  return r(l, tag{});
}

template <typename L, typename Op, typename = std::enable_if_t<enable_pipe<std::decay_t<L>>::value>>
constexpr auto &operator^=(L &l, const Op &r) {
  if constexpr (std::is_invocable_v<const Op &, L &, inplace>) r(l, inplace{});
  else l = r(l, tag{});
  return l;
}

namespace seq_impl {
template <typename C> using V = typename sequence_traits<std::decay_t<C>>::value_type;
template <typename C, typename U> using Reb = typename sequence_traits<std::decay_t<C>>::template rebind<U>;
template <typename C> using Same = Reb<C, V<C>>;
template <typename C, typename = void> inline constexpr bool IsFixedSize = false;
template <typename C>
inline constexpr bool IsFixedSize<C, std::void_t<decltype(sequence_traits<std::decay_t<C>>::fixed_size)>> =
    static_cast<bool>(sequence_traits<std::decay_t<C>>::fixed_size);
// Shape-changing ops (filter, take, etc.) demote to std::vector for fixed-size receivers like std::array.
template <typename C> using Shrink = std::conditional_t<IsFixedSize<C>, std::vector<V<C>>, Same<C>>;
template <typename C> inline constexpr bool SetLike = sequence_traits<std::decay_t<C>>::set_like;
} // namespace seq_impl

#define ASPARTAME_SEQ_TPL(...) typename C, __VA_ARGS__, std::enable_if_t<details::has_sequence_traits_v<C>, int> = 0
#define ASPARTAME_SEQ_TPL_NOTYPE typename C, std::enable_if_t<details::has_sequence_traits_v<C>, int> = 0

template <ASPARTAME_SEQ_TPL(typename F)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto mk_string(const C &in, std::string_view prefix, std::string_view sep, std::string_view suffix,
                                                       F &&f, tag = {}) {
  return details::container1::mk_string<C, F>(in, prefix, sep, suffix, std::forward<F>(f));
}

template <ASPARTAME_SEQ_TPL(typename T)> [[nodiscard]] constexpr auto append(const C &in, const T &t, tag = {}) {
  return details::container1::append<C, seq_impl::Shrink<C>, T>(in, t);
}

template <ASPARTAME_SEQ_TPL(typename Container)> [[nodiscard]] constexpr auto concat(const C &in, const Container &c, tag = {}) {
  return details::container1::concat<C, seq_impl::Shrink<C>, Container>(in, c);
}

template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto map(const C &in, F &&f, tag = {}) {
  using U = std::decay_t<decltype(details::ap(f, *details::seq_view(in).begin()))>;
  return details::container1::map<C, seq_impl::Reb<C, U>, F>(in, std::forward<F>(f));
}

template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto collect(const C &in, F &&f, tag = {}) {
  using R = std::decay_t<decltype(details::ap(f, *details::seq_view(in).begin()))>;
  using U = typename R::value_type;
  return details::container1::collect<C, seq_impl::Reb<C, U>, F>(in, std::forward<F>(f));
}

template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto filter(const C &in, P &&p, tag = {}) {
  return details::container1::filter<C, seq_impl::Shrink<C>, P>(in, std::forward<P>(p));
}

template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto flat_map(const C &in, F &&f, tag = {}) {
  using R = std::decay_t<decltype(details::ap(f, *details::seq_view(in).begin()))>;
  using U = typename R::value_type;
  return details::container1::flat_map<C, seq_impl::Reb<C, U>, F>(in, std::forward<F>(f));
}

template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto flatten(const C &in, tag = {}) {
  static_assert(is_iterable<seq_impl::V<C>>, "flatten requires each element to be iterable");
  using U = typename seq_impl::V<C>::value_type;
  return details::container1::flatten<C, seq_impl::Reb<C, U>>(in);
}

template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto distinct_by(const C &in, F &&f, tag = {}) {
  return details::container1::distinct_by<C, seq_impl::Shrink<C>, F>(in, std::forward<F>(f));
}

template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto distinct(const C &in, tag = {}) {
  return details::container1::distinct<C, seq_impl::Shrink<C>, seq_impl::SetLike<C>>(in);
}

template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto count(const C &in, P &&p, tag = {}) {
  return details::container1::count<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto exists(const C &in, P &&p, tag = {}) {
  return details::container1::exists<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto forall(const C &in, P &&p, tag = {}) {
  return details::container1::forall<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto find(const C &in, P &&p, tag = {}) {
  return details::container1::find<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename T)> [[nodiscard]] constexpr auto contains(const C &in, const T &t, tag = {}) {
  return details::container1::contains<C, T>(in, t);
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto reduce(const C &in, F &&f, tag = {}) {
  return details::container1::reduce<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto min(const C &in, tag = {}) { return details::container1::min<C>(in); }
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto max(const C &in, tag = {}) { return details::container1::max<C>(in); }
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto min_by(const C &in, F &&f, tag = {}) {
  return details::container1::min_by<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto max_by(const C &in, F &&f, tag = {}) {
  return details::container1::max_by<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto index_of_min(const C &in, tag = {}) {
  return details::container1::index_of_min<C>(in);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto index_of_max(const C &in, tag = {}) {
  return details::container1::index_of_max<C>(in);
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto index_of_min_by(const C &in, F &&f, tag = {}) {
  return details::container1::index_of_min_by<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto index_of_max_by(const C &in, F &&f, tag = {}) {
  return details::container1::index_of_max_by<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto sum(const C &in, tag = {}) { return details::container1::sum<C>(in); }
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto product(const C &in, tag = {}) {
  return details::container1::product<C>(in);
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto sum_by(const C &in, F &&f, tag = {}) {
  return details::container1::sum_by<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto none_match(const C &in, P &&p, tag = {}) {
  return !details::container1::exists<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename Other)> [[nodiscard]] constexpr auto intersect(const C &in, const Other &other, tag = {}) {
  return details::container1::intersect<C, seq_impl::Shrink<C>, Other>(in, other);
}
template <ASPARTAME_SEQ_TPL(typename Other)> [[nodiscard]] constexpr auto diff(const C &in, const Other &other, tag = {}) {
  return details::container1::diff<C, seq_impl::Shrink<C>, Other>(in, other);
}
template <ASPARTAME_SEQ_TPL(typename Other)> [[nodiscard]] constexpr auto symmetric_difference(const C &in, const Other &other, tag = {}) {
  return details::container1::symmetric_difference<C, seq_impl::Shrink<C>, Other>(in, other);
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto tap_each(const C &in, F &&f, tag = {}) {
  return details::container1::tap_each<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename F)> constexpr void for_each(const C &in, F &&f, tag = {}) {
  details::container1::for_each<C, F>(in, std::forward<F>(f));
}

template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto partition(const C &in, P &&p, tag = {}) {
  return details::container1::partition<C, seq_impl::Shrink<C>, P>(in, std::forward<P>(p));
}

template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto partition_map(const C &in, F &&f, tag = {}) {
  using R = std::decay_t<decltype(details::ap(f, *details::seq_view(in).begin()))>;
  static_assert(details::has_expected_traits_v<R>, "partition_map function must return an expected-like type");
  using ET = expected_traits<R>;
  using B = typename ET::value_type;
  using E = typename ET::error_type;
  using Errs = typename seq_impl::Reb<C, E>;
  using Oks = typename seq_impl::Reb<C, B>;
  Errs errs;
  Oks oks;
  if constexpr (details::has_reserve<Errs> && details::has_size<C>) errs.reserve(in.size());
  if constexpr (details::has_reserve<Oks> && details::has_size<C>) oks.reserve(in.size());
  for (auto &&x : details::seq_view(in)) {
    auto r = details::ap(f, x);
    if (ET::engaged(r)) details::push(oks, ET::get(r));
    else details::push(errs, ET::error(r));
  }
  return std::pair{std::move(errs), std::move(oks)};
}

template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto traverse(const C &in, F &&f, tag = {}) {
  using GofB = std::decay_t<decltype(details::ap(f, *details::seq_view(in).begin()))>;
  if constexpr (details::has_optional_traits_v<GofB>) {
    using OT = optional_traits<GofB>;
    using B = typename OT::value_type;
    using ResultInner = typename seq_impl::Reb<C, B>;
    using Result = typename OT::template rebind<ResultInner>;
    ResultInner out;
    if constexpr (details::has_reserve<ResultInner> && details::has_size<C>) out.reserve(in.size());
    for (auto &&x : details::seq_view(in)) {
      auto gb = details::ap(f, x);
      if (!OT::engaged(gb)) return Result{};
      details::push(out, OT::get(gb));
    }
    return Result{std::move(out)};
  } else if constexpr (details::has_expected_traits_v<GofB>) {
    using ET = expected_traits<GofB>;
    using B = typename ET::value_type;
    using E = typename ET::error_type;
    using ResultInner = typename seq_impl::Reb<C, B>;
    using Result = typename ET::template rebind_value<ResultInner>;
    using ResultET = expected_traits<Result>;
    ResultInner out;
    if constexpr (details::has_reserve<ResultInner> && details::has_size<C>) out.reserve(in.size());
    for (auto &&x : details::seq_view(in)) {
      auto gb = details::ap(f, x);
      if (!ET::engaged(gb)) return ResultET::template make_error<E>(ET::error(gb));
      details::push(out, ET::get(gb));
    }
    return ResultET::template make_value<ResultInner>(std::move(out));
  } else {
    static_assert(details::has_optional_traits_v<GofB> || details::has_expected_traits_v<GofB>,
                  "traverse function must return an optional-like or expected-like type");
  }
}

template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sequence(const C &in, tag = {}) {
  return traverse(in, [](auto &&x) { return x; }, tag{});
}

template <ASPARTAME_SEQ_TPL(typename T)> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto intersperse(const C &in, const T &sep, tag = {}) {
  using Out = seq_impl::Same<C>;
  Out ys;
  if constexpr (details::has_reserve<Out> && details::has_size<C>) ys.reserve(in.size() ? in.size() * 2 - 1 : 0);
  bool first = true;
  for (auto &&x : details::seq_view(in)) {
    if (!first) details::push(ys, sep);
    first = false;
    details::push(ys, x);
  }
  return ys;
}

template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto combine_all(const C &in, tag = {}) {
  using T = seq_impl::V<C>;
  static_assert(details::has_monoid_traits_v<T>, "combine_all requires a monoid_traits specialisation for the element type");
  T acc = monoid_traits<T>::empty();
  for (auto &&x : details::seq_view(in))
    acc = monoid_traits<T>::combine(std::move(acc), x);
  return acc;
}

template <ASPARTAME_SEQ_TPL(typename G, typename M, typename R)>
[[nodiscard]] constexpr auto group_map_reduce(const C &in, G &&g, M &&m, R &&r, tag = {}) {
  return details::container1::group_map_reduce<C, G, M, R>(in, std::forward<G>(g), std::forward<M>(m), std::forward<R>(r));
}

template <ASPARTAME_SEQ_TPL(typename G, typename M)> [[nodiscard]] constexpr auto group_map(const C &in, G &&g, M &&m, tag = {}) {
  using U = std::decay_t<decltype(details::ap(m, *details::seq_view(in).begin()))>;
  using Inner = seq_impl::Reb<C, U>;
  return details::container1::group_map<C, Inner, G, M>(in, std::forward<G>(g), std::forward<M>(m));
}

template <ASPARTAME_SEQ_TPL(typename G)> [[nodiscard]] constexpr auto group_by(const C &in, G &&g, tag = {}) {
  using Inner = seq_impl::Shrink<C>;
  return details::container1::group_by<C, Inner, G>(in, std::forward<G>(g));
}

template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto to_vector(const C &in, tag = {}) {
  return std::vector<seq_impl::V<C>>{details::seq_view(in).begin(), details::seq_view(in).end()};
}

template <template <typename...> typename Cs, ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto to(const C &in, tag = {}) {
  using T = seq_impl::V<C>;
  if constexpr (is_unary_instantiable<Cs, T>) return Cs<T>{details::seq_view(in).begin(), details::seq_view(in).end()};
  else if constexpr (is_pair<T>)
    return Cs<typename T::first_type, typename T::second_type>{details::seq_view(in).begin(), details::seq_view(in).end()};
  else return Cs<T>{details::seq_view(in).begin(), details::seq_view(in).end()};
}

template <ASPARTAME_SEQ_TPL(typename T)> [[nodiscard]] constexpr auto prepend(const C &in, const T &t, tag = {}) {
  return details::sequence1::prepend<C, T, seq_impl::Shrink<C>>(in, t);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto head_maybe(const C &in, tag = {}) {
  return details::sequence1::head_maybe<C>(in);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto last_maybe(const C &in, tag = {}) {
  return details::sequence1::last_maybe<C>(in);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto init(const C &in, tag = {}) {
  return details::sequence1::init<C, seq_impl::Shrink<C>>(in);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto tail(const C &in, tag = {}) {
  return details::sequence1::tail<C, seq_impl::Shrink<C>>(in);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto at_maybe(const C &in, size_t idx, tag = {}) {
  return details::sequence1::at_maybe<C>(in, idx);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto slice(const C &in, size_t from_inclusive, size_t to_exclusive, tag = {}) {
  return details::sequence1::slice<C, seq_impl::Shrink<C>>(in, from_inclusive, to_exclusive);
}
template <ASPARTAME_SEQ_TPL(typename T)> [[nodiscard]] constexpr auto index_of(const C &in, const T &t, tag = {}) {
  return details::sequence1::index_of<C, T>(in, t);
}
template <ASPARTAME_SEQ_TPL(typename Container)>
[[nodiscard]] constexpr auto index_of_slice(const C &in, const Container &other, tag = {}) {
  return details::sequence1::index_of_slice<C, Container>(in, other);
}
template <ASPARTAME_SEQ_TPL(typename Container)>
[[nodiscard]] constexpr auto contains_slice(const C &in, const Container &other, tag = {}) {
  return details::sequence1::index_of_slice<C, Container>(in, other) != -1;
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto index_where(const C &in, P &&p, tag = {}) {
  return details::sequence1::index_where<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename T)> [[nodiscard]] constexpr auto last_index_of(const C &in, const T &t, tag = {}) {
  return details::sequence1::last_index_of<C, T>(in, t);
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto last_index_where(const C &in, P &&p, tag = {}) {
  return details::sequence1::last_index_where<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename Acc, typename F)> [[nodiscard]] constexpr auto scan_left(const C &in, Acc init, F &&f, tag = {}) {
  using B = std::decay_t<Acc>;
  return details::sequence1::scan_left<C, B, F, seq_impl::Reb<C, B>>(in, std::move(init), std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename Acc, typename F)> [[nodiscard]] constexpr auto scan_right(const C &in, Acc init, F &&f, tag = {}) {
  using B = std::decay_t<Acc>;
  return details::sequence1::scan_right<C, B, F, seq_impl::Reb<C, B>>(in, std::move(init), std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto unzip(const C &in, tag = {}) {
  using P = seq_impl::V<C>;
  using A = std::decay_t<typename P::first_type>;
  using B = std::decay_t<typename P::second_type>;
  return details::sequence1::unzip<C, seq_impl::Reb<C, A>, seq_impl::Reb<C, B>>(in);
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto collect_first(const C &in, F &&f, tag = {}) {
  return details::sequence1::collect_first<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto find_last(const C &in, P &&p, tag = {}) {
  return details::sequence1::find_last<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename N)> [[nodiscard]] constexpr auto zip_with_index(const C &in, N from, tag = {}) {
  // XXX decay so cv-qualified deref types (e.g. `const T*const`) don't make the pair non-copy-assignable
  using Pair = std::pair<std::decay_t<seq_impl::V<C>>, N>;
  return details::sequence1::zip_with_index<C, seq_impl::Reb<C, Pair>, N>(in, from);
}
template <ASPARTAME_SEQ_TPL(typename N)> [[nodiscard]] constexpr auto enumerate(const C &in, N from, tag = {}) {
  return zip_with_index(in, from, tag{});
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto enumerate(const C &in, tag = {}) {
  return zip_with_index(in, size_t{0}, tag{});
}
template <ASPARTAME_SEQ_TPL(typename Container)> [[nodiscard]] constexpr auto zip(const C &in, const Container &other, tag = {}) {
  using U = typename std::decay_t<Container>::value_type;
  using Pair = std::pair<seq_impl::V<C>, U>;
  return details::sequence1::zip<C, Container, seq_impl::Reb<C, Pair>>(in, other);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto transpose(const C &in, tag = {}) {
  using T = typename seq_impl::V<C>::value_type;
  using Inner = seq_impl::Reb<C, T>;
  using Outer = seq_impl::Reb<C, Inner>;
  return details::sequence1::transpose<C, Inner, Outer>(in);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto cartesian_product(const C &in, tag = {}) {
  using T = typename seq_impl::V<C>::value_type;
  using Inner = seq_impl::Reb<C, T>;
  using Outer = seq_impl::Reb<C, Inner>;
  return details::sequence1::cartesian_product<C, Inner, Outer>(in);
}
template <ASPARTAME_SEQ_TPL(typename Other)> [[nodiscard]] constexpr auto cross(const C &in, const Other &other, tag = {}) {
  using A = seq_impl::V<C>;
  using B = typename std::decay_t<Other>::value_type;
  using Pair = std::pair<A, B>;
  using Out = seq_impl::Reb<C, Pair>;
  return details::sequence1::cross<C, Other, Out>(in, other);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto combinations(const C &in, size_t k, tag = {}) {
  using Inner = seq_impl::Shrink<C>;
  using Outer = seq_impl::Reb<C, Inner>;
  return details::sequence1::combinations<C, Inner, Outer>(in, k);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto permutations(const C &in, tag = {}) {
  using Inner = seq_impl::Shrink<C>;
  using Outer = seq_impl::Reb<C, Inner>;
  return details::sequence1::permutations<C, Inner, Outer>(in);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto stride(const C &in, size_t n, tag = {}) {
  return details::sequence1::stride<C, seq_impl::Shrink<C>>(in, n);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto pairwise(const C &in, tag = {}) {
  using T = seq_impl::V<C>;
  using P = std::pair<T, T>;
  return details::sequence1::pairwise<C, seq_impl::Reb<C, P>>(in);
}
template <ASPARTAME_SEQ_TPL(typename Pred)> [[nodiscard]] constexpr auto chunk_by(const C &in, Pred &&p, tag = {}) {
  using Inner = seq_impl::Shrink<C>;
  using Outer = seq_impl::Reb<C, Inner>;
  return details::sequence1::chunk_by<C, Inner, Outer, Pred>(in, std::forward<Pred>(p));
}
template <ASPARTAME_SEQ_TPL(typename Sep)> [[nodiscard]] constexpr auto join_with(const C &in, const Sep &sep, tag = {}) {
  using T = typename seq_impl::V<C>::value_type;
  return details::sequence1::join_with<C, Sep, seq_impl::Reb<C, T>>(in, sep);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto is_sorted(const C &in, tag = {}) {
  return details::sequence1::is_sorted<C>(in);
}
template <ASPARTAME_SEQ_TPL(typename F)> [[nodiscard]] constexpr auto is_sorted_by(const C &in, F &&f, tag = {}) {
  return details::sequence1::is_sorted_by<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename URBG)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sample(const C &in, size_t k, URBG &&urbg, tag = {}) {
  return details::sequence1::sample<C, URBG, seq_impl::Shrink<C>>(in, k, std::forward<URBG>(urbg));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto top_k(const C &in, size_t k, tag = {}) {
  return details::sequence1::top_k<C, seq_impl::Shrink<C>>(in, k);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto bottom_k(const C &in, size_t k, tag = {}) {
  return details::sequence1::bottom_k<C, seq_impl::Shrink<C>>(in, k);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto reverse(const C &in, tag = {}) {
  return details::sequence1::reverse<C, seq_impl::Shrink<C>>(in);
}
template <ASPARTAME_SEQ_TPL(typename URBG)> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto shuffle(const C &in, URBG &&urbg, tag = {}) {
  return details::sequence1::shuffle<C, URBG, seq_impl::Shrink<C>>(in, std::forward<URBG>(urbg));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sort(const C &in, tag = {}) {
  return details::sequence1::sort<C, seq_impl::Shrink<C>>(in);
}
template <ASPARTAME_SEQ_TPL(typename Cmp)> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sort(const C &in, Cmp &&cmp, tag = {}) {
  return details::sequence1::sort<C, Cmp, seq_impl::Shrink<C>>(in, std::forward<Cmp>(cmp));
}
template <ASPARTAME_SEQ_TPL(typename Sel)> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sort_by(const C &in, Sel &&s, tag = {}) {
  return details::sequence1::sort_by<C, Sel, seq_impl::Shrink<C>>(in, std::forward<Sel>(s));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto split_at(const C &in, size_t idx, tag = {}) {
  return details::sequence1::split_at<C, seq_impl::Shrink<C>>(in, idx);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto take(const C &in, size_t n, tag = {}) {
  return details::sequence1::take<C, seq_impl::Shrink<C>>(in, n);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto drop(const C &in, size_t n, tag = {}) {
  return details::sequence1::drop<C, seq_impl::Shrink<C>>(in, n);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto take_right(const C &in, size_t n, tag = {}) {
  return details::sequence1::take_right<C, seq_impl::Shrink<C>>(in, n);
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] constexpr auto drop_right(const C &in, size_t n, tag = {}) {
  return details::sequence1::drop_right<C, seq_impl::Shrink<C>>(in, n);
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto take_while(const C &in, P &&p, tag = {}) {
  return details::sequence1::take_while<C, P, seq_impl::Shrink<C>>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto drop_while(const C &in, P &&p, tag = {}) {
  return details::sequence1::drop_while<C, P, seq_impl::Shrink<C>>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename P)> [[nodiscard]] constexpr auto span(const C &in, P &&p, tag = {}) {
  return details::sequence1::span<C, P, seq_impl::Shrink<C>>(in, std::forward<P>(p));
}
template <ASPARTAME_SEQ_TPL(typename Acc, typename F)> [[nodiscard]] constexpr auto fold_left(const C &in, Acc init, F &&f, tag = {}) {
  return details::sequence1::fold_left<C, Acc, F>(in, std::move(init), std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL(typename Acc, typename F)> [[nodiscard]] constexpr auto fold_right(const C &in, Acc init, F &&f, tag = {}) {
  return details::sequence1::fold_right<C, Acc, F>(in, std::move(init), std::forward<F>(f));
}
template <ASPARTAME_SEQ_TPL_NOTYPE> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sliding(const C &in, size_t size, size_t step, tag = {}) {
  using Inner = seq_impl::Shrink<C>;
  using Outer = seq_impl::Reb<C, Inner>;
  return details::sequence1::sliding<C, Inner, Outer>(in, size, step);
}

#undef ASPARTAME_SEQ_TPL
#undef ASPARTAME_SEQ_TPL_NOTYPE

namespace map_impl {
template <typename C> using K = typename map_traits<std::decay_t<C>>::key_type;
template <typename C> using V = typename map_traits<std::decay_t<C>>::mapped_type;
template <typename C, typename K2, typename V2> using Reb = typename map_traits<std::decay_t<C>>::template rebind<K2, V2>;
template <typename C> using Same = Reb<C, K<C>, V<C>>;
template <typename C, typename Key> using KC = typename map_traits<std::decay_t<C>>::template key_container<Key>;
template <typename C, typename Val> using VC = typename map_traits<std::decay_t<C>>::template value_container<Val>;
template <typename C, typename Val> using GI = typename map_traits<std::decay_t<C>>::template group_inner<Val>;
} // namespace map_impl

#define ASPARTAME_MAP_TPL(...) typename C, __VA_ARGS__, std::enable_if_t<details::has_map_traits_v<C>, int> = 0
#define ASPARTAME_MAP_TPL_NOTYPE typename C, std::enable_if_t<details::has_map_traits_v<C>, int> = 0

namespace map_impl {
template <typename C> decltype(auto) ev(const C &in) { return map_access<C>::entries(in); }
template <typename C> using EntriesT = std::decay_t<decltype(map_access<C>::entries(std::declval<const C &>()))>;
} // namespace map_impl

template <ASPARTAME_MAP_TPL(typename F)>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto mk_string(const C &in, std::string_view prefix, std::string_view sep, std::string_view suffix,
                                                       F &&f, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return details::container1::mk_string<map_impl::EntriesT<C>, F>(es, prefix, sep, suffix, std::forward<F>(f));
}

template <ASPARTAME_MAP_TPL(typename T)> [[nodiscard]] constexpr auto append(const C &in, const T &t, tag = {}) {
  return details::container1::append<C, map_impl::Same<C>, T>(in, t);
}
template <ASPARTAME_MAP_TPL(typename Container)> [[nodiscard]] constexpr auto concat(const C &in, const Container &c, tag = {}) {
  return details::container1::concat<C, map_impl::Same<C>, Container>(in, c);
}

template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto map(const C &in, F &&f, tag = {}) {
  using Pair = std::decay_t<decltype(details::ap(f, *std::begin(map_access<C>::entries(in))))>;
  static_assert(details::is_pair_like_v<Pair>, "map() over a map-like container must return a pair");
  using K2 = std::decay_t<typename Pair::first_type>;
  using V2 = std::decay_t<typename Pair::second_type>;
  using Out = map_impl::Reb<C, K2, V2>;
  return details::container2::map<C, Out, F>(in, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto collect(const C &in, F &&f, tag = {}) {
  using R = std::decay_t<decltype(details::ap(f, *std::begin(map_access<C>::entries(in))))>;
  using P = typename R::value_type;
  using K2 = std::decay_t<typename P::first_type>;
  using V2 = std::decay_t<typename P::second_type>;
  using Out = map_impl::Reb<C, K2, V2>;
  return details::container2::collect<C, Out, F>(in, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto collect_first(const C &in, F &&f, tag = {}) {
  return details::container2::collect_first<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto filter(const C &in, P &&p, tag = {}) {
  return details::container2::filter<C, map_impl::Same<C>, P>(in, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto flat_map(const C &in, F &&f, tag = {}) {
  using R = std::decay_t<decltype(details::ap(f, *std::begin(map_access<C>::entries(in))))>;
  using K2 = std::decay_t<typename R::key_type>;
  using V2 = std::decay_t<typename R::mapped_type>;
  using Out = map_impl::Reb<C, K2, V2>;
  return details::container2::flat_map<C, Out, F>(in, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL_NOTYPE> [[nodiscard]] constexpr auto flatten(const C &in, tag = {}) {
  using Inner = map_impl::V<C>;
  using K2 = std::decay_t<typename Inner::key_type>;
  using V2 = std::decay_t<typename Inner::mapped_type>;
  using Out = map_impl::Reb<C, K2, V2>;
  return details::container2::flatten<C, Out>(in);
}

template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto exists(const C &in, P &&p, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return details::container1::exists<map_impl::EntriesT<C>, P>(es, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto forall(const C &in, P &&p, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return details::container1::forall<map_impl::EntriesT<C>, P>(es, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto count(const C &in, P &&p, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return details::container1::count<map_impl::EntriesT<C>, P>(es, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto find(const C &in, P &&p, tag = {}) {
  return details::container2::find<C, P>(in, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto reduce(const C &in, F &&f, tag = {}) {
  return details::container2::reduce<C, F>(in, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto tap_each(const C &in, F &&f, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  (void)details::container1::tap_each<map_impl::EntriesT<C>, F>(es, std::forward<F>(f));
  return in;
}
template <ASPARTAME_MAP_TPL(typename F)> constexpr void for_each(const C &in, F &&f, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  details::container1::for_each<map_impl::EntriesT<C>, F>(es, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto partition(const C &in, P &&p, tag = {}) {
  return details::container2::partition<C, map_impl::Same<C>, P>(in, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename G, typename M, typename R)>
[[nodiscard]] constexpr auto group_map_reduce(const C &in, G &&g, M &&m, R &&r, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return details::container1::group_map_reduce<map_impl::EntriesT<C>, G, M, R>(es, std::forward<G>(g), std::forward<M>(m),
                                                                               std::forward<R>(r));
}
template <ASPARTAME_MAP_TPL(typename G, typename M)> [[nodiscard]] constexpr auto group_map(const C &in, G &&g, M &&m, tag = {}) {
  using OutV = std::decay_t<decltype(details::ap(m, *std::begin(map_access<C>::entries(in))))>;
  using Inner = map_impl::GI<C, OutV>;
  return details::container2::group_map<C, Inner, G, M>(in, std::forward<G>(g), std::forward<M>(m));
}
template <ASPARTAME_MAP_TPL(typename G)> [[nodiscard]] constexpr auto group_by(const C &in, G &&g, tag = {}) {
  using P = std::pair<map_impl::K<C>, map_impl::V<C>>;
  using Inner = map_impl::GI<C, P>;
  return details::container2::group_by<C, Inner, G>(in, std::forward<G>(g));
}
template <ASPARTAME_MAP_TPL_NOTYPE> [[nodiscard]] constexpr auto to_vector(const C &in, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return std::vector<std::pair<map_impl::K<C>, map_impl::V<C>>>{std::begin(es), std::end(es)};
}
template <template <typename...> typename Cs, ASPARTAME_MAP_TPL_NOTYPE> [[nodiscard]] constexpr auto to(const C &in, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  using T = std::pair<map_impl::K<C>, map_impl::V<C>>;
  if constexpr (is_unary_instantiable<Cs, T>) return Cs<T>{std::begin(es), std::end(es)};
  else return Cs<map_impl::K<C>, map_impl::V<C>>{std::begin(es), std::end(es)};
}
template <ASPARTAME_MAP_TPL(typename Acc, typename F)> [[nodiscard]] constexpr auto fold_left(const C &in, Acc init, F &&f, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return details::sequence1::fold_left<map_impl::EntriesT<C>, Acc, F>(es, std::move(init), std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename Acc, typename F)> [[nodiscard]] constexpr auto fold_right(const C &in, Acc init, F &&f, tag = {}) {
  auto &&es = map_access<C>::entries(in);
  return details::sequence1::fold_right<map_impl::EntriesT<C>, Acc, F>(es, std::move(init), std::forward<F>(f));
}

template <ASPARTAME_MAP_TPL_NOTYPE> [[nodiscard]] constexpr auto keys(const C &in, tag = {}) {
  return details::container2::keys<C, map_impl::KC<C, map_impl::K<C>>>(in);
}
template <ASPARTAME_MAP_TPL_NOTYPE> [[nodiscard]] constexpr auto values(const C &in, tag = {}) {
  return details::container2::values<C, map_impl::VC<C, map_impl::V<C>>>(in);
}
template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto map_keys(const C &in, F &&f, tag = {}) {
  using K2 = std::decay_t<decltype(details::ap(f, (*std::begin(map_access<C>::entries(in))).first))>;
  using Out = map_impl::Reb<C, K2, map_impl::V<C>>;
  return details::container2::map_keys<C, Out, F>(in, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename F)> [[nodiscard]] constexpr auto map_values(const C &in, F &&f, tag = {}) {
  using V2 = std::decay_t<decltype(details::ap(f, (*std::begin(map_access<C>::entries(in))).second))>;
  using Out = map_impl::Reb<C, map_impl::K<C>, V2>;
  return details::container2::map_values<C, Out, F>(in, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto filter_keys(const C &in, P &&p, tag = {}) {
  return details::container2::filter_keys<C, map_impl::Same<C>, P>(in, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename P)> [[nodiscard]] constexpr auto filter_values(const C &in, P &&p, tag = {}) {
  return details::container2::filter_values<C, map_impl::Same<C>, P>(in, std::forward<P>(p));
}
template <ASPARTAME_MAP_TPL(typename Other, typename F)>
[[nodiscard]] constexpr auto combine(const C &in, const Other &other, F &&f, tag = {}) {
  return details::container2::combine<C, map_impl::Same<C>, Other, F>(in, other, std::forward<F>(f));
}
template <ASPARTAME_MAP_TPL(typename Key)> [[nodiscard]] constexpr auto get_maybe(const C &in, const Key &key, tag = {}) {
  return details::container2::get_maybe<C, Key>(in, key);
}
template <ASPARTAME_MAP_TPL(typename Key, typename Value)>
[[nodiscard]] constexpr auto get_or_default(const C &in, const Key &k, const Value &v, tag = {}) {
  return details::container2::get_or_default<C, Key, Value>(in, k, v);
}
template <ASPARTAME_MAP_TPL(typename Key, typename F)> constexpr auto get_or_emplace(C &in, const Key &key, F &&f, tag = {}) {
  if (auto existing = map_access<C>::lookup(in, key)) return *existing;
  map_impl::V<C> value = details::ap(f, key);
  map_access<C>::put(in, key, value);
  return value;
}

#undef ASPARTAME_MAP_TPL
#undef ASPARTAME_MAP_TPL_NOTYPE

constexpr auto show_string = [](const auto &x) -> std::string {
  using T = std::decay_t<decltype(x)>;
  if constexpr (std::is_same_v<T, std::string>) return x;
  else if constexpr (std::is_same_v<T, char>) return std::string(1, x);
  else if constexpr (std::is_convertible_v<decltype(x), std::string_view>) return std::string{std::string_view{x}};
  else if constexpr (std::is_arithmetic_v<T>) return std::to_string(x);
  else {
#ifdef ASPARTAME_SHOW_STREAM
    std::stringstream out;
    out << x;
    return out.str();
#else
    static_assert(sizeof(T) == 0, "mk_string default formatter handles arithmetic and string-like only; "
                                  "pass an explicit formatter or define ASPARTAME_SHOW_STREAM to enable the ostream<< fallback");
    return {};
#endif
  }
};

template <typename F> [[nodiscard]] constexpr auto map(F f) {
  return dual_op{[f](auto &&o) { return map(o, f, tag{}); },
                 [f](auto &o) -> std::enable_if_t<details::inplace::can_map_inplace<std::decay_t<decltype(o)>, std::decay_t<decltype(f)>>> {
                   details::inplace::map(o, f);
                 }};
}
template <typename U> [[nodiscard]] constexpr auto static_as() {
  auto cast = [](auto &&x) { return static_cast<U>(x); };
  using F = decltype(cast);
  return dual_op{[cast](auto &&o) { return map(o, cast, tag{}); },
                 [cast](auto &o) -> std::enable_if_t<details::inplace::can_map_inplace<std::decay_t<decltype(o)>, F>> {
                   details::inplace::map(o, cast);
                 }};
}
template <typename U> [[nodiscard]] constexpr auto reinterpret_as() {
  auto cast = [](auto &&x) { return reinterpret_cast<U>(x); };
  using F = decltype(cast);
  return dual_op{[cast](auto &&o) { return map(o, cast, tag{}); },
                 [cast](auto &o) -> std::enable_if_t<details::inplace::can_map_inplace<std::decay_t<decltype(o)>, F>> {
                   details::inplace::map(o, cast);
                 }};
}
template <typename U> [[nodiscard]] constexpr auto const_as() {
  auto cast = [](auto &&x) { return const_cast<U>(x); };
  using F = decltype(cast);
  return dual_op{[cast](auto &&o) { return map(o, cast, tag{}); },
                 [cast](auto &o) -> std::enable_if_t<details::inplace::can_map_inplace<std::decay_t<decltype(o)>, F>> {
                   details::inplace::map(o, cast);
                 }};
}

// XXX `as_ref` needs `|` to keep the source non-const; under `^` const propagates and you get `reference_wrapper<const T>`
[[nodiscard]] inline constexpr auto as_ref() {
  return [](auto &&o, tag) { return map(o, [](auto &x) { return std::ref(x); }, tag{}); };
}
[[nodiscard]] inline constexpr auto as_cref() {
  return [](auto &&o, tag) { return map(o, [](const auto &x) { return std::cref(x); }, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto filter(P p) {
  return dual_op{[p](auto &&o) { return filter(o, p, tag{}); },
                 [p](auto &o) -> std::enable_if_t<details::inplace::can_filter_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::filter(o, p);
                 }};
}
template <typename F> [[nodiscard]] constexpr auto flat_map(F f) {
  return [f = std::move(f)](auto &&o, tag) { return flat_map(o, f, tag{}); };
}
[[nodiscard]] inline constexpr auto flatten() {
  return [](auto &&o, tag) { return flatten(o, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto collect(F f) {
  return [f = std::move(f)](auto &&o, tag) { return collect(o, f, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto collect_first(F f) {
  return [f = std::move(f)](auto &&o, tag) { return collect_first(o, f, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto append(T t) {
  return dual_op{[t](auto &&o) { return append(o, t, tag{}); },
                 [t](auto &o) -> std::enable_if_t<details::inplace::can_append_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::append(o, t);
                 }};
}
template <typename C> [[nodiscard]] constexpr auto concat(const C &c) {
  return dual_op{[&c](auto &&o) { return concat(o, c, tag{}); },
                 [&c](auto &o) -> std::enable_if_t<details::inplace::can_concat_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::concat(o, c);
                 }};
}
template <typename F> [[nodiscard]] constexpr auto distinct_by(F f) {
  return dual_op{
      [f](auto &&o) { return distinct_by(o, f, tag{}); },
      [f](auto &o) -> std::enable_if_t<details::inplace::can_distinct_inplace<std::decay_t<decltype(o)>,
                                                                              std::decay_t<decltype(details::ap(f, *std::begin(o)))>>> {
        details::inplace::distinct_by(o, f);
      }};
}
[[nodiscard]] inline constexpr auto distinct() {
  return dual_op{
      [](auto &&o) { return distinct(o, tag{}); },
      [](auto &o) -> std::enable_if_t<
                      details::inplace::can_distinct_inplace<std::decay_t<decltype(o)>, typename std::decay_t<decltype(o)>::value_type>> {
        details::inplace::distinct(o);
      }};
}
template <typename P> [[nodiscard]] constexpr auto count(P p) {
  return [p = std::move(p)](auto &&o, tag) { return count(o, p, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto exists(P p) {
  return [p = std::move(p)](auto &&o, tag) { return exists(o, p, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto forall(P p) {
  return [p = std::move(p)](auto &&o, tag) { return forall(o, p, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto find(P p) {
  return [p = std::move(p)](auto &&o, tag) { return find(o, p, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto reduce(F f) {
  return [f = std::move(f)](auto &&o, tag) { return reduce(o, f, tag{}); };
}
[[nodiscard]] inline constexpr auto min() {
  return [](auto &&o, tag) { return min(o, tag{}); };
}
[[nodiscard]] inline constexpr auto max() {
  return [](auto &&o, tag) { return max(o, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto min_by(F f) {
  return [f = std::move(f)](auto &&o, tag) { return min_by(o, f, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto max_by(F f) {
  return [f = std::move(f)](auto &&o, tag) { return max_by(o, f, tag{}); };
}
[[nodiscard]] inline constexpr auto index_of_min() {
  return [](auto &&o, tag) { return index_of_min(o, tag{}); };
}
[[nodiscard]] inline constexpr auto index_of_max() {
  return [](auto &&o, tag) { return index_of_max(o, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto index_of_min_by(F f) {
  return [f = std::move(f)](auto &&o, tag) { return index_of_min_by(o, f, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto index_of_max_by(F f) {
  return [f = std::move(f)](auto &&o, tag) { return index_of_max_by(o, f, tag{}); };
}
[[nodiscard]] inline constexpr auto sum() {
  return [](auto &&o, tag) { return sum(o, tag{}); };
}
[[nodiscard]] inline constexpr auto product() {
  return [](auto &&o, tag) { return product(o, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto sum_by(F f) {
  return [f = std::move(f)](auto &&o, tag) { return sum_by(o, f, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto none_match(P p) {
  return [p = std::move(p)](auto &&o, tag) { return none_match(o, p, tag{}); };
}
template <typename C> [[nodiscard]] constexpr auto intersect(const C &c) {
  return [&c](auto &&o, tag) { return intersect(o, c, tag{}); };
}
template <typename C> [[nodiscard]] constexpr auto diff(const C &c) {
  return [&c](auto &&o, tag) { return diff(o, c, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto tap_each(F f) {
  return dual_op{[f](auto &&o) { return tap_each(o, f, tag{}); }, [f](auto &o) { details::inplace::tap_each(o, f); }};
}
template <typename F> [[nodiscard]] constexpr auto for_each(F f) {
  return [f = std::move(f)](auto &&o, tag) { return for_each(o, f, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto partition(P p) {
  return [p = std::move(p)](auto &&o, tag) { return partition(o, p, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto partition_map(F f) {
  return [f = std::move(f)](auto &&o, tag) { return partition_map(o, f, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto traverse(F f) {
  return [f = std::move(f)](auto &&o, tag) { return traverse(o, f, tag{}); };
}
[[nodiscard]] inline constexpr auto sequence() {
  return [](auto &&o, tag) { return sequence(o, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto intersperse(T sep) {
  return [sep = std::move(sep)](auto &&o, tag) { return intersperse(o, sep, tag{}); };
}
[[nodiscard]] inline constexpr auto combine_all() {
  return [](auto &&o, tag) { return combine_all(o, tag{}); };
}
template <typename G, typename M, typename R> [[nodiscard]] constexpr auto group_map_reduce(G g, M m, R r) {
  return [g = std::move(g), m = std::move(m), r = std::move(r)](auto &&o, tag) { return group_map_reduce(o, g, m, r, tag{}); };
}
template <typename G, typename M> [[nodiscard]] constexpr auto group_map(G g, M m) {
  return [g = std::move(g), m = std::move(m)](auto &&o, tag) { return group_map(o, g, m, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto group_by(F f) {
  return [f = std::move(f)](auto &&o, tag) { return group_by(o, f, tag{}); };
}
[[nodiscard]] inline constexpr auto to_vector() {
  return [](auto &&o, tag) { return to_vector(o, tag{}); };
}
[[nodiscard]] inline constexpr auto to_optional() {
  return [](auto &&o, tag) { return to_optional(o, tag{}); };
}
[[nodiscard]] inline constexpr auto to_optional_error() {
  return [](auto &&o, tag) { return to_optional_error(o, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto map_error(F f) {
  return [f = std::move(f)](auto &&o, tag) { return map_error(o, f, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto tap_error(F f) {
  return [f = std::move(f)](auto &&o, tag) { return tap_error(o, f, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto flat_map_error(F f) {
  return [f = std::move(f)](auto &&o, tag) { return flat_map_error(o, f, tag{}); };
}
template <template <typename...> typename C> [[nodiscard]] constexpr auto to() {
  return [](auto &&o, tag) { return to<C>(o, tag{}); };
}

template <typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto mk_string(std::string prefix, std::string sep, std::string suffix, F f) {
  return [prefix = std::move(prefix), sep = std::move(sep), suffix = std::move(suffix), f = std::move(f)](auto &&o, tag) {
    return mk_string(o, std::string_view{prefix}, std::string_view{sep}, std::string_view{suffix}, f, tag{});
  };
}
[[nodiscard]] inline ASPARTAME_CONSTEXPR_ALLOC auto mk_string(std::string prefix, std::string sep, std::string suffix) {
  return [prefix = std::move(prefix), sep = std::move(sep), suffix = std::move(suffix)](auto &&o, tag) {
    return mk_string(o, std::string_view{prefix}, std::string_view{sep}, std::string_view{suffix}, show_string, tag{});
  };
}
[[nodiscard]] inline ASPARTAME_CONSTEXPR_ALLOC auto mk_string(std::string sep = "") {
  return [sep = std::move(sep)](auto &&o, tag) {
    return mk_string(o, std::string_view{""}, std::string_view{sep}, std::string_view{""}, show_string, tag{});
  };
}
template <typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto mk_string(std::string sep, F f) {
  return [sep = std::move(sep), f = std::move(f)](auto &&o, tag) {
    return mk_string(o, std::string_view{""}, std::string_view{sep}, std::string_view{""}, f, tag{});
  };
}

template <typename T> [[nodiscard]] constexpr auto prepend(T t) {
  return dual_op{[t](auto &&o) { return prepend(o, t, tag{}); },
                 [t](auto &o) -> std::enable_if_t<details::inplace::can_prepend_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::prepend(o, t);
                 }};
}
[[nodiscard]] inline constexpr auto head_maybe() {
  return [](auto &&o, tag) { return head_maybe(o, tag{}); };
}
[[nodiscard]] inline constexpr auto last_maybe() {
  return [](auto &&o, tag) { return last_maybe(o, tag{}); };
}
[[nodiscard]] inline constexpr auto init() {
  return [](auto &&o, tag) { return init(o, tag{}); };
}
[[nodiscard]] inline constexpr auto tail() {
  return [](auto &&o, tag) { return tail(o, tag{}); };
}
[[nodiscard]] inline constexpr auto at_maybe(size_t idx) {
  return [idx](auto &&o, tag) { return at_maybe(o, idx, tag{}); };
}
[[nodiscard]] inline constexpr auto slice(size_t from_inclusive, size_t to_exclusive) {
  return dual_op{
      [from_inclusive, to_exclusive](auto &&o) { return slice(o, from_inclusive, to_exclusive, tag{}); },
      [from_inclusive, to_exclusive](auto &o) -> std::enable_if_t<details::inplace::can_truncate_inplace<std::decay_t<decltype(o)>>> {
        details::inplace::slice(o, from_inclusive, to_exclusive);
      }};
}
template <typename Container> [[nodiscard]] constexpr auto index_of_slice(const Container &other) {
  return [&other](auto &&o, tag) { return index_of_slice(o, other, tag{}); };
}
template <typename Container> [[nodiscard]] constexpr auto index_of_slice_maybe(const Container &other) {
  return [&other](auto &&o, tag) -> std::optional<size_t> {
    auto idx = index_of_slice(o, other, tag{});
    if (idx != -1) return idx;
    else return std::nullopt;
  };
}
template <typename Container> [[nodiscard]] constexpr auto contains_slice(const Container &other) {
  return [&other](auto &&o, tag) { return contains_slice(o, other, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto index_of(T t) {
  return [t = std::move(t)](auto &&o, tag) { return index_of(o, t, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto index_of_maybe(T t) {
  return [t = std::move(t)](auto &&o, tag) -> std::optional<size_t> {
    auto idx = index_of(o, t, tag{});
    if (idx != -1) return idx;
    else return std::nullopt;
  };
}
template <typename T> [[nodiscard]] constexpr auto contains(T t) {
  return [t = std::move(t)](auto &&o, tag) { return contains(o, t, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto find_last(P p) {
  return [p = std::move(p)](auto &&o, tag) { return find_last(o, p, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto index_where(P p) {
  return [p = std::move(p)](auto &&o, tag) { return index_where(o, p, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto index_where_maybe(P p) {
  return [p = std::move(p)](auto &&o, tag) -> std::optional<size_t> {
    auto idx = index_where(o, p, tag{});
    if (idx != -1) return idx;
    else return std::nullopt;
  };
}
template <typename T> [[nodiscard]] constexpr auto last_index_of(T t) {
  return [t = std::move(t)](auto &&o, tag) { return last_index_of(o, t, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto last_index_of_maybe(T t) {
  return [t = std::move(t)](auto &&o, tag) -> std::optional<size_t> {
    auto idx = last_index_of(o, t, tag{});
    if (idx != -1) return idx;
    else return std::nullopt;
  };
}
template <typename P> [[nodiscard]] constexpr auto last_index_where(P p) {
  return [p = std::move(p)](auto &&o, tag) { return last_index_where(o, p, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto last_index_where_maybe(P p) {
  return [p = std::move(p)](auto &&o, tag) -> std::optional<size_t> {
    auto idx = last_index_where(o, p, tag{});
    if (idx != -1) return idx;
    else return std::nullopt;
  };
}
template <typename Acc, typename F> [[nodiscard]] constexpr auto scan_left(Acc init, F f) {
  return [init = std::move(init), f = std::move(f)](auto &&o, tag) { return scan_left(o, init, f, tag{}); };
}
template <typename Acc, typename F> [[nodiscard]] constexpr auto scan_right(Acc init, F f) {
  return [init = std::move(init), f = std::move(f)](auto &&o, tag) { return scan_right(o, init, f, tag{}); };
}
[[nodiscard]] inline constexpr auto unzip() {
  return [](auto &&o, tag) { return unzip(o, tag{}); };
}
template <typename N = size_t> [[nodiscard]] constexpr auto zip_with_index(N from = 0) {
  return [from](auto &&o, tag) { return zip_with_index(o, from, tag{}); };
}
template <typename Container> [[nodiscard]] constexpr auto zip(const Container &other) {
  return [&other](auto &&o, tag) { return zip(o, other, tag{}); };
}
[[nodiscard]] inline constexpr auto transpose() {
  return [](auto &&o, tag) { return transpose(o, tag{}); };
}
[[nodiscard]] inline constexpr auto cartesian_product() {
  return [](auto &&o, tag) { return cartesian_product(o, tag{}); };
}
template <typename C> [[nodiscard]] constexpr auto cross(const C &c) {
  return [&c](auto &&o, tag) { return cross(o, c, tag{}); };
}
[[nodiscard]] inline constexpr auto combinations(size_t k) {
  return [k](auto &&o, tag) { return combinations(o, k, tag{}); };
}
[[nodiscard]] inline constexpr auto permutations() {
  return [](auto &&o, tag) { return permutations(o, tag{}); };
}
[[nodiscard]] inline constexpr auto stride(size_t n) {
  return [n](auto &&o, tag) { return stride(o, n, tag{}); };
}
[[nodiscard]] inline constexpr auto pairwise() {
  return [](auto &&o, tag) { return pairwise(o, tag{}); };
}
template <typename Pred> [[nodiscard]] constexpr auto chunk_by(Pred p) {
  return [p = std::move(p)](auto &&o, tag) { return chunk_by(o, p, tag{}); };
}
template <typename Sep> [[nodiscard]] constexpr auto join_with(Sep sep) {
  return [sep = std::move(sep)](auto &&o, tag) { return join_with(o, sep, tag{}); };
}
[[nodiscard]] inline constexpr auto is_sorted() {
  return [](auto &&o, tag) { return is_sorted(o, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto is_sorted_by(F f) {
  return [f = std::move(f)](auto &&o, tag) { return is_sorted_by(o, f, tag{}); };
}
template <typename URBG> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sample(size_t k, URBG &urbg) {
  return [k, &urbg](auto &&o, tag) { return sample(o, k, urbg, tag{}); };
}
template <typename URBG, std::enable_if_t<!std::is_lvalue_reference_v<URBG>, int> = 0>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sample(size_t k, URBG &&urbg) {
  auto p = std::make_shared<std::decay_t<URBG>>(std::move(urbg));
  return [k, p](auto &&o, tag) { return sample(o, k, *p, tag{}); };
}
[[nodiscard]] inline constexpr auto top_k(size_t k) {
  return [k](auto &&o, tag) { return top_k(o, k, tag{}); };
}
[[nodiscard]] inline constexpr auto bottom_k(size_t k) {
  return [k](auto &&o, tag) { return bottom_k(o, k, tag{}); };
}
template <typename C> [[nodiscard]] constexpr auto symmetric_difference(const C &c) {
  return [&c](auto &&o, tag) { return symmetric_difference(o, c, tag{}); };
}
template <typename N = size_t> [[nodiscard]] constexpr auto enumerate(N from = 0) {
  return [from](auto &&o, tag) { return zip_with_index(o, from, tag{}); };
}
[[nodiscard]] inline constexpr auto reverse() {
  return dual_op{
      [](auto &&o) { return reverse(o, tag{}); },
      [](auto &o) -> std::enable_if_t<details::inplace::can_reverse_inplace<std::decay_t<decltype(o)>>> { details::inplace::reverse(o); }};
}
template <typename URBG> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto shuffle(URBG &urbg) {
  return dual_op{[&urbg](auto &&o) { return shuffle(o, urbg, tag{}); },
                 [&urbg](auto &o) -> std::enable_if_t<details::inplace::can_shuffle_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::shuffle(o, urbg);
                 }};
}

template <typename URBG, std::enable_if_t<!std::is_lvalue_reference_v<URBG>, int> = 0>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto shuffle(URBG &&urbg) {
  auto p = std::make_shared<std::decay_t<URBG>>(std::move(urbg));
  return dual_op{[p](auto &&o) { return shuffle(o, *p, tag{}); },
                 [p](auto &o) -> std::enable_if_t<details::inplace::can_shuffle_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::shuffle(o, *p);
                 }};
}
[[nodiscard]] inline constexpr auto sort() {
  return dual_op{
      [](auto &&o) { return sort(o, tag{}); },
      [](auto &o) -> std::enable_if_t<details::inplace::can_sort_inplace<std::decay_t<decltype(o)>>> { details::inplace::sort(o); }};
}
template <typename Cmp> [[nodiscard]] constexpr auto sort(Cmp cmp) {
  return dual_op{[cmp](auto &&o) { return sort(o, cmp, tag{}); },
                 [cmp](auto &o) -> std::enable_if_t<details::inplace::can_sort_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::sort(o, cmp);
                 }};
}
template <typename Sel> [[nodiscard]] constexpr auto sort_by(Sel sel) {
  return dual_op{[sel](auto &&o) { return sort_by(o, sel, tag{}); },
                 [sel](auto &o) -> std::enable_if_t<details::inplace::can_sort_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::sort_by(o, sel);
                 }};
}
[[nodiscard]] inline constexpr auto split_at(size_t idx) {
  return [idx](auto &&o, tag) { return split_at(o, idx, tag{}); };
}
[[nodiscard]] inline constexpr auto take(size_t n) {
  return dual_op{[n](auto &&o) { return take(o, n, tag{}); },
                 [n](auto &o) -> std::enable_if_t<details::inplace::can_truncate_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::take(o, n);
                 }};
}
[[nodiscard]] inline constexpr auto drop(size_t n) {
  return dual_op{[n](auto &&o) { return drop(o, n, tag{}); },
                 [n](auto &o) -> std::enable_if_t<details::inplace::can_truncate_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::drop(o, n);
                 }};
}
[[nodiscard]] inline constexpr auto take_right(size_t n) {
  return [n](auto &&o, tag) { return take_right(o, n, tag{}); };
}
[[nodiscard]] inline constexpr auto drop_right(size_t n) {
  return [n](auto &&o, tag) { return drop_right(o, n, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto take_while(P p) {
  return dual_op{[p](auto &&o) { return take_while(o, p, tag{}); },
                 [p](auto &o) -> std::enable_if_t<details::inplace::can_truncate_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::take_while(o, p);
                 }};
}
template <typename P> [[nodiscard]] constexpr auto drop_while(P p) {
  return dual_op{[p](auto &&o) { return drop_while(o, p, tag{}); },
                 [p](auto &o) -> std::enable_if_t<details::inplace::can_truncate_inplace<std::decay_t<decltype(o)>>> {
                   details::inplace::drop_while(o, p);
                 }};
}
template <typename P> [[nodiscard]] constexpr auto span(P p) {
  return [p = std::move(p)](auto &&o, tag) { return span(o, p, tag{}); };
}
template <typename Acc, typename F> [[nodiscard]] constexpr auto fold_left(Acc init, F f) {
  return [init = std::move(init), f = std::move(f)](auto &&o, tag) { return fold_left(o, init, f, tag{}); };
}
template <typename Acc, typename F> [[nodiscard]] constexpr auto fold_right(Acc init, F f) {
  return [init = std::move(init), f = std::move(f)](auto &&o, tag) { return fold_right(o, init, f, tag{}); };
}
[[nodiscard]] inline constexpr auto sliding(size_t size, size_t step) {
  return [size, step](auto &&o, tag) { return sliding(o, size, step, tag{}); };
}
[[nodiscard]] inline constexpr auto grouped(size_t size) {
  return [size](auto &&o, tag) { return sliding(o, size, size, tag{}); };
}

template <typename Key, typename F> [[nodiscard]] constexpr auto get_or_emplace(Key key, F f) {
  return inplace_op{[key = std::move(key), f = std::move(f)](auto &o) { (void)get_or_emplace(o, key, f, tag{}); }};
}
[[nodiscard]] inline constexpr auto keys() {
  return [](auto &&o, tag) { return keys(o, tag{}); };
}
[[nodiscard]] inline constexpr auto values() {
  return [](auto &&o, tag) { return values(o, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto map_keys(F f) {
  return [f = std::move(f)](auto &&o, tag) { return map_keys(o, f, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto map_values(F f) {
  return dual_op{
      [f](auto &&o) { return map_values(o, f, tag{}); },
      [f](auto &o) -> std::enable_if_t<details::inplace::can_map_values_inplace<std::decay_t<decltype(o)>, std::decay_t<decltype(f)>>> {
        details::inplace::map_values(o, f);
      }};
}
template <typename P> [[nodiscard]] constexpr auto filter_keys(P p) {
  return [p = std::move(p)](auto &&o, tag) { return filter_keys(o, p, tag{}); };
}
template <typename P> [[nodiscard]] constexpr auto filter_values(P p) {
  return [p = std::move(p)](auto &&o, tag) { return filter_values(o, p, tag{}); };
}
template <typename C, typename F> [[nodiscard]] constexpr auto combine(const C &c, F f) {
  return [&c, f = std::move(f)](auto &&o, tag) { return combine(o, c, f, tag{}); };
}
template <typename Key> [[nodiscard]] constexpr auto get_maybe(Key key) {
  return [key = std::move(key)](auto &&o, tag) { return get_maybe(o, key, tag{}); };
}
template <typename K, typename V> [[nodiscard]] constexpr auto get_or_default(K key, V default_value) {
  return [key = std::move(key), default_value = std::move(default_value)](auto &&o, tag) {
    return get_or_default(o, key, default_value, tag{});
  };
}

template <typename T> [[nodiscard]] constexpr auto get_maybe() {
  return [](auto &&o, tag) { return get_maybe<T>(o, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto holds() {
  return [](auto &&o, tag) { return holds<T>(o, tag{}); };
}
template <typename... T> [[nodiscard]] constexpr auto holds_any() {
  return [](auto &&o, tag) { return holds_any<T...>(o, tag{}); };
}
template <typename... T> [[nodiscard]] constexpr auto narrow() {
  return [](auto &&o, tag) { return narrow<T...>(o, tag{}); };
}
template <typename... Fs> [[nodiscard]] constexpr auto fold_total(Fs... fs) {
  return [fs...](auto &&o, tag) { return fold_total(tag{}, o, fs...); };
}
template <typename... Fs> [[nodiscard]] constexpr auto fold_partial(Fs... fs) {
  return [fs...](auto &&o, tag) { return fold_partial(tag{}, o, fs...); };
}
template <typename... Fs> [[nodiscard]] constexpr auto foreach_total(Fs... fs) {
  return [fs...](auto &&o, tag) { return foreach_total(tag{}, o, fs...); };
}
template <typename... Fs> [[nodiscard]] constexpr auto foreach_partial(Fs... fs) {
  return [fs...](auto &&o, tag) { return foreach_partial(tag{}, o, fs...); };
}

template <typename F, typename FE> [[nodiscard]] constexpr auto fold(F f, FE empty) {
  return [f = std::move(f), empty = std::move(empty)](auto &&o, tag) { return fold(o, f, empty, tag{}); };
}
template <typename FE> [[nodiscard]] constexpr auto fold(FE empty) {
  return [empty = std::move(empty)](auto &&o, tag) { return fold(o, empty, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto get_or_else(T default_value) {
  return [default_value = std::move(default_value)](auto &&o, tag) { return get_or_else(o, default_value, tag{}); };
}
[[nodiscard]] inline constexpr auto get_or_default() {
  return [](auto &&o, tag) { return get_or_default(o, tag{}); };
}
template <typename T> [[nodiscard]] constexpr auto or_else_maybe(T that) {
  return [that = std::move(that)](auto &&o, tag) { return or_else_maybe(o, that, tag{}); };
}
template <typename F> [[nodiscard]] constexpr auto or_else(F f) {
  return [f = std::move(f)](auto &&o, tag) { return or_else(o, f, tag{}); };
}

#define ASPARTAME_STRING_OP_REQUIRES(name)                                                                                                 \
  static_assert(enable_string_ops<std::decay_t<decltype(o)>>::value, #name " requires a std::basic_string receiver")

[[nodiscard]] inline constexpr auto trim_leading() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(trim_leading);
    return trim_leading(o, tag{});
  };
}
[[nodiscard]] inline constexpr auto trim_trailing() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(trim_trailing);
    return trim_trailing(o, tag{});
  };
}
[[nodiscard]] inline constexpr auto trim() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(trim);
    return trim(o, tag{});
  };
}
[[nodiscard]] inline constexpr auto is_blank() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(is_blank);
    return is_blank(o, tag{});
  };
}
[[nodiscard]] inline constexpr auto to_upper() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(to_upper);
    return to_upper(o, tag{});
  };
}
[[nodiscard]] inline constexpr auto to_lower() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(to_lower);
    return to_lower(o, tag{});
  };
}
template <typename Needle, typename With> [[nodiscard]] constexpr auto replace_all(Needle needle, With with) {
  return [needle = std::move(needle), with = std::move(with)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(replace_all);
    return replace_all(o, needle, with, tag{});
  };
}
[[nodiscard]] inline constexpr auto indent(int n) {
  return [n](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(indent);
    return indent(o, n, std::decay_t<decltype(o)>{'\n'}, tag{});
  };
}
template <typename NewLine> [[nodiscard]] constexpr auto indent(int n, NewLine newLine) {
  return [n, newLine = std::move(newLine)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(indent);
    return indent(o, n, newLine, tag{});
  };
}
template <typename String> [[nodiscard]] constexpr auto starts_with(String s) {
  return [s = std::move(s)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(starts_with);
    return starts_with(o, s, tag{});
  };
}
template <typename String> [[nodiscard]] constexpr auto ends_with(String s) {
  return [s = std::move(s)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(ends_with);
    return ends_with(o, s, tag{});
  };
}
template <typename String> [[nodiscard]] constexpr auto contains_ignore_case(String s) {
  return [s = std::move(s)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(contains_ignore_case);
    return contains_ignore_case(o, s, tag{});
  };
}
template <typename String> [[nodiscard]] constexpr auto equals_ignore_case(String s) {
  return [s = std::move(s)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(equals_ignore_case);
    return equals_ignore_case(o, s, tag{});
  };
}
template <typename Delim> [[nodiscard]] constexpr auto split(Delim d) {
  return [d = std::move(d)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(split);
    return split(o, d, tag{});
  };
}
[[nodiscard]] inline constexpr auto lines() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(lines);
    return lines(o, tag{});
  };
}
template <typename String> [[nodiscard]] constexpr auto strip_prefix(String s) {
  return [s = std::move(s)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(strip_prefix);
    return strip_prefix(o, s, tag{});
  };
}
template <typename String> [[nodiscard]] constexpr auto strip_suffix(String s) {
  return [s = std::move(s)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(strip_suffix);
    return strip_suffix(o, s, tag{});
  };
}
[[nodiscard]] inline constexpr auto capitalize() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(capitalize);
    return capitalize(o, tag{});
  };
}
[[nodiscard]] inline constexpr auto uncapitalize() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(uncapitalize);
    return uncapitalize(o, tag{});
  };
}
template <typename Needle, typename With> [[nodiscard]] constexpr auto replace_first(Needle needle, With with) {
  return [needle = std::move(needle), with = std::move(with)](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(replace_first);
    return replace_first(o, needle, with, tag{});
  };
}
[[nodiscard]] inline constexpr auto repeated(size_t n) {
  return [n](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(repeated);
    return repeated(o, n, tag{});
  };
}
[[nodiscard]] inline constexpr auto pad_left(size_t width, char fill = ' ') {
  return [width, fill](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(pad_left);
    return pad_left(o, width, fill, tag{});
  };
}
[[nodiscard]] inline constexpr auto pad_right(size_t width, char fill = ' ') {
  return [width, fill](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(pad_right);
    return pad_right(o, width, fill, tag{});
  };
}
[[nodiscard]] inline constexpr auto words() {
  return [](auto &&o, tag) {
    ASPARTAME_STRING_OP_REQUIRES(words);
    return words(o, tag{});
  };
}

#undef ASPARTAME_STRING_OP_REQUIRES

template <typename F> [[nodiscard]] constexpr auto and_then(F f) {
  return [f = std::move(f)](auto &&o, tag) {
    if constexpr (details::assert_non_void<decltype(details::ap(f, o))>()) {}
    return details::ap(f, o);
  };
}
template <typename F> [[nodiscard]] constexpr auto tap(F f) {
  return [f = std::move(f)](auto &&o, tag) {
    if constexpr (details::assert_void<decltype(details::ap(f, o))>()) {}
    details::ap(f, o);
  };
}

template <typename F> [[nodiscard]] auto attempt(F &&f) -> std::optional<std::decay_t<decltype(f())>> {
  static_assert(!std::is_void_v<decltype(f())>, "attempt: callable must return a value");
  try {
    return std::forward<F>(f)();
  } catch (...) { return std::nullopt; }
}

template <typename P> [[nodiscard]] constexpr auto ensure(P p) {
  return [p = std::move(p)](auto &&o, tag) { return ensure(o, p, tag{}); };
}
template <typename P, typename EFn> [[nodiscard]] constexpr auto ensure(P p, EFn efn) {
  return [p = std::move(p), efn = std::move(efn)](auto &&o, tag) { return ensure(o, p, efn, tag{}); };
}
template <typename ValueFn, typename ErrorFn> [[nodiscard]] constexpr auto bimap(ValueFn vfn, ErrorFn efn) {
  return [vfn = std::move(vfn), efn = std::move(efn)](auto &&o, tag) { return bimap(o, vfn, efn, tag{}); };
}

} // namespace aspartame
