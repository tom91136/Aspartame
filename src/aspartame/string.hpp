#pragma once

#include "details/base.hpp"
#include "details/container1_impl.hpp"
#include "details/sequence1_impl.hpp"
#include "fluent.hpp"

#include <string>


namespace aspartame {
namespace details {
template <typename T> constexpr bool is_basic_string_impl = false;
template <typename CharT, typename Traits, typename Allocator>
constexpr bool is_basic_string_impl<std::basic_string<CharT, Traits, Allocator>> = true;
} // namespace details
template <typename T> inline constexpr bool is_basic_string = details::is_basic_string_impl<std::decay_t<T>>;

template <typename Op> [[nodiscard]] auto operator^(const std::string &l, const Op &r) { return r(l); }
template <typename Op> [[nodiscard]] auto operator^(const std::wstring &l, const Op &r) { return r(l); }
template <typename Op> [[nodiscard]] auto operator^(const std::u16string &l, const Op &r) { return r(l); }
template <typename Op> [[nodiscard]] auto operator^(const std::u32string &l, const Op &r) { return r(l); }
#if __cplusplus >= 202002L
template <typename Op> auto operator^(const std::u8string &l, const Op &r) { return r(l); }
#endif

} // namespace aspartame

#define ASPARTAME_OUT_TYPE std::basic_string

