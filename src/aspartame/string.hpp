#pragma once

#include <string>

#include "details/base.hpp"
#include "details/container1_impl.hpp"
#include "details/sequence1_impl.hpp"

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp"
#endif

#define ASPARTAME_STRING(T, op, ...)                                                                                                       \
  if constexpr (is_basic_string<T>) { return string_##op(__VA_ARGS__); }

namespace aspartame {
namespace details {
template <typename T> constexpr bool is_basic_string_impl = false;
template <typename CharT, typename Traits, typename Allocator>
constexpr bool is_basic_string_impl<std::basic_string<CharT, Traits, Allocator>> = true;
} // namespace details
template <typename T> inline constexpr bool is_basic_string = details::is_basic_string_impl<std::decay_t<T>>;

template <typename Op> [[nodiscard]] auto operator|(const std::string &l, const Op &r) { return r(l); }
template <typename Op> [[nodiscard]] auto operator|(const std::wstring &l, const Op &r) { return r(l); }
template <typename Op> [[nodiscard]] auto operator|(const std::u16string &l, const Op &r) { return r(l); }
template <typename Op> [[nodiscard]] auto operator|(const std::u32string &l, const Op &r) { return r(l); }
#if __cplusplus >= 202002L
template <typename Op> auto operator|(const std::u8string &l, const Op &r) { return r(l); }
#endif

} // namespace aspartame

#define ASPARTAME_PREFIX(name) string_##name
#define ASPARTAME_OUT_TYPE std::basic_string

namespace aspartame {

// container

template <typename In, typename Function> //
[[nodiscard]] auto ASPARTAME_PREFIX(mk_string)(const In &in, const std::string_view &sep, const std::string_view &prefix,
                                               const std::string_view &suffix, Function &&f) {
  return details::container::mk_string(in, sep, prefix, suffix, f);
}

template <typename In, typename T> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(append)(const In &in, T &&t) {
  return details::container::append<In, T, In>(in, std::forward<T &&>(t));
}

template <typename In, typename Container> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(concat)(const In &in, Container &&other) {
  return in + other;
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(map)(const In &in, Function &&function) {
  using T = decltype(details::ap(function, *std::begin(in)));
  static_assert(std::is_trivial_v<T>, "return type must be trivial for a string container");
  if constexpr (std::is_convertible_v<typename In::value_type, T>) {
    In ys(in.size(), {});
    std::transform(in.begin(), in.end(), ys.begin(), [&](auto x) { return details::ap(function, x); });
    return ys;
  } else {
    std::basic_string<T> ys(in.size(), {});
    std::transform(in.begin(), in.end(), ys.begin(), [&](auto x) { return details::ap(function, x); });
    return ys;
  }
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(collect)(const In &in, Function &&function) {
  return details::container::collect<In, Function, ASPARTAME_OUT_TYPE>(in, function);
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(filter)(const In &in, Predicate &&predicate) {
  return details::container::filter<In, Predicate, ASPARTAME_OUT_TYPE>(in, predicate);
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(bind)(const In &in, Function &&function) {
  using T = decltype(details::ap(function, *std::begin(in)));
  static_assert(std::is_convertible_v<T, In>, "bind function should return an string type");
  In ys;
  for (auto x : in)
    ys += details::ap(function, x);
  return ys;
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(flatten)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(distinct_by)(const In &in, Function &&function) {
  return details::container::distinct_by<In, Function, In>(in, function);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(distinct)(const In &in) {
  return details::container::distinct<In, In, false>(in);
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(count)(const In &in, Predicate &&predicate) {
  return details::container::count<In, Predicate>(in, predicate);
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(exists)(const In &in, Predicate &&predicate) {
  return details::container::exists<In, Predicate>(in, predicate);
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(forall)(const In &in, Predicate &&predicate) {
  return details::container::forall<In, Predicate>(in, predicate);
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(reduce)(const In &in, Function &&function) {
  return details::container::reduce<In, Function>(in, function);
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(tap_each)(const In &in, Function &&function) {
  return details::container::tap_each<In, Function>(in, function);
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(for_each)(const In &in, Function &&function) {
  details::container::for_each<In, Function>(in, function);
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(partition)(const In &in, Predicate &&predicate) {
  return details::container::partition<In, Predicate, In>(in, predicate);
}

template <typename In, typename GroupFunction, typename MapFunction, typename ReduceFunction> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(group_map_reduce)(const In &in, GroupFunction &&group, MapFunction &&map,
                                                                    ReduceFunction &&reduce) {
  return details::container::group_map_reduce<In, GroupFunction, MapFunction, ReduceFunction>(in, group, map, reduce);
}

template <typename In, typename GroupFunction, typename MapFunction> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(group_map)(const In &in, GroupFunction &&group, MapFunction &&map) {
  return details::container::group_map<In, GroupFunction, MapFunction, std::vector>(in, group, map);
}

template <typename In, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(group_by)(const In &in, Function &&function) {
  return details::container::group_by<In, Function, std::vector>(in, function);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(to_vector)(const In &in) {
  if constexpr (is_vector<In>) return in;
  else return std::vector<typename In::value_type>{in.begin(), in.end()};
}

// sequence

template <typename In, typename T> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(prepend)(const In &in, const T &t) {
  return details::sequence::prepend<In, T, In>(in, t);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(head_maybe)(const In &in) {
  return details::sequence::head_maybe<In>(in);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(last_maybe)(const In &in) {
  return details::sequence::last_maybe<In>(in);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(init)(const In &in) {
  return details::sequence::init<In, In>(in);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(tail)(const In &in) {
  return details::sequence::tail<In, In>(in);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(at_maybe)(const In &in, size_t idx) {
  return details::sequence::at_maybe<In>(in, idx);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(slice)(const In &in, size_t from_inclusive, size_t to_exclusive) {
  return details::sequence::slice<In, In>(in, from_inclusive, to_exclusive);
}

template <typename In, typename Container> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(index_of_slice)(const In &in, const Container &other) {
  static_assert(std::is_convertible_v<Container, In>, "other string type must be convertible to LHS string type");
  return details::sequence::index_of_slice<In, In>(in, other);
}

template <typename In, typename Container> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(contains_slice)(const In &in, const Container &other) {
  static_assert(std::is_convertible_v<Container, In>, "other string type must be convertible to LHS string type");
  return details::sequence::index_of_slice<In, In>(in, other) != -1;
}

template <typename In, typename T> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(index_of)(const In &in, const T &t) {
  return details::sequence::index_of<In>(in, t);
}

template <typename In, typename T> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(contains)(const In &in, const T &t) {
  return details::sequence::index_of<In>(in, t) != -1;
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(index_where)(const In &in, Predicate &&predicate) {
  return details::sequence::index_where<In, Predicate>(in, predicate);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(zip_with_index)(const In &in) {
  return details::sequence::zip_with_index<In, std::vector>(in);
}

template <typename In, typename Container> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(zip)(const In &in, const Container &other) {
  static_assert(std::is_convertible_v<Container, In>, "other string type must be convertible to LHS string type");
  return details::sequence::zip<In, In, std::vector>(in, static_cast<In>(other));
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(transpose)(const In &in) {
  if constexpr (details::unsupported<In>(in)) {}
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(reverse)(const In &in) {
  return details::sequence::reverse<In, In>(in);
}

template <typename In, typename URBG> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(shuffle)(const In &in, URBG &&urbg) {
  return details::sequence::shuffle<In, URBG, In>(in, std::forward<URBG &&>(urbg));
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(sort)(const In &in) {
  return details::sequence::sort<In, In>(in);
}

template <typename In, typename Compare> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(sort)(const In &in, Compare &&compare) {
  return details::sequence::sort<In, Compare, In>(in, compare);
}

template <typename In, typename Select> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(sort_by)(const In &in, Select &&select) {
  return details::sequence::sort_by<In, Select, In>(in, select);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(split_at)(const In &in, size_t idx) {
  return details::sequence::split_at<In, In>(in, idx);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(take_right)(const In &in, size_t n) {
  return details::sequence::take_right<In, In>(in, n);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(drop_right)(const In &in, size_t n) {
  return details::sequence::drop_right<In, In>(in, n);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(take)(const In &in, size_t n) {
  return details::sequence::take<In, In>(in, n);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(drop)(const In &in, size_t n) {
  return details::sequence::drop<In, In>(in, n);
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(take_while)(const In &in, Predicate &&predicate) {
  return details::sequence::take_while<In, Predicate, In>(in, predicate);
}

template <typename In, typename Predicate> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(drop_while)(const In &in, Predicate &&predicate) {
  return details::sequence::drop_while<In, Predicate, In>(in, predicate);
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(fold_left)(const In &in, Accumulator &&init, Function &&function) {
  return details::sequence::fold_left<In, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename In, typename Accumulator, typename Function> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(fold_right)(const In &in, Accumulator &&init, Function &&function) {
  return details::sequence::fold_right<In, Accumulator, Function>(in, std::forward<Accumulator &&>(init), function);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(sliding)(const In &in, size_t size, size_t step) {
  if (size == 0 || step == 0)
    throw std::range_error("cannot apply windowing with zero size or step, size=" + std::to_string(size) = " step=" + std::to_string(step));
  if (in.empty()) return std::vector<In>{};
  if (in.length() <= size) return std::vector<In>{in};
  std::vector<In> ys;
  size_t i = 0;
  for (; i + size <= in.length(); i += step)
    ys.push_back(in.substr(i, size));
  if (i < in.length() && i - step + size != in.length()) ys.push_back(in.substr(i));
  return ys;
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(trim_leading)(const In &in) {
  auto first_not_space = std::find_if_not(in.begin(), in.end(), isspace);
  return In(first_not_space, in.end());
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(trim_trailing)(const In &in) {
  auto last_not_space = std::find_if_not(in.rbegin(), in.rend(), isspace).base();
  return In(in.begin(), last_not_space);
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(trim)(const In &in) {
  auto first_not_space = std::find_if_not(in.begin(), in.end(), isspace);
  auto last_not_space = std::find_if_not(in.rbegin(), in.rend(), isspace).base();
  return (first_not_space <= last_not_space) ? In(first_not_space, last_not_space) : In();
}

template <typename In> //
[[nodiscard]] /*constexpr*/ bool ASPARTAME_PREFIX(is_blank)(const In &in) {
  return std::all_of(in.begin(), in.end(), [](auto c) { return std::isspace(c); });
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(indent)(const In &in, int n) {
  In out;
  In line;
  In prefix(n > 0 ? n : 0, ' ');
  for (auto c : in) {
    if (c == '\n' || c == in.back()) {
      if (n < 0) {
        line.erase(0, line.find_first_not_of(' '));
        if (static_cast<std::make_signed_t<size_t>>(line.length()) > -n) { line.erase(0, -n); }
      }
      out += prefix + line + c;
      line.clear();
    } else line += c;
  }
  return out;
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(to_upper)(const In &in) {
  In out(in.length(), char());
  std::transform(in.begin(), in.end(), out.begin(), [](auto x) { return std::toupper(x); });
  return out;
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(to_lower)(const In &in) {
  In out(in.length(), char());
  std::transform(in.begin(), in.end(), out.begin(), [](auto x) { return std::tolower(x); });
  return out;
}

template <typename In, typename Needle, typename With> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(replace_all)(const In &in, const Needle &needle, const With &with) {
  auto needle_ = static_cast<In>(needle);
  auto with_ = static_cast<In>(with);
  if (needle_.empty()) return in;
  In out;
  size_t start_pos = 0, pos;
  while ((pos = in.find(needle_, start_pos)) != In::npos) {
    out.append(in, start_pos, pos - start_pos);
    out += with_;
    start_pos = pos + needle_.length();
  }
  out += in.substr(start_pos);
  return out;
}

template <typename In, typename String> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(contains_ignore_case)(const In &in, const String &that) {
  In in_lower = in, that_lower = static_cast<In>(that);
  std::transform(in_lower.begin(), in_lower.end(), in_lower.begin(), [](auto x) { return std::tolower(x); });
  std::transform(that_lower.begin(), that_lower.end(), that_lower.begin(), [](auto x) { return std::tolower(x); });
  return in_lower.find(that_lower) != In::npos;
}

template <typename In, typename String> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(equals_ignore_case)(const In &in, const String &that) {
  auto that_ = static_cast<In>(that);
  if (in.size() != that_.size()) return false;
  for (size_t i = 0; i < in.size(); ++i) {
    if (tolower(in[i]) != tolower(that_[i])) return false;
  }
  return true;
}

template <typename In, typename Delimiter> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(split)(const In &in, const Delimiter &delimiter) {
  std::vector<In> ys;
  size_t start = 0, end;
  if constexpr (std::is_convertible_v<Delimiter, In>) {
    auto delimiter_ = static_cast<In>(delimiter);
    while ((end = in.find(delimiter_, start)) != In::npos) {
      ys.push_back(in.substr(start, end - start));
      start = end + delimiter_.size();
    }
  } else if constexpr (std::is_same_v<Delimiter, typename In::value_type>) {
    while ((end = in.find(delimiter, start)) != In::npos) {
      ys.push_back(in.substr(start, end - start));
      start = end + 1;
    }
  } else static_assert(details::is_supported<Delimiter>, "delimiter must be either the same type as input or a character type of input");
  ys.push_back(in.substr(start));
  return ys;
}

template <typename In> //
[[nodiscard]] /*constexpr*/ auto ASPARTAME_PREFIX(lines)(const In &in) {
  auto xs = string_split(in,  static_cast<typename In::value_type>('\n'));
  if(!xs.empty() && xs.back().empty()) xs.erase(xs.end()-1);
  return xs;
}

} // namespace aspartame

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"

#undef ASPARTAME_PREFIX
#undef ASPARTAME_OUT_TYPE
