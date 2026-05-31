#pragma once

#include <algorithm>
#include <cctype>
#include <cwctype>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "details/container1_impl.hpp"
#include "details/sequence1_impl.hpp"
#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

namespace details {
template <typename T> constexpr bool is_basic_string_impl = false;
template <typename CharT, typename Traits, typename Allocator>
constexpr bool is_basic_string_impl<std::basic_string<CharT, Traits, Allocator>> = true;

template <typename T>
struct is_string_char_impl : std::bool_constant<std::is_same_v<std::decay_t<T>, char> || std::is_same_v<std::decay_t<T>, wchar_t> ||
                                                std::is_same_v<std::decay_t<T>, char16_t> || std::is_same_v<std::decay_t<T>, char32_t>
#if __cplusplus >= 202002L
                                                || std::is_same_v<std::decay_t<T>, char8_t>
#endif
                                                > {
};
} // namespace details

template <typename T> inline constexpr bool is_basic_string = details::is_basic_string_impl<std::decay_t<T>>;
template <typename T> inline constexpr bool is_string_char = details::is_string_char_impl<std::decay_t<T>>::value;

template <typename CharT, typename Traits, typename Allocator>
struct enable_pipe<std::basic_string<CharT, Traits, Allocator>> : std::true_type {};
template <typename CharT, typename Traits, typename Allocator>
struct enable_string_ops<std::basic_string<CharT, Traits, Allocator>> : std::true_type {};

template <typename CharT, typename Traits, typename Allocator> struct monoid_traits<std::basic_string<CharT, Traits, Allocator>> {
  static std::basic_string<CharT, Traits, Allocator> empty() { return {}; }
  static std::basic_string<CharT, Traits, Allocator> combine(std::basic_string<CharT, Traits, Allocator> a,
                                                             const std::basic_string<CharT, Traits, Allocator> &b) {
    a += b;
    return a;
  }
};

template <typename C, typename Op, std::enable_if_t<is_string_char<C>, int> = 0>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto operator^(const C *l, const Op &r) {
  std::basic_string<C> s{l};
  return r(s, tag{});
}

namespace details {
template <typename C> struct string_char_ops {
  static bool is_space(C c) {
    return c == static_cast<C>(' ') || c == static_cast<C>('\t') || c == static_cast<C>('\n') || c == static_cast<C>('\r') ||
           c == static_cast<C>('\f') || c == static_cast<C>('\v');
  }
  static C to_upper(C c) {
    if (c >= static_cast<C>('a') && c <= static_cast<C>('z')) return static_cast<C>(c - static_cast<C>('a') + static_cast<C>('A'));
    return c;
  }
  static C to_lower(C c) {
    if (c >= static_cast<C>('A') && c <= static_cast<C>('Z')) return static_cast<C>(c - static_cast<C>('A') + static_cast<C>('a'));
    return c;
  }
};
template <> struct string_char_ops<char> {
  static bool is_space(char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; }
  static char to_upper(char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); }
  static char to_lower(char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }
};
template <> struct string_char_ops<wchar_t> {
  static bool is_space(wchar_t c) { return std::iswspace(c) != 0; }
  static wchar_t to_upper(wchar_t c) { return static_cast<wchar_t>(std::towupper(c)); }
  static wchar_t to_lower(wchar_t c) { return static_cast<wchar_t>(std::towlower(c)); }
};
template <> struct string_char_ops<unsigned char> {
  static bool is_space(unsigned char c) { return std::isspace(c) != 0; }
  static unsigned char to_upper(unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); }
  static unsigned char to_lower(unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); }
};
template <> struct string_char_ops<signed char> {
  static bool is_space(signed char c) { return string_char_ops<unsigned char>::is_space(static_cast<unsigned char>(c)); }
  static signed char to_upper(signed char c) {
    return static_cast<signed char>(string_char_ops<unsigned char>::to_upper(static_cast<unsigned char>(c)));
  }
  static signed char to_lower(signed char c) {
    return static_cast<signed char>(string_char_ops<unsigned char>::to_lower(static_cast<unsigned char>(c)));
  }
};
#if __cplusplus >= 202002L
template <> struct string_char_ops<char8_t> {
  static bool is_space(char8_t c) { return string_char_ops<unsigned char>::is_space(static_cast<unsigned char>(c)); }
  static char8_t to_upper(char8_t c) {
    return static_cast<char8_t>(string_char_ops<unsigned char>::to_upper(static_cast<unsigned char>(c)));
  }
  static char8_t to_lower(char8_t c) {
    return static_cast<char8_t>(string_char_ops<unsigned char>::to_lower(static_cast<unsigned char>(c)));
  }
};
#endif
} // namespace details

