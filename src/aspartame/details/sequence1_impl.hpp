#pragma once

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "base.hpp"

namespace aspartame::details::sequence1 {

template <typename In, typename T, typename Out> //
[[nodiscard]] constexpr Out prepend(const In &in, const T &t) {
  auto ys = Out{in.begin(), in.end()};
  ys.insert(ys.begin(), t);
  return ys;
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> head_maybe(const In &in) {
  return !in.empty() ? std::optional<T>{*in.begin()} : std::nullopt;
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> last_maybe(const In &in) {
  if constexpr (std::is_same_v<std::bidirectional_iterator_tag, typename In::const_iterator::iterator_category> ||
                std::is_same_v<std::random_access_iterator_tag, typename In::const_iterator::iterator_category>
#if __cplusplus >= 202002L
                || std::is_same_v<std::contiguous_iterator_tag, typename In::const_iterator::iterator_category>
#endif
  ) {
    return !in.empty() ? std::optional<T>{*(std::prev(in.end()))} : std::nullopt;

  } else {
    auto it = in.begin(), end = in.end();
    while (it != end) {
      T x = *it;
      if ((++it) == end) return std::optional<T>{x};
    }
    return std::nullopt;
  }
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out init(const In &in) {
  return in.empty() ? Out(in) : Out{in.begin(), std::prev(in.end())};
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out tail(const In &in) {
  return in.empty() ? Out(in) : Out{std::next(in.begin()), in.end()};
}

template <typename In, typename T = typename In::value_type> //
[[nodiscard]] constexpr std::optional<T> at_maybe(const In &in, size_t idx) {
  if constexpr (has_size<In>) {
    if (idx >= in.size()) return std::nullopt;
    auto it = in.begin();
    std::advance(it, idx);
    return *it;
  } else {
    size_t i = 0;
    auto it = in.begin();
    for (; it != in.end() && i < idx; ++it, ++i) {}
    return (it == in.end() || i != idx) ? std::nullopt : std::optional{*it};
  }
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out slice(const In &in, size_t from_inclusive, size_t to_exclusive) {
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(to_exclusive > from_inclusive ? to_exclusive - from_inclusive : 0);
  if (from_inclusive >= in.size() || from_inclusive > to_exclusive) return ys;
  if (to_exclusive > in.size()) to_exclusive = in.size();
  if constexpr (std::is_same<In, std::string>::value) return in.substr(from_inclusive, to_exclusive - from_inclusive);
  else {
    auto from_it = in.begin();
    std::advance(from_it, from_inclusive);
    auto to_it = in.begin();
    std::advance(to_it, to_exclusive);
    std::copy(from_it, to_it, std::back_inserter(ys));
    return ys;
  }
}

template <typename In, typename T> //
[[nodiscard]] constexpr std::make_signed_t<size_t> index_of(const In &in, const T &t) {
  static_assert(std::is_convertible_v<std::decay_t<T>, typename In::value_type>, "type does not match vector's value type");
  size_t i = 0;

  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
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
  auto first = in.begin(), last = in.end();
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
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  size_t i = 0;
  for (auto it = in.begin(), end = in.end(); it != end; ++it) {
    auto x = *it;
    if (details::ap(p, x)) return static_cast<std::make_signed_t<size_t>>(i);
    i++;
  }
  return -1;
}

template <typename In, typename Predicate> //
[[nodiscard]] constexpr auto find_last(const In &in, Predicate p) {
  using T = typename In::value_type;
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  if constexpr (has_rbegin<In> && has_rend<In>) {
    auto it = std::find_if(in.rbegin(), in.rend(), [&](auto x) { return details::ap(p, x); });
    if (it == in.rend()) return std::optional<T>{};
    else return std::optional<T>{*it};
  } else {
    std::optional<T> found;
    for (auto it = in.begin(), end = in.end(); it != end; ++it) {
      if (details::ap(p, *it)) found = *it;
    }
    return found;
  }
}

template <typename In, template <typename...> typename Out> //
[[nodiscard]] constexpr auto zip_with_index(const In &in) {
  using T = typename In::value_type;
  if constexpr (std::is_default_constructible_v<T>) {
    Out<std::pair<T, size_t>> ys(in.size());
    auto it = ys.begin();
    size_t i = 0;
    for (auto x : in) {
      *it++ = {x, i};
      i++;
    }
    return ys;
  } else {
    Out<std::pair<T, size_t>> ys;
    if constexpr (has_reserve<Out<std::pair<T, size_t>>>) ys.reserve(in.size());
    size_t i = 0;
    for (auto it = in.begin(), end = in.end(); it != end; ++it) {
      auto x = *it;
      ys.emplace_back(x, i);
      i++;
    }
    return ys;
  }
}

template <typename In, typename Container, template <typename...> typename Out> //
[[nodiscard]] constexpr auto zip(const In &in, const Container &c) {
  using T = typename In::value_type;
  using U = typename std::decay_t<Container>::value_type;
  size_t min_size = std::min(in.size(), c.size());
  Out<std::pair<T, U>> ys;
  if constexpr (has_reserve<Out<std::pair<T, U>>>) ys.reserve(min_size);
  auto it_o = in.begin();
  auto it_c = c.begin();
  for (size_t i = 0; i < min_size; ++i, ++it_o, ++it_c)
    ys.emplace_back(*it_o, *it_c);

  return ys;
}

template <typename In, template <typename...> typename Out> //
[[nodiscard]] constexpr auto transpose(const In &in) {
  static_assert(is_iterable<typename In::value_type>, "not a nested type that is iterable");
  using T = typename In::value_type::value_type;
  using Inner = typename In::value_type;

  if (in.empty()) return Out<Out<T>>();

  size_t cols = in.begin()->size();
  size_t idx = 1;
  for (auto it = std::next(in.begin()); it != in.end(); ++it, ++idx) {
    if (it->size() != cols)
      throw std::length_error("cannot transposed a non-square (i.e all nested contains must share the same size) nested container"
                              ": container at [" +
                              std::to_string(idx) + "] has size of " + std::to_string(it->size()) + " but [0] is " + std::to_string(cols));
  }

  Out<Out<T>> ys;
  if constexpr (has_reserve<Out<Out<T>>>) ys.reserve(cols);

  std::vector<typename Inner::const_iterator> iterators;
  for (const Inner &x : in)
    iterators.push_back(x.begin());

  for (size_t j = 0; j < cols; ++j) {
    Out<T> row;
    if constexpr (has_reserve<Out<T>>) row.reserve(in.size());
    for (size_t i = 0; i < in.size(); ++i) {
      row.push_back(*(iterators[i]));
      if (j < cols - 1) ++(iterators[i]);
    }
    ys.push_back(std::move(row));
  }
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out reverse(const In &in) {
  Out ys = in;
  if constexpr (has_reverse<In>) ys.reverse();
  else std::reverse(ys.begin(), ys.end());
  return ys;
}

template <typename In, typename URBG, typename Out> //
[[nodiscard]] constexpr Out shuffle(const In &in, URBG &&urbg) {
  if constexpr (std::is_same_v<typename std::iterator_traits<typename In::iterator>::iterator_category, std::random_access_iterator_tag>) {
    Out ys = in;
    std::shuffle(ys.begin(), ys.end(), std::forward<URBG &&>(urbg));
    return ys;
  } else {
    std::vector<std::reference_wrapper<const typename In::value_type>> refs{in.cbegin(), in.cend()};
    std::shuffle(refs.begin(), refs.end(), std::forward<URBG &&>(urbg));
    return Out{refs.begin(), refs.end()};
  }
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out sort(const In &in) {
  Out ys = in;
  if constexpr (has_sort<In>) ys.sort();
  else std::sort(ys.begin(), ys.end());
  return ys;
}

template <typename In, typename Compare, typename Out> //
[[nodiscard]] constexpr Out sort(const In &in, Compare &&c) {
  Out ys = in;
  if constexpr (has_sort<In>) ys.sort(c);
  else std::sort(ys.begin(), ys.end(), c);
  return ys;
}

template <typename In, typename Select, typename Out> //
[[nodiscard]] constexpr Out sort_by(const In &in, Select &&s) {
  Out ys = in;
  const auto compare = [&](auto l, auto r) { return details::ap(s, l) < details::ap(s, r); };
  if constexpr (has_sort<In>) ys.sort(compare);
  else std::sort(ys.begin(), ys.end(), compare);
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr std::pair<Out, Out> split_at(const In &in, size_t idx) {
  auto size = in.size();
  if (size == 0) return {Out{}, Out{}};
  if (idx > size) return {in, Out{}};

  Out l;
  Out r;
  if constexpr (has_reserve<Out>) {
    l.reserve(idx);
    r.reserve(size - idx);
  }

  auto first_end = in.begin();
  std::advance(first_end, idx);
  std::copy(in.begin(), first_end, std::back_inserter(l));
  std::copy(first_end, in.end(), std::back_inserter(r));
  return {l, r};
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out take(const In &in, size_t n) {
  if (n >= in.size()) return in;
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(n);
  std::copy_n(in.begin(), n, std::back_inserter(ys));
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out drop(const In &in, size_t n) {
  if (n >= in.size()) return In{};
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(in.size() - n);
  auto it = in.begin();
  std::advance(it, n);
  std::copy(it, in.end(), std::back_inserter(ys));
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out take_right(const In &in, size_t n) {
  static_assert(std::is_same_v<std::bidirectional_iterator_tag, typename In::const_iterator::iterator_category> ||
                    std::is_same_v<std::random_access_iterator_tag, typename In::const_iterator::iterator_category>
#if __cplusplus >= 202002L
                    || std::is_same_v<std::contiguous_iterator_tag, typename In::const_iterator::iterator_category>
#endif
                ,
                "iterator does not meet a minimum of the BidirectionalIterator requirement");
  if (n >= in.size()) return in;
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(n);
  auto it = in.end();
  std::advance(it, -static_cast<std::make_signed_t<size_t>>(n));
  std::copy(it, in.end(), std::back_inserter(ys));
  return ys;
}

template <typename In, typename Out> //
[[nodiscard]] constexpr Out drop_right(const In &in, size_t n) {
  static_assert(std::is_same_v<std::bidirectional_iterator_tag, typename In::const_iterator::iterator_category> ||
                    std::is_same_v<std::random_access_iterator_tag, typename In::const_iterator::iterator_category>
#if __cplusplus >= 202002L
                    || std::is_same_v<std::contiguous_iterator_tag, typename In::const_iterator::iterator_category>
#endif
                ,
                "iterator does not meet a minimum of the BidirectionalIterator requirement");
  if (n >= in.size()) return In{};
  Out ys;
  if constexpr (has_reserve<Out>) ys.reserve(in.size() - n);
  auto it = in.end();
  std::advance(it, -static_cast<std::make_signed_t<size_t>>(n));
  std::copy(in.begin(), it, std::back_inserter(ys));
  return ys;
}

template <typename In, typename Predicate, typename Out> //
[[nodiscard]] constexpr Out take_while(const In &in, Predicate &&p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  Out ys;
  for (const auto &x : in) {
    if (!details::ap(p, x)) break;
    ys.insert(ys.end(), x);
  }
  return ys;
}

template <typename In, typename Predicate, typename Out> //
[[nodiscard]] constexpr Out drop_while(const In &in, Predicate &&p) {
  if constexpr (details::assert_predicate<decltype(details::ap(p, *in.begin()))>()) {}
  Out ys;
  auto it = in.begin();
  while (it != in.end() && details::ap(p, *it))
    ++it;
  if constexpr (has_reserve<Out>) ys.reserve(std::distance(it, in.end()));
  std::copy(it, in.end(), std::back_inserter(ys));
  return ys;
}

template <typename In, typename Accumulator, typename Function>
[[nodiscard]] constexpr auto fold_left(const In &in, Accumulator &&init, Function &&function) {
  static_assert(std::is_invocable_v<Function, Accumulator, decltype(*in.begin())>,
                "function must be invocable with with accumulator and container's value type (in this exact order)");
  for (const auto &element : in)
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

template <typename In, template <typename...> typename Out> //
[[nodiscard]] constexpr auto sliding(const In &in, size_t size, size_t step) {
  using T = typename In::value_type;

  if (size == 0 || step == 0)
    throw std::range_error("cannot apply windowing with zero size or step, size=" + std::to_string(size) = " step=" + std::to_string(step));
  if (in.empty()) return Out<Out<T>>{};
  if (in.size() <= size) return Out<Out<T>>{in};

  Out<Out<T>> ys;
  auto it = in.begin();
  while (it != in.end()) {
    auto window_size = std::min(size, static_cast<size_t>(std::distance(it, in.end())));
    if (window_size < size && step == 1) break;
    Out<T> window;
    if constexpr (has_reserve<Out<T>>) window.reserve(window_size);
    std::copy_n(it, window_size, std::back_inserter(window));
    ys.push_back(window);
    if (std::distance(it, in.end()) <= static_cast<typename decltype(it)::difference_type>(step)) break;
    std::advance(it, step);
  }
  return ys;
}

} // namespace aspartame::details::sequence1