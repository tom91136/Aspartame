#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "string.hpp"

namespace aspartame {

template <typename C, typename Traits> struct enable_pipe<std::basic_string_view<C, Traits>> : std::true_type {};
template <typename C, typename Traits> struct enable_string_ops<std::basic_string_view<C, Traits>> : std::true_type {};

template <typename C, typename Traits> struct sequence_traits<std::basic_string_view<C, Traits>> {
  using value_type = C;
  template <typename U>
  using rebind = std::conditional_t<std::is_same_v<std::decay_t<U>, C>, std::basic_string<C, Traits>, std::vector<std::decay_t<U>>>;
  static constexpr bool set_like = false;
};

template <typename C, typename Traits> [[nodiscard]] constexpr auto trim_leading(std::basic_string_view<C, Traits> in, tag = {}) {
  size_t pos = 0;
  while (pos < in.size() && details::string_char_ops<C>::is_space(in[pos]))
    ++pos;
  return in.substr(pos);
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto trim_trailing(std::basic_string_view<C, Traits> in, tag = {}) {
  size_t pos = in.size();
  while (pos && details::string_char_ops<C>::is_space(in[pos - 1]))
    --pos;
  return in.substr(0, pos);
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto trim(std::basic_string_view<C, Traits> in, tag = {}) {
  return trim_trailing(trim_leading(in));
}
template <typename C, typename Traits> [[nodiscard]] constexpr bool is_blank(std::basic_string_view<C, Traits> in, tag = {}) {
  return trim(in).empty();
}

template <typename C, typename Traits>
[[nodiscard]] constexpr auto slice(std::basic_string_view<C, Traits> in, size_t from, size_t to, tag = {}) {
  from = std::min(from, in.size());
  to = std::min(std::max(to, from), in.size());
  return in.substr(from, to - from);
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto take(std::basic_string_view<C, Traits> in, size_t n, tag = {}) {
  return in.substr(0, std::min(n, in.size()));
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto drop(std::basic_string_view<C, Traits> in, size_t n, tag = {}) {
  return in.substr(std::min(n, in.size()));
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto take_right(std::basic_string_view<C, Traits> in, size_t n, tag = {}) {
  return in.substr(in.size() - std::min(n, in.size()));
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto drop_right(std::basic_string_view<C, Traits> in, size_t n, tag = {}) {
  return in.substr(0, in.size() - std::min(n, in.size()));
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto init(std::basic_string_view<C, Traits> in, tag = {}) {
  return in.empty() ? in : in.substr(0, in.size() - 1);
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto tail(std::basic_string_view<C, Traits> in, tag = {}) {
  return in.empty() ? in : in.substr(1);
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto split_at(std::basic_string_view<C, Traits> in, size_t idx, tag = {}) {
  idx = std::min(idx, in.size());
  return std::pair{in.substr(0, idx), in.substr(idx)};
}
template <typename C, typename Traits, typename P>
[[nodiscard]] constexpr auto take_while(std::basic_string_view<C, Traits> in, P &&p, tag = {}) {
  size_t pos = 0;
  while (pos < in.size() && details::ap(p, in[pos]))
    ++pos;
  return in.substr(0, pos);
}
template <typename C, typename Traits, typename P>
[[nodiscard]] constexpr auto drop_while(std::basic_string_view<C, Traits> in, P &&p, tag = {}) {
  size_t pos = 0;
  while (pos < in.size() && details::ap(p, in[pos]))
    ++pos;
  return in.substr(pos);
}
template <typename C, typename Traits, typename P>
[[nodiscard]] constexpr auto span(std::basic_string_view<C, Traits> in, P &&p, tag = {}) {
  size_t pos = 0;
  while (pos < in.size() && details::ap(p, in[pos]))
    ++pos;
  return std::pair{in.substr(0, pos), in.substr(pos)};
}

template <typename C, typename Traits, typename String>
[[nodiscard]] constexpr bool starts_with(std::basic_string_view<C, Traits> in, const String &that, tag = {}) {
  const std::basic_string_view<C, Traits> value(that);
  return in.size() >= value.size() && in.compare(0, value.size(), value) == 0;
}
template <typename C, typename Traits, typename String>
[[nodiscard]] constexpr bool ends_with(std::basic_string_view<C, Traits> in, const String &that, tag = {}) {
  const std::basic_string_view<C, Traits> value(that);
  return in.size() >= value.size() && in.compare(in.size() - value.size(), value.size(), value) == 0;
}
template <typename C, typename Traits, typename String>
[[nodiscard]] constexpr auto strip_prefix(std::basic_string_view<C, Traits> in, const String &prefix, tag = {}) {
  const std::basic_string_view<C, Traits> p(prefix);
  return starts_with(in, p) ? in.substr(p.size()) : in;
}
template <typename C, typename Traits, typename String>
[[nodiscard]] constexpr auto strip_suffix(std::basic_string_view<C, Traits> in, const String &suffix, tag = {}) {
  const std::basic_string_view<C, Traits> s(suffix);
  return ends_with(in, s) ? in.substr(0, in.size() - s.size()) : in;
}

template <typename C, typename Traits, typename Delimiter>
[[nodiscard]] constexpr auto split(std::basic_string_view<C, Traits> in, const Delimiter &delimiter, tag = {}) {
  using StringView = std::basic_string_view<C, Traits>;
  std::vector<std::basic_string_view<C, Traits>> result;
  size_t start = 0, pos = 0;
  if constexpr (std::is_convertible_v<Delimiter, StringView>) {
    const StringView d(delimiter);
    if (d.empty()) return std::vector<std::basic_string_view<C, Traits>>{in};
    while ((pos = in.find(d, start)) != std::basic_string_view<C, Traits>::npos) {
      result.push_back(in.substr(start, pos - start));
      start = pos + d.size();
    }
  } else if constexpr (std::is_same_v<std::decay_t<Delimiter>, C>) {
    while ((pos = in.find(delimiter, start)) != std::basic_string_view<C, Traits>::npos) {
      result.push_back(in.substr(start, pos - start));
      start = pos + 1;
    }
  } else {
    static_assert(std::is_convertible_v<Delimiter, StringView>, "delimiter must be a compatible string or character");
  }
  result.push_back(in.substr(start));
  return result;
}
template <typename C, typename Traits, typename Delimiter>
[[nodiscard]] constexpr auto split_once(std::basic_string_view<C, Traits> in, const Delimiter &delimiter, tag = {}) {
  using StringView = std::basic_string_view<C, Traits>;
  size_t pos = 0, width = 0;
  if constexpr (std::is_convertible_v<Delimiter, StringView>) {
    const StringView d(delimiter);
    if (d.empty() || (pos = in.find(d)) == std::basic_string_view<C, Traits>::npos)
      return std::optional<std::pair<std::basic_string_view<C, Traits>, std::basic_string_view<C, Traits>>>{};
    width = d.size();
  } else if constexpr (std::is_same_v<std::decay_t<Delimiter>, C>) {
    if ((pos = in.find(delimiter)) == std::basic_string_view<C, Traits>::npos)
      return std::optional<std::pair<std::basic_string_view<C, Traits>, std::basic_string_view<C, Traits>>>{};
    width = 1;
  } else {
    static_assert(std::is_convertible_v<Delimiter, StringView>, "delimiter must be a compatible string or character");
  }
  return std::optional{std::pair{in.substr(0, pos), in.substr(pos + width)}};
}
template <typename C, typename Traits, typename Delimiter>
[[nodiscard]] constexpr auto rsplit_once(std::basic_string_view<C, Traits> in, const Delimiter &delimiter, tag = {}) {
  using StringView = std::basic_string_view<C, Traits>;
  size_t pos = 0, width = 0;
  if constexpr (std::is_convertible_v<Delimiter, StringView>) {
    const StringView d(delimiter);
    if (d.empty() || (pos = in.rfind(d)) == std::basic_string_view<C, Traits>::npos)
      return std::optional<std::pair<std::basic_string_view<C, Traits>, std::basic_string_view<C, Traits>>>{};
    width = d.size();
  } else if constexpr (std::is_same_v<std::decay_t<Delimiter>, C>) {
    if ((pos = in.rfind(delimiter)) == std::basic_string_view<C, Traits>::npos)
      return std::optional<std::pair<std::basic_string_view<C, Traits>, std::basic_string_view<C, Traits>>>{};
    width = 1;
  } else {
    static_assert(std::is_convertible_v<Delimiter, StringView>, "delimiter must be a compatible string or character");
  }
  return std::optional{std::pair{in.substr(0, pos), in.substr(pos + width)}};
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto lines(std::basic_string_view<C, Traits> in, tag = {}) {
  auto result = split(in, static_cast<C>('\n'));
  if (!result.empty() && result.back().empty()) result.pop_back();
  return result;
}
template <typename C, typename Traits> [[nodiscard]] constexpr auto words(std::basic_string_view<C, Traits> in, tag = {}) {
  std::vector<std::basic_string_view<C, Traits>> result;
  size_t start = 0;
  while (start < in.size()) {
    while (start < in.size() && details::string_char_ops<C>::is_space(in[start]))
      ++start;
    auto end = start;
    while (end < in.size() && !details::string_char_ops<C>::is_space(in[end]))
      ++end;
    if (start != end) result.push_back(in.substr(start, end - start));
    start = end;
  }
  return result;
}

template <typename C, typename Traits, typename Pattern>
[[nodiscard]] constexpr bool glob_matches(std::basic_string_view<C, Traits> in, const Pattern &pattern, tag = {}) {
  const std::basic_string_view<C, Traits> p(pattern);
  return details::glob_matches_by(in, p, [](C a, C b) { return a == b; });
}
template <typename C, typename Traits, typename Pattern>
[[nodiscard]] constexpr bool glob_matches_ignore_case(std::basic_string_view<C, Traits> in, const Pattern &pattern, tag = {}) {
  const std::basic_string_view<C, Traits> p(pattern);
  return details::glob_matches_by(
      in, p, [](C a, C b) { return details::string_char_ops<C>::to_lower(a) == details::string_char_ops<C>::to_lower(b); });
}

template <typename C, typename Traits, typename Needle, typename With>
[[nodiscard]] auto replace_all(std::basic_string_view<C, Traits> in, const Needle &needle, const With &with, tag = {}) {
  const std::basic_string_view<C, Traits> n(needle);
  const std::basic_string_view<C, Traits> w(with);
  if (n.empty()) return std::basic_string<C, Traits>(in.begin(), in.end());
  std::basic_string<C, Traits> out;
  size_t start = 0, pos = 0;
  while ((pos = in.find(n, start)) != std::basic_string_view<C, Traits>::npos) {
    out.append(in.begin() + static_cast<std::ptrdiff_t>(start), in.begin() + static_cast<std::ptrdiff_t>(pos));
    out.append(w.begin(), w.end());
    start = pos + n.size();
  }
  out.append(in.begin() + static_cast<std::ptrdiff_t>(start), in.end());
  return out;
}
template <typename C, typename Traits, typename Needle, typename With>
[[nodiscard]] auto replace_first(std::basic_string_view<C, Traits> in, const Needle &needle, const With &with, tag = {}) {
  const std::basic_string_view<C, Traits> n(needle);
  const std::basic_string_view<C, Traits> w(with);
  std::basic_string<C, Traits> out(in.begin(), in.end());
  if (n.empty()) return out;
  const auto pos = in.find(n);
  if (pos != std::basic_string_view<C, Traits>::npos) out.replace(pos, n.size(), w.data(), w.size());
  return out;
}
template <typename C, typename Traits, typename NewLine>
[[nodiscard]] auto indent(std::basic_string_view<C, Traits> in, int n, const NewLine &new_line, tag = {}) {
  const std::basic_string_view<C, Traits> nl(new_line);
  if (n == 0 || in.empty()) return std::basic_string<C, Traits>(in.begin(), in.end());
  const auto unindent = n < 0 ? static_cast<size_t>(-static_cast<long long>(n)) : 0;
  if (nl.empty()) {
    if (n <= 0) return std::basic_string<C, Traits>(in.begin(), in.end());
    return std::basic_string<C, Traits>(static_cast<size_t>(n), static_cast<C>(' ')) + std::basic_string<C, Traits>(in.begin(), in.end());
  }
  const std::basic_string<C, Traits> prefix(n > 0 ? static_cast<size_t>(n) : 0, static_cast<C>(' '));
  std::basic_string<C, Traits> out(prefix);
  size_t start = 0, pos = 0;
  if (n < 0)
    while (start < in.size() && start < unindent && in[start] == static_cast<C>(' '))
      ++start;
  while ((pos = in.find(nl, start)) != std::basic_string_view<C, Traits>::npos) {
    const size_t after_nl = pos + nl.size();
    out.append(in.begin() + static_cast<std::ptrdiff_t>(start), in.begin() + static_cast<std::ptrdiff_t>(after_nl));
    if (n > 0) {
      if (after_nl != in.size()) out += prefix;
      start = after_nl;
    } else {
      size_t drop = 0;
      while (after_nl + drop < in.size() && drop < unindent && in[after_nl + drop] == static_cast<C>(' '))
        ++drop;
      start = after_nl + drop;
    }
  }
  out.append(in.begin() + static_cast<std::ptrdiff_t>(start), in.end());
  return out;
}
template <typename C, typename Traits> [[nodiscard]] auto repeated(std::basic_string_view<C, Traits> in, size_t n, tag = {}) {
  std::basic_string<C, Traits> out;
  out.reserve(in.size() * n);
  for (size_t i = 0; i < n; ++i)
    out.append(in.begin(), in.end());
  return out;
}
template <typename C, typename Traits, typename Fill>
[[nodiscard]] auto pad_left(std::basic_string_view<C, Traits> in, size_t width, Fill fill, tag = {}) {
  if (in.size() >= width) return std::basic_string<C, Traits>(in.begin(), in.end());
  std::basic_string<C, Traits> out(width - in.size(), static_cast<C>(fill));
  out.append(in.begin(), in.end());
  return out;
}
template <typename C, typename Traits, typename Fill>
[[nodiscard]] auto pad_right(std::basic_string_view<C, Traits> in, size_t width, Fill fill, tag = {}) {
  std::basic_string<C, Traits> out(in.begin(), in.end());
  if (out.size() < width) out.append(width - out.size(), static_cast<C>(fill));
  return out;
}

template <typename C, typename Traits> [[nodiscard]] auto to_upper(std::basic_string_view<C, Traits> in, tag = {}) {
  std::basic_string<C, Traits> out(in.begin(), in.end());
  std::transform(out.begin(), out.end(), out.begin(), [](auto x) { return details::string_char_ops<C>::to_upper(x); });
  return out;
}
template <typename C, typename Traits> [[nodiscard]] auto to_lower(std::basic_string_view<C, Traits> in, tag = {}) {
  std::basic_string<C, Traits> out(in.begin(), in.end());
  std::transform(out.begin(), out.end(), out.begin(), [](auto x) { return details::string_char_ops<C>::to_lower(x); });
  return out;
}
template <typename C, typename Traits> [[nodiscard]] auto capitalize(std::basic_string_view<C, Traits> in, tag = {}) {
  std::basic_string<C, Traits> out(in.begin(), in.end());
  if (!out.empty()) out[0] = details::string_char_ops<C>::to_upper(out[0]);
  return out;
}
template <typename C, typename Traits> [[nodiscard]] auto uncapitalize(std::basic_string_view<C, Traits> in, tag = {}) {
  std::basic_string<C, Traits> out(in.begin(), in.end());
  if (!out.empty()) out[0] = details::string_char_ops<C>::to_lower(out[0]);
  return out;
}
template <typename C, typename Traits, typename String>
[[nodiscard]] auto contains_ignore_case(std::basic_string_view<C, Traits> in, const String &that, tag = {}) {
  const std::basic_string_view<C, Traits> value(that);
  if (value.size() > in.size()) return false;
  for (size_t i = 0; i + value.size() <= in.size(); ++i) {
    size_t j = 0;
    while (j < value.size() && details::string_char_ops<C>::to_lower(in[i + j]) == details::string_char_ops<C>::to_lower(value[j]))
      ++j;
    if (j == value.size()) return true;
  }
  return false;
}
template <typename C, typename Traits, typename String>
[[nodiscard]] auto equals_ignore_case(std::basic_string_view<C, Traits> in, const String &that, tag = {}) {
  const std::basic_string_view<C, Traits> value(that);
  if (value.size() != in.size()) return false;
  for (size_t i = 0; i < in.size(); ++i)
    if (details::string_char_ops<C>::to_lower(in[i]) != details::string_char_ops<C>::to_lower(value[i])) return false;
  return true;
}

} // namespace aspartame