template <typename C, typename F>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto mk_string(const std::basic_string<C> &in, std::string_view prefix, std::string_view sep,
                                                       std::string_view suffix, F &&f, tag = {}) {
  return details::container1::mk_string<std::basic_string<C>, F>(in, prefix, sep, suffix, std::forward<F>(f));
}

template <typename C, typename T> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto append(const std::basic_string<C> &in, T &&t, tag = {}) {
  return details::container1::append<std::basic_string<C>, std::basic_string<C>, T>(in, std::forward<T>(t));
}

template <typename C, typename Container>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto concat(const std::basic_string<C> &in, Container &&other, tag = {}) {
  return in + other;
}

template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto map(const std::basic_string<C> &in, F &&f, tag = {}) {
  using T = std::decay_t<decltype(details::ap(f, *std::begin(in)))>;
  if constexpr (std::is_trivial_v<T> && std::is_convertible_v<C, T>) {
    std::basic_string<C> ys(in.size(), C{});
    std::transform(in.begin(), in.end(), ys.begin(), [&](auto &&x) { return details::ap(f, x); });
    return ys;
  } else if constexpr (std::is_trivial_v<T>) {
    std::basic_string<T> ys(in.size(), T{});
    std::transform(in.begin(), in.end(), ys.begin(), [&](auto &&x) { return details::ap(f, x); });
    return ys;
  } else {
    std::vector<T> ys;
    ys.reserve(in.size());
    std::transform(in.begin(), in.end(), std::back_inserter(ys), [&](auto &&x) { return details::ap(f, x); });
    return ys;
  }
}

template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto collect(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::container1::collect<std::basic_string<C>, std::basic_string<C>, F>(in, std::forward<F>(f));
}

template <typename C, typename F>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto collect_first(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::sequence1::collect_first<std::basic_string<C>, F>(in, std::forward<F>(f));
}

template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto filter(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::container1::filter<std::basic_string<C>, std::basic_string<C>, P>(in, std::forward<P>(p));
}

template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto flat_map(const std::basic_string<C> &in, F &&f, tag = {}) {
  using T = decltype(details::ap(f, *std::begin(in)));
  static_assert(std::is_convertible_v<T, std::basic_string<C>>, "flat_map function should return a string type");
  std::basic_string<C> ys;
  for (auto &&x : in)
    ys += details::ap(f, x);
  return ys;
}

template <typename C, typename F>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto distinct_by(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::container1::distinct_by<std::basic_string<C>, std::basic_string<C>, F>(in, std::forward<F>(f));
}

template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto distinct(const std::basic_string<C> &in, tag = {}) {
  return details::container1::distinct<std::basic_string<C>, std::basic_string<C>, false>(in);
}