namespace aspartame {

// container

template <typename C, typename Function> //
[[nodiscard]] auto mk_string(const std::basic_string<C> &in, const std::string_view &prefix, const std::string_view &sep,
                             const std::string_view &suffix, Function &&f, tag = {}) {
  return details::container1::mk_string(in, prefix, sep, suffix, f);
}

template <typename C, typename T> //
[[nodiscard]] /*constexpr*/ auto append(const std::basic_string<C> &in, T &&t, tag = {}) {
  return details::container1::append<std::basic_string<C>, T, std::basic_string<C>>(in, std::forward<T &&>(t));
}

template <typename C, typename Container> //
[[nodiscard]] /*constexpr*/ auto concat(const std::basic_string<C> &in, Container &&other, tag = {}) {
  return in + other;
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto map(const std::basic_string<C> &in, Function &&function, tag = {}) {
  using T = decltype(details::ap(function, *std::begin(in)));
  static_assert(std::is_trivial_v<T>, "return type must be trivial for a string container");
  if constexpr (std::is_convertible_v<C, T>) {
    std::basic_string<C> ys(in.size(), {});
    std::transform(in.begin(), in.end(), ys.begin(), [&](auto &&x) { return details::ap(function, x); });
    return ys;
  } else {
    std::basic_string<T> ys(in.size(), {});
    std::transform(in.begin(), in.end(), ys.begin(), [&](auto &&x) { return details::ap(function, x); });
    return ys;
  }
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto collect(const std::basic_string<C> &in, Function &&function, tag = {}) {
  return details::container1::collect<std::basic_string<C>, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto filter(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::container1::filter<std::basic_string<C>, Predicate, ASPARTAME_OUT_TYPE>(in, predicate);
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto bind(const std::basic_string<C> &in, Function &&function, tag = {}) {
  using T = decltype(details::ap(function, *std::begin(in)));
  static_assert(std::is_convertible_v<T, std::basic_string<C>>, "bind function should return an string type");
  std::basic_string<C> ys;
  for (auto &&x : in)
    ys += details::ap(function, x);
  return ys;
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto flatten(const std::basic_string<C> &in, tag = {}) {
  if constexpr (details::unsupported<std::basic_string<C>>(in)) {}
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto distinct_by(const std::basic_string<C> &in, Function &&function, tag = {}) {
  return details::container1::distinct_by<std::basic_string<C>, Function, std::basic_string<C>>(in, function);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto distinct(const std::basic_string<C> &in, tag = {}) {
  return details::container1::distinct<std::basic_string<C>, std::basic_string<C>, false>(in);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto count(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::container1::count<std::basic_string<C>, Predicate>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto exists(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::container1::exists<std::basic_string<C>, Predicate>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto forall(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::container1::forall<std::basic_string<C>, Predicate>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto find(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::container1::find<std::basic_string<C>, Predicate>(in, predicate);
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto reduce(const std::basic_string<C> &in, Function &&function, tag = {}) {
  return details::container1::reduce<std::basic_string<C>, Function>(in, function);
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto tap_each(const std::basic_string<C> &in, Function &&function, tag = {}) {
  return details::container1::tap_each<std::basic_string<C>, Function>(in, function);
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto for_each(const std::basic_string<C> &in, Function &&function, tag = {}) {
  details::container1::for_each<std::basic_string<C>, Function>(in, function);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto partition(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::container1::partition<std::basic_string<C>, Predicate, std::basic_string<C>>(in, predicate);
}

template <typename C, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] /*constexpr*/ auto group_map_reduce(const std::basic_string<C> &in, GroupFunction &&group, MapFunction &&map,
                                                  ReduceFunction &&reduce, tag = {}) {
  return details::container1::group_map_reduce<std::basic_string<C>, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename C, typename GroupFunction, typename MapFunction> //
[[nodiscard]] /*constexpr*/ auto group_map(const std::basic_string<C> &in, GroupFunction &&group, MapFunction &&map, tag = {}) {
  return details::container1::group_map<std::basic_string<C>, GroupFunction, MapFunction, std::vector>(in, group, map);
}

template <typename C, typename Function> //
[[nodiscard]] /*constexpr*/ auto group_by(const std::basic_string<C> &in, Function &&function, tag = {}) {
  return details::container1::group_by<std::basic_string<C>, Function, std::vector>(in, function);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto to_vector(const std::basic_string<C> &in, tag = {}) {
  if constexpr (is_vector<std::basic_string<C>>) return in;
  else return std::vector<C>{in.begin(), in.end()};
}

// sequence

template <typename C, typename T> //
[[nodiscard]] /*constexpr*/ auto prepend(const std::basic_string<C> &in, const T &t, tag = {}) {
  return details::sequence1::prepend<std::basic_string<C>, T, std::basic_string<C>>(in, t);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto head_maybe(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::head_maybe<std::basic_string<C>>(in);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto last_maybe(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::last_maybe<std::basic_string<C>>(in);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto init(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::init<std::basic_string<C>, std::basic_string<C>>(in);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto tail(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::tail<std::basic_string<C>, std::basic_string<C>>(in);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto at_maybe(const std::basic_string<C> &in, size_t idx, tag = {}) {
  return details::sequence1::at_maybe<std::basic_string<C>>(in, idx);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto slice(const std::basic_string<C> &in, size_t from_inclusive, size_t to_exclusive, tag = {}) {
  return details::sequence1::slice<std::basic_string<C>, std::basic_string<C>>(in, from_inclusive, to_exclusive);
}

template <typename C, typename Container> //
[[nodiscard]] /*constexpr*/ auto index_of_slice(const std::basic_string<C> &in, const Container &other, tag = {}) {
  static_assert(std::is_convertible_v<Container, std::basic_string<C>>, "other string type must be convertible to LHS string type");
  return details::sequence1::index_of_slice<std::basic_string<C>, std::basic_string<C>>(in, other);
}

template <typename C, typename Container> //
[[nodiscard]] /*constexpr*/ auto contains_slice(const std::basic_string<C> &in, const Container &other, tag = {}) {
  static_assert(std::is_convertible_v<Container, std::basic_string<C>>, "other string type must be convertible to LHS string type");
  return details::sequence1::index_of_slice<std::basic_string<C>, std::basic_string<C>>(in, other) != -1;
}

template <typename C, typename T> //
[[nodiscard]] /*constexpr*/ auto index_of(const std::basic_string<C> &in, const T &t, tag = {}) {
  return details::sequence1::index_of<std::basic_string<C>>(in, t);
}

template <typename C, typename T> //
[[nodiscard]] /*constexpr*/ auto contains(const std::basic_string<C> &in, const T &t, tag = {}) {
  return details::sequence1::index_of<std::basic_string<C>>(in, t) != -1;
}

template <typename C, typename T> //
[[nodiscard]] /*constexpr*/ auto find_last(const std::basic_string<C> &in, const T &t, tag = {}) {
  return details::sequence1::find_last<std::basic_string<C>>(in, t);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto index_where(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::index_where<std::basic_string<C>, Predicate>(in, predicate);
}

template <typename C, typename N> //
[[nodiscard]] /*constexpr*/ auto zip_with_index(const std::basic_string<C> &in, N from, tag = {}) {
  return details::sequence1::zip_with_index<std::basic_string<C>, std::vector, N>(in, from);
}

template <typename C, typename Container> //
[[nodiscard]] /*constexpr*/ auto zip(const std::basic_string<C> &in, const Container &other, tag = {}) {
  static_assert(std::is_convertible_v<Container, std::basic_string<C>>, "other string type must be convertible to LHS string type");
  return details::sequence1::zip<std::basic_string<C>, std::basic_string<C>, std::vector>(in, static_cast<std::basic_string<C>>(other));
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto transpose(const std::basic_string<C> &in, tag = {}) {
  if constexpr (details::unsupported<std::basic_string<C>>(in)) {}
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto reverse(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::reverse<std::basic_string<C>, std::basic_string<C>>(in);
}

template <typename C, typename URBG> //
[[nodiscard]] /*constexpr*/ auto shuffle(const std::basic_string<C> &in, URBG &&urbg, tag = {}) {
  return details::sequence1::shuffle<std::basic_string<C>, URBG, std::basic_string<C>>(in, std::forward<URBG &&>(urbg));
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto sort(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::sort<std::basic_string<C>, std::basic_string<C>>(in);
}

template <typename C, typename Compare> //
[[nodiscard]] /*constexpr*/ auto sort(const std::basic_string<C> &in, Compare &&compare, tag = {}) {
  return details::sequence1::sort<std::basic_string<C>, Compare, std::basic_string<C>>(in, compare);
}

template <typename C, typename Select> //
[[nodiscard]] /*constexpr*/ auto sort_by(const std::basic_string<C> &in, Select &&select, tag = {}) {
  return details::sequence1::sort_by<std::basic_string<C>, Select, std::basic_string<C>>(in, select);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto split_at(const std::basic_string<C> &in, size_t idx, tag = {}) {
  return details::sequence1::split_at<std::basic_string<C>, std::basic_string<C>>(in, idx);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto take_right(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::take_right<std::basic_string<C>, std::basic_string<C>>(in, n);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto drop_right(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::drop_right<std::basic_string<C>, std::basic_string<C>>(in, n);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto take(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::take<std::basic_string<C>, std::basic_string<C>>(in, n);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto drop(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::drop<std::basic_string<C>, std::basic_string<C>>(in, n);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto take_while(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::take_while<std::basic_string<C>, Predicate, std::basic_string<C>>(in, predicate);
}

template <typename C, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto drop_while(const std::basic_string<C> &in, Predicate &&predicate, tag = {}) {
  return details::sequence1::drop_while<std::basic_string<C>, Predicate, std::basic_string<C>>(in, predicate);
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] /*constexpr*/ auto fold_left(const std::basic_string<C> &in, Accumulator &&init, Function &&function, tag = {}) {
  return details::sequence1::fold_left<std::basic_string<C>, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C, typename Accumulator, typename Function> //
[[nodiscard]] /*constexpr*/ auto fold_right(const std::basic_string<C> &in, Accumulator &&init, Function &&function, tag = {}) {
  return details::sequence1::fold_right<std::basic_string<C>, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto sliding(const std::basic_string<C> &in, size_t size, size_t step, tag = {}) {
  if (size == 0 || step == 0)
    throw std::range_error("cannot apply windowing with zero size or step, size=" + std::to_string(size) = " step=" + std::to_string(step));
  if (in.empty()) return std::vector<std::basic_string<C>>{};
  if (in.length() <= size) return std::vector<std::basic_string<C>>{in};
  std::vector<std::basic_string<C>> ys;
  size_t i = 0;
  for (; i + size <= in.length(); i += step)
    ys.push_back(in.substr(i, size));
  if (i < in.length() && i - step + size != in.length()) ys.push_back(in.substr(i));
  return ys;
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto trim_leading(const std::basic_string<C> &in, tag = {}) {
  auto first_not_space = std::find_if_not(in.begin(), in.end(), isspace);
  return std::basic_string<C>(first_not_space, in.end());
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto trim_trailing(const std::basic_string<C> &in, tag = {}) {
  auto last_not_space = std::find_if_not(in.rbegin(), in.rend(), isspace).base();
  return std::basic_string<C>(in.begin(), last_not_space);
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto trim(const std::basic_string<C> &in, tag = {}) {
  auto first_not_space = std::find_if_not(in.begin(), in.end(), isspace);
  auto last_not_space = std::find_if_not(in.rbegin(), in.rend(), isspace).base();
  return (first_not_space <= last_not_space) ? std::basic_string<C>(first_not_space, last_not_space) : std::basic_string<C>();
}

template <typename C> //
[[nodiscard]] /*constexpr*/ bool is_blank(const std::basic_string<C> &in, tag = {}) {
  return std::all_of(in.begin(), in.end(), [](auto &&c) { return std::isspace(c); });
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto to_upper(const std::basic_string<C> &in, tag = {}) {
  std::basic_string<C> out(in.length(), char());
  std::transform(in.begin(), in.end(), out.begin(), [](auto &&x) { return std::toupper(x); });
  return out;
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto to_lower(const std::basic_string<C> &in, tag = {}) {
  std::basic_string<C> out(in.length(), char());
  std::transform(in.begin(), in.end(), out.begin(), [](auto &&x) { return std::tolower(x); });
  return out;
}

template <typename C, typename Needle, typename With> //
[[nodiscard]] /*constexpr*/ auto replace_all(const std::basic_string<C> &in, const Needle &needle, const With &with, tag = {}) {
  auto needle_ = static_cast<std::basic_string<C>>(needle);
  auto with_ = static_cast<std::basic_string<C>>(with);
  if (needle_.empty()) return in;
  std::basic_string<C> out;
  size_t start_pos = 0, pos;
  while ((pos = in.find(needle_, start_pos)) != std::basic_string<C>::npos) {
    out.append(in, start_pos, pos - start_pos);
    out += with_;
    start_pos = pos + needle_.length();
  }
  out += in.substr(start_pos);
  return out;
}

template <typename C, typename NewLine> //
[[nodiscard]] /*constexpr*/ auto indent(const std::basic_string<C> &in, int n, const NewLine &nl, tag = {}) {
  if (n == 0 || in.empty()) return in;
  auto nl_ = static_cast<std::basic_string<C>>(nl);
  std::basic_string<C> prefix(n > 0 ? n : 0, ' ');
  std::basic_string<C> out(prefix);
  size_t start_pos = 0, pos;
  if (n < 0) {
    while (start_pos < in.size() && start_pos < static_cast<size_t>(-n) && in[start_pos] == ' ')
      start_pos++;
  }
  while ((pos = in.find(nl_, start_pos)) != std::basic_string<C>::npos) {
    size_t after_nl = pos + nl_.size();
    out.append(in, start_pos, after_nl - start_pos);
    if (n > 0) {
      if (pos != in.size() - nl_.size()) out += prefix;
      start_pos = after_nl;
    } else {
      int drop = 0; // find out how many ws chars after nl and drop those
      while (after_nl + drop < in.size() && drop < -n && in[after_nl + drop] == ' ')
        drop++;
      start_pos = after_nl + drop;
    }
  }
  out += in.substr(start_pos);
  return out;
}

template <typename C, typename String> //
[[nodiscard]] /*constexpr*/ auto starts_with(const std::basic_string<C> &in, const String &that, tag = {}) {
  auto that_ = static_cast<std::basic_string<C>>(that);
  return in.size() >= that_.size() && in.compare(0, that_.size(), that_) == 0;
}

template <typename C, typename String> //
[[nodiscard]] /*constexpr*/ auto ends_with(const std::basic_string<C> &in, const String &that, tag = {}) {
  auto that_ = static_cast<std::basic_string<C>>(that);
  return in.size() >= that_.size() && in.compare(in.size() - that_.size(), that_.size(), that_) == 0;
}

template <typename C, typename String> //
[[nodiscard]] /*constexpr*/ auto contains_ignore_case(const std::basic_string<C> &in, const String &that, tag = {}) {
  std::basic_string<C> in_lower = in, that_lower = static_cast<std::basic_string<C>>(that);
  std::transform(in_lower.begin(), in_lower.end(), in_lower.begin(), [](auto &&x) { return std::tolower(x); });
  std::transform(that_lower.begin(), that_lower.end(), that_lower.begin(), [](auto &&x) { return std::tolower(x); });
  return in_lower.find(that_lower) != std::basic_string<C>::npos;
}

template <typename C, typename String> //
[[nodiscard]] /*constexpr*/ auto equals_ignore_case(const std::basic_string<C> &in, const String &that, tag = {}) {
  auto that_ = static_cast<std::basic_string<C>>(that);
  if (in.size() != that_.size()) return false;
  for (size_t i = 0; i < in.size(); ++i) {
    if (tolower(in[i]) != tolower(that_[i])) return false;
  }
  return true;
}

template <typename C, typename Delimiter> //
[[nodiscard]] /*constexpr*/ auto split(const std::basic_string<C> &in, const Delimiter &delimiter, tag = {}) {
  std::vector<std::basic_string<C>> ys;
  size_t start = 0, end;
  if constexpr (std::is_convertible_v<Delimiter, std::basic_string<C>>) {
    auto delimiter_ = static_cast<std::basic_string<C>>(delimiter);
    while ((end = in.find(delimiter_, start)) != std::basic_string<C>::npos) {
      ys.push_back(in.substr(start, end - start));
      start = end + delimiter_.size();
    }
  } else if constexpr (std::is_same_v<Delimiter, C>) {
    while ((end = in.find(delimiter, start)) != std::basic_string<C>::npos) {
      ys.push_back(in.substr(start, end - start));
      start = end + 1;
    }
  } else static_assert(details::is_supported<Delimiter>, "delimiter must be either the same type as input or a character type of input");
  ys.push_back(in.substr(start));
  return ys;
}

template <typename C> //
[[nodiscard]] /*constexpr*/ auto lines(const std::basic_string<C> &in, tag = {}) {
  auto xs = split(in, static_cast<C>('\n'));
  if (!xs.empty() && xs.back().empty()) xs.erase(xs.end() - 1);
  return xs;
}

} // namespace aspartame
#undef ASPARTAME_OUT_TYPE

#define ASPARTAME_IN_TYPE2(K, V) std::basic_string<std::pair<K, V>>
#define ASPARTAME_IN_TYPE1(C) std::basic_string<C>

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"

#undef ASPARTAME_IN_TYPE2
#undef ASPARTAME_IN_TYPE1
