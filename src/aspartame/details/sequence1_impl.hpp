#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "base.hpp"

namespace aspartame::details::sequence1 {

template <typename In, typename T, typename Out> //
[[nodiscard]] constexpr Out prepend(const In &in, const T &t) {
  if constexpr (has_reserve<Out> && has_size<In>) {
    Out ys;
    ys.reserve(in.size() + 1);
    push(ys, t);
    for (auto &&x : seq_view(in))
      push(ys, x);
    return ys;
  } else {
    auto ys = Out{seq_view(in).begin(), seq_view(in).end()};
    ys.insert(ys.begin(), t);
    return ys;
  }
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> head_maybe(const In &in) {
  return !in.empty() ? std::optional<T>{*seq_view(in).begin()} : std::nullopt;
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> last_maybe(const In &in) {
  if constexpr (std::is_base_of_v<std::bidirectional_iterator_tag,
                                  typename std::iterator_traits<typename In::const_iterator>::iterator_category>) {
    return !in.empty() ? std::optional<T>{*(std::prev(seq_view(in).end()))} : std::nullopt;

  } else {
    auto it = seq_view(in).begin(), end = seq_view(in).end();
    while (it != end) {
      T x = *it;
      if ((++it) == end) return std::optional<T>{x};
    }
    return std::nullopt;
  }
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out init(const In &in) {
  return in.empty() ? Out{} : Out{seq_view(in).begin(), std::prev(seq_view(in).end())};
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out tail(const In &in) {
  return in.empty() ? Out{} : Out{std::next(seq_view(in).begin()), seq_view(in).end()};
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> at_maybe(const In &in, size_t idx) {
  if constexpr (has_size<In>) {
    if (idx >= static_cast<size_t>(in.size())) return std::nullopt;
    auto it = seq_view(in).begin();
    std::advance(it, idx);
    return *it;
  } else {
    size_t i = 0;
    auto it = seq_view(in).begin();
    for (; it != seq_view(in).end() && i < idx; ++it, ++i) {}
    return (it == seq_view(in).end() || i != idx) ? std::nullopt : std::optional{*it};
  }
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out slice(const In &in, size_t from_inclusive, size_t to_exclusive) {
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(to_exclusive > from_inclusive ? to_exclusive - from_inclusive : 0);
  if (from_inclusive >= static_cast<size_t>(in.size()) || from_inclusive > to_exclusive) return ys;
  if (to_exclusive > static_cast<size_t>(in.size())) to_exclusive = static_cast<size_t>(in.size());
  if constexpr (std::is_same<In, std::string>::value) return in.substr(from_inclusive, to_exclusive - from_inclusive);
  else {
    auto from_it = seq_view(in).begin();
    std::advance(from_it, from_inclusive);
    auto to_it = seq_view(in).begin();
    std::advance(to_it, to_exclusive);
    std::copy(from_it, to_it, std::back_inserter(ys));
    return ys;
  }
}

template <typename In, typename T> //
[[nodiscard]] constexpr std::make_signed_t<size_t> index_of(const In &in, const T &t) {
  static_assert(std::is_convertible_v<std::decay_t<T>, typename In::value_type>, "type does not match vector's value type");
  size_t i = 0;

  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
    auto x = *it;
    if (x == t) return static_cast<std::make_signed_t<size_t>>(i);
    i++;
  }
  return -1;
}

template <typename In, typename Container> //
[[nodiscard]] constexpr std::make_signed_t<size_t> index_of_slice(const In &in, const Container &other) {
  static_assert(std::is_convertible_v<std::decay_t<typename Container::value_type>, typename In::value_type>,
                "type does not match vector's value type");
  auto first = seq_view(in).begin(), last = seq_view(in).end();
  std::make_signed_t<size_t> i = 0;
  while (true) {
    auto it = first;
    for (auto s_it = other.begin();; ++it, ++s_it) {
      if (s_it == other.end()) return i;
      if (it == last) return -1;
      if (!(*it == *s_it)) break;
    }
    ++i;
    ++first;
  }
  return -1;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr std::make_signed_t<size_t> index_where(const In &in, Predicate &&p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  size_t i = 0;
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
    if (details::ap(p, *it)) return static_cast<std::make_signed_t<size_t>>(i);
    i++;
  }
  return -1;
}

template <typename In, typename Function> //
[[nodiscard]] constexpr auto collect_first(const In &in, Function f) {
  using T = decltype(details::ap(f, *seq_view(in).begin()));
  static_assert(is_optional<T>, "collect function should return an optional");
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
    if (auto y = details::ap(f, *it); y) return std::optional<typename T::value_type>{*y};
  }
  return std::optional<typename T::value_type>{};
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto find_last(const In &in, Predicate p) {
  using T = typename In::value_type;
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  if constexpr (has_rbegin<In> && has_rend<In>) {
    auto it = std::find_if(in.rbegin(), in.rend(), [&](auto &&x) { return details::ap(p, x); });
    if (it == in.rend()) return std::optional<T>{std::nullopt};
    else return std::optional<T>{*it};
  } else {
    std::optional<T> found;
    for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
      if (details::ap(p, *it)) found = *it;
    }
    return found;
  }
}

template <typename In, typename Out, typename N> //
[[nodiscard]] constexpr auto zip_with_index(const In &in, N from) {
  using T = typename In::value_type;
  if constexpr (std::is_default_constructible_v<T> && has_size<In>) {
    Out ys(in.size());
    auto it = ys.begin();
    N i = from;
    for (auto &&x : seq_view(in)) {
      *it++ = {x, i};
      i++;
    }
    return ys;
  } else {
    Out ys;
    if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size());
    N i = from;
    for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it) {
      auto x = *it;
      ys.emplace_back(x, i);
      i++;
    }
    return ys;
  }
}

template <typename In, typename Container, typename Out> //
[[nodiscard]] constexpr auto zip(const In &in, const Container &c) {
  size_t min_size = std::min(in.size(), c.size());
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(min_size);
  auto it_o = seq_view(in).begin();
  auto it_c = c.begin();
  for (size_t i = 0; i < min_size; ++i, ++it_o, ++it_c)
    ys.emplace_back(*it_o, *it_c);

  return ys;
}

template <typename In, typename Inner, typename Outer> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto transpose(const In &in) {
  static_assert(is_iterable<typename In::value_type>, "not a nested type that is iterable");
  using Inr = typename In::value_type;

  if (in.empty()) return Outer();

  const size_t cols = static_cast<size_t>(seq_view(in).begin()->size());
  size_t idx = 1;
  for (auto it = std::next(seq_view(in).begin()); it != seq_view(in).end(); ++it, ++idx) {
    if (static_cast<size_t>(it->size()) != cols)
      details::raise<std::length_error>("cannot transpose a non-square (i.e all nested contains must share the same size) nested container"
                                        ": container at [" +
                                        std::to_string(idx) + "] has size of " + std::to_string(it->size()) + " but [0] is " +
                                        std::to_string(cols));
  }

  Outer ys;
  if constexpr (has_reserve<Outer>) ys.reserve(cols);

  std::vector<typename Inr::const_iterator> iterators;
  for (const Inr &x : seq_view(in))
    iterators.push_back(x.begin());

  const size_t rows = static_cast<size_t>(in.size());
  for (size_t j = 0; j < cols; ++j) {
    Inner row;
    if constexpr (has_reserve<Inner>) row.reserve(rows);
    for (size_t i = 0; i < rows; ++i) {
      row.push_back(*(iterators[i]));
      if (j < cols - 1) ++(iterators[i]);
    }
    ys.push_back(std::move(row));
  }
  return ys;
}

template <typename In, typename Inner, typename Outer> //
[[nodiscard]] constexpr auto cartesian_product(const In &in) {
  static_assert(is_iterable<typename In::value_type>, "not a nested type that is iterable");
  Outer s = {Inner{}};
  for (const auto &u : seq_view(in)) {
    Outer r;
    for (const auto &x : s) {
      for (const auto &y : u) {
        r.push_back(x);
        r.back().push_back(y);
      }
    }
    s = std::move(r);
  }
  return s;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out reverse(const In &in) {
  if constexpr (has_rbegin<In> && has_rend<In>) {
    return Out{in.rbegin(), in.rend()};
  } else {
    Out ys{seq_view(in).begin(), seq_view(in).end()};
    if constexpr (has_reverse<In>) ys.reverse();
    else std::reverse(ys.begin(), ys.end());
    return ys;
  }
}

template <typename In, typename URBG, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out shuffle(const In &in, URBG &&urbg) {
  if constexpr (std::is_same_v<typename std::iterator_traits<typename In::iterator>::iterator_category, std::random_access_iterator_tag>) {
    Out ys{seq_view(in).begin(), seq_view(in).end()};
    std::shuffle(ys.begin(), ys.end(), std::forward<URBG &&>(urbg));
    return ys;
  } else {
    std::vector<std::reference_wrapper<const typename In::value_type>> refs{in.cbegin(), in.cend()};
    std::shuffle(refs.begin(), refs.end(), std::forward<URBG>(urbg));
    return Out{refs.begin(), refs.end()};
  }
}

template <typename In, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out sort(const In &in) {
  Out ys{seq_view(in).begin(), seq_view(in).end()};
  if constexpr (has_sort<In>) ys.sort();
  else std::sort(ys.begin(), ys.end());
  return ys;
}

template <typename In, typename Compare, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out sort(const In &in, Compare &&c) {
  Out ys{seq_view(in).begin(), seq_view(in).end()};
  if constexpr (has_sort<In>) ys.sort(c);
  else std::sort(ys.begin(), ys.end(), c);
  return ys;
}

template <typename In, typename Select, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out sort_by(const In &in, Select &&s) {
  Out ys{seq_view(in).begin(), seq_view(in).end()};
  const auto compare = [&](const auto &l, const auto &r) { return details::ap(s, l) < details::ap(s, r); };
  if constexpr (has_sort<In>) ys.sort(compare);
  else std::sort(ys.begin(), ys.end(), compare);
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr std::pair<Out, Out> split_at(const In &in, size_t idx) {
  const size_t size = static_cast<size_t>(in.size());
  if (size == 0) return {Out{}, Out{}};
  if (idx > size) return {Out{seq_view(in).begin(), seq_view(in).end()}, Out{}};

  Out l;
  Out r;
  if constexpr (has_reserve<Out>) {
    l.reserve(idx);
    r.reserve(size - idx);
  }

  auto first_end = seq_view(in).begin();
  std::advance(first_end, idx);
  std::copy(seq_view(in).begin(), first_end, std::back_inserter(l));
  std::copy(first_end, seq_view(in).end(), std::back_inserter(r));
  return {l, r};
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out take(const In &in, size_t n) {
  if (n >= static_cast<size_t>(in.size())) return Out{seq_view(in).begin(), seq_view(in).end()};
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(n);
  std::copy_n(seq_view(in).begin(), n, std::back_inserter(ys));
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out drop(const In &in, size_t n) {
  const size_t in_size = static_cast<size_t>(in.size());
  if (n >= in_size) return Out{};
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(in_size - n);
  auto it = seq_view(in).begin();
  std::advance(it, n);
  std::copy(it, seq_view(in).end(), std::back_inserter(ys));
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out take_right(const In &in, size_t n) {
  static_assert(
      std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<typename In::const_iterator>::iterator_category>,
      "iterator does not meet a minimum of the BidirectionalIterator requirement");
  if (n >= static_cast<size_t>(in.size())) return Out{seq_view(in).begin(), seq_view(in).end()};
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(n);
  auto it = seq_view(in).end();
  std::advance(it, -static_cast<std::make_signed_t<size_t>>(n));
  std::copy(it, seq_view(in).end(), std::back_inserter(ys));
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out drop_right(const In &in, size_t n) {
  static_assert(
      std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<typename In::const_iterator>::iterator_category>,
      "iterator does not meet a minimum of the BidirectionalIterator requirement");
  const size_t in_size = static_cast<size_t>(in.size());
  if (n >= in_size) return Out{};
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(in_size - n);
  auto it = seq_view(in).end();
  std::advance(it, -static_cast<std::make_signed_t<size_t>>(n));
  std::copy(seq_view(in).begin(), it, std::back_inserter(ys));
  return ys;
}

template <typename In, typename Predicate, typename Out> //
[[nodiscard]] constexpr Out take_while(const In &in, Predicate &&p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  Out ys;
  auto it = seq_view(in).begin();
  while (it != seq_view(in).end() && details::ap(p, *it))
    ++it;
  if constexpr (has_reserve<Out>) ys.reserve(std::distance(seq_view(in).begin(), it));
  std::copy(seq_view(in).begin(), it, std::back_inserter(ys));
  return ys;
}

template <typename In, typename Predicate, typename Out> //
[[nodiscard]] constexpr Out drop_while(const In &in, Predicate &&p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  Out ys;
  auto it = seq_view(in).begin();
  while (it != seq_view(in).end() && details::ap(p, *it))
    ++it;
  if constexpr (has_reserve<Out>) ys.reserve(std::distance(it, seq_view(in).end()));
  std::copy(it, seq_view(in).end(), std::back_inserter(ys));
  return ys;
}

template <typename In, typename Predicate, typename Out> //
[[nodiscard]] constexpr std::pair<Out, Out> span(const In &in, Predicate &&p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  Out take, drop;
  auto it = seq_view(in).begin();
  while (it != seq_view(in).end() && details::ap(p, *it))
    ++it;
  if constexpr (has_reserve<Out>) {
    take.reserve(std::distance(seq_view(in).begin(), it));
    drop.reserve(std::distance(it, seq_view(in).end()));
  }
  std::copy(seq_view(in).begin(), it, std::back_inserter(take));
  std::copy(it, seq_view(in).end(), std::back_inserter(drop));
  return std::pair{take, drop};
}

template <typename In, typename Accumulator, typename Function>
[[nodiscard]] constexpr auto fold_left(const In &in, Accumulator &&init, Function &&function) {
  static_assert(std::is_invocable_v<Function, Accumulator, decltype(*seq_view(in).begin())>,
                "function must be invocable with with accumulator and container's value type (in this exact order)");
  for (const auto &element : seq_view(in))
    init = function(init, element);
  return init;
}

template <typename In, typename Accumulator, typename Function>
[[nodiscard]] constexpr auto fold_right(const In &in, Accumulator &&init, Function &&function) {
  static_assert(std::is_invocable_v<Function, decltype(*std::rbegin(in)), Accumulator>,
                "function must be invocable with container's value type and accumulator (in this exact order)");
  for (auto it = in.rbegin(); it != in.rend(); ++it)
    init = function(*it, init);
  return init;
}

template <typename In, typename Inner, typename Outer> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sliding(const In &in, size_t size, size_t step) {
  if (size == 0 || step == 0)
    details::raise<std::range_error>("cannot apply windowing with zero size or step, size=" + std::to_string(size) +
                                     " step=" + std::to_string(step));
  if (in.empty()) return Outer{};
  if (static_cast<size_t>(in.size()) <= size) return Outer{Inner{seq_view(in).begin(), seq_view(in).end()}};

  Outer ys;
  auto it = seq_view(in).begin();
  while (it != seq_view(in).end()) {
    auto window_size = std::min(size, static_cast<size_t>(std::distance(it, seq_view(in).end())));
    if (window_size < size && step == 1) break;
    Inner window;
    if constexpr (has_reserve<Inner>) window.reserve(window_size);
    std::copy_n(it, window_size, std::back_inserter(window));
    ys.push_back(std::move(window));
    if (std::distance(it, seq_view(in).end()) <= static_cast<typename std::iterator_traits<decltype(it)>::difference_type>(step)) break;
    std::advance(it, step);
  }
  return ys;
}

template <typename In, typename Acc, typename Function, typename Out> //
[[nodiscard]] constexpr Out scan_left(const In &in, Acc init, Function f) {
  Out ys;
  if constexpr (has_reserve<Out> && has_size<In>) ys.reserve(in.size() + 1);
  Acc acc = std::move(init);
  push(ys, acc);
  for (auto &&x : seq_view(in)) {
    acc = f(acc, x);
    push(ys, acc);
  }
  return ys;
}

template <typename In, typename Acc, typename Function, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out scan_right(const In &in, Acc init, Function f) {
  std::vector<Acc> tmp;
  if constexpr (has_size<In>) tmp.reserve(in.size() + 1);
  Acc acc = std::move(init);
  tmp.push_back(acc);
  for (auto it = in.rbegin(); it != in.rend(); ++it) {
    acc = f(*it, acc);
    tmp.push_back(acc);
  }
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(tmp.size());
  for (auto it = tmp.rbegin(); it != tmp.rend(); ++it)
    push(ys, *it);
  return ys;
}

template <typename In, typename OutA, typename OutB> //
[[nodiscard]] constexpr std::pair<OutA, OutB> unzip(const In &in) {
  static_assert(is_pair_like_v<typename In::value_type>, "unzip requires a sequence of pair-like elements");
  OutA as;
  OutB bs;
  if constexpr (has_reserve<OutA>) as.reserve(in.size());
  if constexpr (has_reserve<OutB>) bs.reserve(in.size());
  for (auto &&x : seq_view(in)) {
    push(as, x.first);
    push(bs, x.second);
  }
  return {as, bs};
}

template <typename In, typename T> //
[[nodiscard]] constexpr std::make_signed_t<size_t> last_index_of(const In &in, const T &t) {
  static_assert(std::is_convertible_v<std::decay_t<T>, typename In::value_type>, "type does not match the container's value type");
  std::make_signed_t<size_t> found = -1;
  size_t i = 0;
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it, ++i)
    if (*it == t) found = static_cast<std::make_signed_t<size_t>>(i);
  return found;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr std::make_signed_t<size_t> last_index_where(const In &in, Predicate &&p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *seq_view(in).begin()))>()) {}
  std::make_signed_t<size_t> found = -1;
  size_t i = 0;
  for (auto it = seq_view(in).begin(), end = seq_view(in).end(); it != end; ++it, ++i)
    if (details::ap(p, *it)) found = static_cast<std::make_signed_t<size_t>>(i);
  return found;
}

template <typename In, typename Other, typename Out> //
[[nodiscard]] constexpr Out cross(const In &in, const Other &other) {
  Out ys;
  if constexpr (has_reserve<Out>) {
    if constexpr (has_size<Other>) ys.reserve(in.size() * other.size());
    else ys.reserve(in.size());
  }
  for (auto &&x : seq_view(in))
    for (auto &&y : other)
      ys.emplace_back(x, y);
  return ys;
}

template <typename In, typename Inner, typename Outer> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Outer combinations(const In &in, size_t k) {
  Outer out;
  std::vector<typename In::value_type> src(seq_view(in).begin(), seq_view(in).end());
  size_t n = src.size();
  if (k > n) return out;
  if (k == 0) {
    Inner empty;
    out.push_back(std::move(empty));
    return out;
  }
  std::vector<size_t> idx(k);
  std::iota(idx.begin(), idx.end(), size_t{0});
  while (true) {
    Inner one;
    if constexpr (has_reserve<Inner>) one.reserve(k);
    for (size_t i : idx)
      push(one, src[i]);
    out.push_back(std::move(one));
    size_t i = k;
    while (i > 0 && idx[i - 1] == n - k + (i - 1))
      --i;
    if (i == 0) break;
    ++idx[i - 1];
    for (size_t j = i; j < k; ++j)
      idx[j] = idx[j - 1] + 1;
  }
  return out;
}

template <typename In, typename Inner, typename Outer> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Outer permutations(const In &in) {
  Outer out;
  std::vector<typename In::value_type> src(seq_view(in).begin(), seq_view(in).end());
  size_t n = src.size();
  if (n == 0) {
    Inner empty;
    out.push_back(std::move(empty));
    return out;
  }
  std::vector<size_t> idx(n);
  std::iota(idx.begin(), idx.end(), size_t{0});
  do {
    Inner one;
    if constexpr (has_reserve<Inner>) one.reserve(n);
    for (size_t i : idx)
      push(one, src[i]);
    out.push_back(std::move(one));
  } while (std::next_permutation(idx.begin(), idx.end()));
  return out;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out stride(const In &in, size_t n) {
  Out ys;
  if (n == 0) return ys;
  if constexpr (has_reserve<Out>)
    if constexpr (has_size<In>) ys.reserve((in.size() + n - 1) / n);
  size_t i = 0;
  for (auto &&x : seq_view(in)) {
    if (i % n == 0) push(ys, x);
    ++i;
  }
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out pairwise(const In &in) {
  Out ys;
  if constexpr (has_size<In>) {
    if (in.size() < 2) return ys;
    if constexpr (has_reserve<Out>) ys.reserve(in.size() - 1);
  }
  auto prev = seq_view(in).begin();
  auto end = seq_view(in).end();
  if (prev == end) return ys;
  auto it = prev;
  ++it;
  for (; it != end; ++it, ++prev)
    ys.emplace_back(*prev, *it);
  return ys;
}

template <typename In, typename Inner, typename Outer, typename Predicate> //
[[nodiscard]] constexpr Outer chunk_by(const In &in, Predicate p) {
  Outer out;
  if (in.empty()) return out;
  Inner current;
  auto it = seq_view(in).begin();
  auto end = seq_view(in).end();
  push(current, *it);
  auto prev_val = *it;
  ++it;
  for (; it != end; ++it) {
    if (p(prev_val, *it)) push(current, *it);
    else {
      out.push_back(std::move(current));
      current = Inner{};
      push(current, *it);
    }
    prev_val = *it;
  }
  out.push_back(std::move(current));
  return out;
}

template <typename In, typename Sep, typename Out> //
[[nodiscard]] constexpr Out join_with(const In &in, const Sep &sep) {
  Out ys;
  bool first = true;
  for (auto &&inner : seq_view(in)) {
    if (!first) {
      if constexpr (is_iterable<Sep>) {
        for (auto &&s : sep)
          push(ys, s);
      } else {
        push(ys, sep);
      }
    }
    for (auto &&x : inner)
      push(ys, x);
    first = false;
  }
  return ys;
}

template <typename In> //
[[nodiscard]] constexpr bool is_sorted(const In &in) {
  return std::is_sorted(seq_view(in).begin(), seq_view(in).end());
}

template <typename In, typename Function> //
[[nodiscard]] constexpr bool is_sorted_by(const In &in, Function f) {
  return std::is_sorted(seq_view(in).begin(), seq_view(in).end(),
                        [&](auto &&a, auto &&b) { return details::ap(f, a) < details::ap(f, b); });
}

template <typename In, typename URBG, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out sample(const In &in, size_t k, URBG &&urbg) {
  Out ys;
  if constexpr (has_reserve<Out>)
    if constexpr (has_size<In>) ys.reserve(std::min(k, in.size()));
  std::sample(seq_view(in).begin(), seq_view(in).end(), std::back_inserter(ys), k, std::forward<URBG>(urbg));
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out bottom_k(const In &in, size_t k) {
  std::vector<typename In::value_type> tmp(seq_view(in).begin(), seq_view(in).end());
  k = std::min(k, tmp.size());
  std::partial_sort(tmp.begin(), tmp.begin() + k, tmp.end());
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(k);
  for (size_t i = 0; i < k; ++i)
    push(ys, tmp[i]);
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC Out top_k(const In &in, size_t k) {
  std::vector<typename In::value_type> tmp(seq_view(in).begin(), seq_view(in).end());
  k = std::min(k, tmp.size());
  std::partial_sort(tmp.begin(), tmp.begin() + k, tmp.end(), std::greater<>());
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(k);
  for (size_t i = 0; i < k; ++i)
    push(ys, tmp[i]);
  return ys;
}

} // namespace aspartame::details::sequence1