template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto count(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::container1::count<std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto exists(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::container1::exists<std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto forall(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::container1::forall<std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto find(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::container1::find<std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto reduce(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::container1::reduce<std::basic_string<C>, F>(in, std::forward<F>(f));
}
template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto tap_each(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::container1::tap_each<std::basic_string<C>, F>(in, std::forward<F>(f));
}
template <typename C, typename F> auto for_each(const std::basic_string<C> &in, F &&f, tag = {}) {
  details::container1::for_each<std::basic_string<C>, F>(in, std::forward<F>(f));
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto partition(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::container1::partition<std::basic_string<C>, std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename G, typename M, typename R>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_map_reduce(const std::basic_string<C> &in, G &&g, M &&m, R &&r, tag = {}) {
  return details::container1::group_map_reduce<std::basic_string<C>, G, M, R>(in, std::forward<G>(g), std::forward<M>(m),
                                                                              std::forward<R>(r));
}
template <typename C, typename G, typename M>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_map(const std::basic_string<C> &in, G &&g, M &&m, tag = {}) {
  using V = std::decay_t<decltype(details::ap(m, *in.begin()))>;
  using Inner = std::vector<V>;
  return details::container1::group_map<std::basic_string<C>, Inner, G, M>(in, std::forward<G>(g), std::forward<M>(m));
}
template <typename C, typename G> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto group_by(const std::basic_string<C> &in, G &&g, tag = {}) {
  using Inner = std::vector<C>;
  return details::container1::group_by<std::basic_string<C>, Inner, G>(in, std::forward<G>(g));
}

template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto to_vector(const std::basic_string<C> &in, tag = {}) {
  return std::vector<C>{in.begin(), in.end()};
}
template <template <typename...> typename Cs, typename C>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto to(const std::basic_string<C> &in, tag = {}) {
  return Cs<C>{in.begin(), in.end()};
}

template <typename C, typename T>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto prepend(const std::basic_string<C> &in, const T &t, tag = {}) {
  return details::sequence1::prepend<std::basic_string<C>, T, std::basic_string<C>>(in, t);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto head_maybe(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::head_maybe<std::basic_string<C>>(in);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto last_maybe(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::last_maybe<std::basic_string<C>>(in);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto init(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::init<std::basic_string<C>, std::basic_string<C>>(in);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto tail(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::tail<std::basic_string<C>, std::basic_string<C>>(in);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto at_maybe(const std::basic_string<C> &in, size_t idx, tag = {}) {
  return details::sequence1::at_maybe<std::basic_string<C>>(in, idx);
}
template <typename C>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto slice(const std::basic_string<C> &in, size_t from_inclusive, size_t to_exclusive, tag = {}) {
  return details::sequence1::slice<std::basic_string<C>, std::basic_string<C>>(in, from_inclusive, to_exclusive);
}
template <typename C, typename Container>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto index_of_slice(const std::basic_string<C> &in, const Container &other, tag = {}) {
  static_assert(std::is_convertible_v<Container, std::basic_string<C>>, "other string type must be convertible to LHS string type");
  return details::sequence1::index_of_slice<std::basic_string<C>, std::basic_string<C>>(in, other);
}
template <typename C, typename Container>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto contains_slice(const std::basic_string<C> &in, const Container &other, tag = {}) {
  return index_of_slice(in, other, tag{}) != -1;
}
template <typename C, typename T>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto index_of(const std::basic_string<C> &in, const T &t, tag = {}) {
  return details::sequence1::index_of<std::basic_string<C>>(in, t);
}
template <typename C, typename T>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto contains(const std::basic_string<C> &in, const T &t, tag = {}) {
  return index_of(in, t, tag{}) != -1;
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto find_last(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::sequence1::find_last<std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename P>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto index_where(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::sequence1::index_where<std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename N>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto zip_with_index(const std::basic_string<C> &in, N from, tag = {}) {
  using Pair = std::pair<C, N>;
  using Out = std::vector<Pair>;
  return details::sequence1::zip_with_index<std::basic_string<C>, Out, N>(in, from);
}
template <typename C, typename Container>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto zip(const std::basic_string<C> &in, const Container &other, tag = {}) {
  static_assert(std::is_convertible_v<Container, std::basic_string<C>>, "other string type must be convertible to LHS string type");
  auto other_ = static_cast<std::basic_string<C>>(other);
  using Pair = std::pair<C, C>;
  using Out = std::vector<Pair>;
  return details::sequence1::zip<std::basic_string<C>, std::basic_string<C>, Out>(in, other_);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto reverse(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::reverse<std::basic_string<C>, std::basic_string<C>>(in);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto combinations(const std::basic_string<C> &in, size_t k, tag = {}) {
  return details::sequence1::combinations<std::basic_string<C>, std::basic_string<C>, std::vector<std::basic_string<C>>>(in, k);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto permutations(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::permutations<std::basic_string<C>, std::basic_string<C>, std::vector<std::basic_string<C>>>(in);
}
template <typename C, typename URBG> [[nodiscard]] auto shuffle(const std::basic_string<C> &in, URBG &&urbg, tag = {}) {
  return details::sequence1::shuffle<std::basic_string<C>, URBG, std::basic_string<C>>(in, std::forward<URBG>(urbg));
}
template <typename C> [[nodiscard]] auto sort(const std::basic_string<C> &in, tag = {}) {
  return details::sequence1::sort<std::basic_string<C>, std::basic_string<C>>(in);
}
template <typename C, typename Cmp> [[nodiscard]] auto sort(const std::basic_string<C> &in, Cmp &&cmp, tag = {}) {
  return details::sequence1::sort<std::basic_string<C>, Cmp, std::basic_string<C>>(in, std::forward<Cmp>(cmp));
}
template <typename C, typename Sel> [[nodiscard]] auto sort_by(const std::basic_string<C> &in, Sel &&s, tag = {}) {
  return details::sequence1::sort_by<std::basic_string<C>, Sel, std::basic_string<C>>(in, std::forward<Sel>(s));
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto split_at(const std::basic_string<C> &in, size_t idx, tag = {}) {
  return details::sequence1::split_at<std::basic_string<C>, std::basic_string<C>>(in, idx);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto take(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::take<std::basic_string<C>, std::basic_string<C>>(in, n);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto drop(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::drop<std::basic_string<C>, std::basic_string<C>>(in, n);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto take_right(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::take_right<std::basic_string<C>, std::basic_string<C>>(in, n);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto drop_right(const std::basic_string<C> &in, size_t n, tag = {}) {
  return details::sequence1::drop_right<std::basic_string<C>, std::basic_string<C>>(in, n);
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto take_while(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::sequence1::take_while<std::basic_string<C>, P, std::basic_string<C>>(in, std::forward<P>(p));
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto drop_while(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::sequence1::drop_while<std::basic_string<C>, P, std::basic_string<C>>(in, std::forward<P>(p));
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto span(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::sequence1::span<std::basic_string<C>, P, std::basic_string<C>>(in, std::forward<P>(p));
}
template <typename C, typename Acc, typename F>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto fold_left(const std::basic_string<C> &in, Acc init, F &&f, tag = {}) {
  return details::sequence1::fold_left<std::basic_string<C>, Acc, F>(in, std::move(init), std::forward<F>(f));
}
template <typename C, typename Acc, typename F>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto fold_right(const std::basic_string<C> &in, Acc init, F &&f, tag = {}) {
  return details::sequence1::fold_right<std::basic_string<C>, Acc, F>(in, std::move(init), std::forward<F>(f));
}
template <typename C>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sliding(const std::basic_string<C> &in, size_t size, size_t step, tag = {}) {
  if (size == 0 || step == 0)
    details::raise<std::range_error>("cannot apply windowing with zero size or step, size=" + std::to_string(size) +
                                     " step=" + std::to_string(step));
  if (in.empty()) return std::vector<std::basic_string<C>>{};
  if (in.length() <= size) return std::vector<std::basic_string<C>>{in};
  std::vector<std::basic_string<C>> ys;
  size_t i = 0;
  for (; i + size <= in.length(); i += step)
    ys.push_back(in.substr(i, size));
  if (i < in.length() && i - step + size != in.length()) ys.push_back(in.substr(i));
  return ys;
}

template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto min(const std::basic_string<C> &in, tag = {}) {
  return details::container1::min<std::basic_string<C>>(in);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto max(const std::basic_string<C> &in, tag = {}) {
  return details::container1::max<std::basic_string<C>>(in);
}
template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto min_by(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::container1::min_by<std::basic_string<C>, F>(in, std::forward<F>(f));
}
template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto max_by(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::container1::max_by<std::basic_string<C>, F>(in, std::forward<F>(f));
}
template <typename C, typename F> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto sum_by(const std::basic_string<C> &in, F &&f, tag = {}) {
  return details::container1::sum_by<std::basic_string<C>, F>(in, std::forward<F>(f));
}
template <typename C, typename P> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto none_match(const std::basic_string<C> &in, P &&p, tag = {}) {
  return !details::container1::exists<std::basic_string<C>, P>(in, std::forward<P>(p));
}
template <typename C, typename Other>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto intersect(const std::basic_string<C> &in, const Other &other, tag = {}) {
  auto other_ = static_cast<std::basic_string<C>>(other);
  return details::container1::intersect<std::basic_string<C>, std::basic_string<C>, std::basic_string<C>>(in, other_);
}
template <typename C, typename Other>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto diff(const std::basic_string<C> &in, const Other &other, tag = {}) {
  auto other_ = static_cast<std::basic_string<C>>(other);
  return details::container1::diff<std::basic_string<C>, std::basic_string<C>, std::basic_string<C>>(in, other_);
}
template <typename C, typename Acc, typename F>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto scan_left(const std::basic_string<C> &in, Acc init, F &&f, tag = {}) {
  using B = std::decay_t<Acc>;
  return details::sequence1::scan_left<std::basic_string<C>, B, F, std::vector<B>>(in, std::move(init), std::forward<F>(f));
}
template <typename C, typename Acc, typename F>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto scan_right(const std::basic_string<C> &in, Acc init, F &&f, tag = {}) {
  using B = std::decay_t<Acc>;
  return details::sequence1::scan_right<std::basic_string<C>, B, F, std::vector<B>>(in, std::move(init), std::forward<F>(f));
}
template <typename C, typename T>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto last_index_of(const std::basic_string<C> &in, const T &t, tag = {}) {
  return details::sequence1::last_index_of<std::basic_string<C>, T>(in, t);
}
template <typename C, typename P>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto last_index_where(const std::basic_string<C> &in, P &&p, tag = {}) {
  return details::sequence1::last_index_where<std::basic_string<C>, P>(in, std::forward<P>(p));
}

template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto trim_leading(const std::basic_string<C> &in, tag = {}) {
  auto first = std::find_if_not(in.begin(), in.end(), [](auto c) { return details::string_char_ops<C>::is_space(c); });
  return std::basic_string<C>(first, in.end());
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto trim_trailing(const std::basic_string<C> &in, tag = {}) {
  auto last = std::find_if_not(in.rbegin(), in.rend(), [](auto c) { return details::string_char_ops<C>::is_space(c); }).base();
  return std::basic_string<C>(in.begin(), last);
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto trim(const std::basic_string<C> &in, tag = {}) {
  auto first = std::find_if_not(in.begin(), in.end(), [](auto c) { return details::string_char_ops<C>::is_space(c); });
  auto last = std::find_if_not(in.rbegin(), in.rend(), [](auto c) { return details::string_char_ops<C>::is_space(c); }).base();
  return (first <= last) ? std::basic_string<C>(first, last) : std::basic_string<C>();
}
template <typename C> [[nodiscard]] bool is_blank(const std::basic_string<C> &in, tag = {}) {
  return std::all_of(in.begin(), in.end(), [](auto c) { return details::string_char_ops<C>::is_space(c); });
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto to_upper(const std::basic_string<C> &in, tag = {}) {
  std::basic_string<C> out(in.length(), C{});
  std::transform(in.begin(), in.end(), out.begin(), [](auto x) { return details::string_char_ops<C>::to_upper(x); });
  return out;
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto to_lower(const std::basic_string<C> &in, tag = {}) {
  std::basic_string<C> out(in.length(), C{});
  std::transform(in.begin(), in.end(), out.begin(), [](auto x) { return details::string_char_ops<C>::to_lower(x); });
  return out;
}
template <typename C, typename Needle, typename With>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto replace_all(const std::basic_string<C> &in, const Needle &needle, const With &with, tag = {}) {
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
template <typename C, typename NewLine>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto indent(const std::basic_string<C> &in, int n, const NewLine &nl, tag = {}) {
  if (n == 0 || in.empty()) return in;
  auto nl_ = static_cast<std::basic_string<C>>(nl);
  if (nl_.empty()) return n > 0 ? std::basic_string<C>(n, ' ') + in : in;
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
      int drop = 0;
      while (after_nl + drop < in.size() && drop < -n && in[after_nl + drop] == ' ')
        drop++;
      start_pos = after_nl + drop;
    }
  }
  out += in.substr(start_pos);
  return out;
}
template <typename C, typename String>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto starts_with(const std::basic_string<C> &in, const String &that, tag = {}) {
  auto that_ = static_cast<std::basic_string<C>>(that);
  return in.size() >= that_.size() && in.compare(0, that_.size(), that_) == 0;
}
template <typename C, typename String>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto ends_with(const std::basic_string<C> &in, const String &that, tag = {}) {
  auto that_ = static_cast<std::basic_string<C>>(that);
  return in.size() >= that_.size() && in.compare(in.size() - that_.size(), that_.size(), that_) == 0;
}
template <typename C, typename String>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto contains_ignore_case(const std::basic_string<C> &in, const String &that, tag = {}) {
  std::basic_string<C> in_lower = in, that_lower = static_cast<std::basic_string<C>>(that);
  std::transform(in_lower.begin(), in_lower.end(), in_lower.begin(), [](auto x) { return details::string_char_ops<C>::to_lower(x); });
  std::transform(that_lower.begin(), that_lower.end(), that_lower.begin(), [](auto x) { return details::string_char_ops<C>::to_lower(x); });
  return in_lower.find(that_lower) != std::basic_string<C>::npos;
}
template <typename C, typename String>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto equals_ignore_case(const std::basic_string<C> &in, const String &that, tag = {}) {
  auto that_ = static_cast<std::basic_string<C>>(that);
  if (in.size() != that_.size()) return false;
  for (size_t i = 0; i < in.size(); ++i)
    if (details::string_char_ops<C>::to_lower(in[i]) != details::string_char_ops<C>::to_lower(that_[i])) return false;
  return true;
}
template <typename C, typename Delimiter>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto split(const std::basic_string<C> &in, const Delimiter &delimiter, tag = {}) {
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
  } else {
    static_assert(std::is_convertible_v<Delimiter, std::basic_string<C>>, "delimiter must be either a string or a character type of input");
  }
  ys.push_back(in.substr(start));
  return ys;
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto lines(const std::basic_string<C> &in, tag = {}) {
  auto xs = split(in, static_cast<C>('\n'));
  if (!xs.empty() && xs.back().empty()) xs.erase(xs.end() - 1);
  return xs;
}
template <typename C, typename String>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto strip_prefix(const std::basic_string<C> &in, const String &prefix, tag = {}) {
  auto p = static_cast<std::basic_string<C>>(prefix);
  if (in.size() >= p.size() && in.compare(0, p.size(), p) == 0) return in.substr(p.size());
  return std::basic_string<C>{in};
}
template <typename C, typename String>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto strip_suffix(const std::basic_string<C> &in, const String &suffix, tag = {}) {
  auto s = static_cast<std::basic_string<C>>(suffix);
  if (in.size() >= s.size() && in.compare(in.size() - s.size(), s.size(), s) == 0) return in.substr(0, in.size() - s.size());
  return std::basic_string<C>{in};
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto capitalize(const std::basic_string<C> &in, tag = {}) {
  std::basic_string<C> out{in};
  if (!out.empty()) out[0] = details::string_char_ops<C>::to_upper(out[0]);
  return out;
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto uncapitalize(const std::basic_string<C> &in, tag = {}) {
  std::basic_string<C> out{in};
  if (!out.empty()) out[0] = details::string_char_ops<C>::to_lower(out[0]);
  return out;
}
template <typename C, typename Needle, typename With>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto replace_first(const std::basic_string<C> &in, const Needle &needle, const With &with,
                                                           tag = {}) {
  auto needle_ = static_cast<std::basic_string<C>>(needle);
  if (needle_.empty()) return std::basic_string<C>{in};
  auto pos = in.find(needle_);
  if (pos == std::basic_string<C>::npos) return std::basic_string<C>{in};
  std::basic_string<C> out{in};
  out.replace(pos, needle_.length(), static_cast<std::basic_string<C>>(with));
  return out;
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto repeated(const std::basic_string<C> &in, size_t n, tag = {}) {
  std::basic_string<C> out;
  out.reserve(in.size() * n);
  for (size_t i = 0; i < n; ++i)
    out += in;
  return out;
}
template <typename C, typename Fill>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto pad_left(const std::basic_string<C> &in, size_t width, Fill fill, tag = {}) {
  if (in.size() >= width) return std::basic_string<C>{in};
  std::basic_string<C> out(width - in.size(), static_cast<C>(fill));
  out += in;
  return out;
}
template <typename C, typename Fill>
[[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto pad_right(const std::basic_string<C> &in, size_t width, Fill fill, tag = {}) {
  if (in.size() >= width) return std::basic_string<C>{in};
  std::basic_string<C> out{in};
  out.append(width - in.size(), static_cast<C>(fill));
  return out;
}
template <typename C> [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC auto words(const std::basic_string<C> &in, tag = {}) {
  std::vector<std::basic_string<C>> ys;
  auto it = in.begin(), end = in.end();
  while (it != end) {
    while (it != end && details::string_char_ops<C>::is_space(*it))
      ++it;
    auto start = it;
    while (it != end && !details::string_char_ops<C>::is_space(*it))
      ++it;
    if (start != it) ys.emplace_back(start, it);
  }
  return ys;
}

} // namespace aspartame
