#pragma once

#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

#if __has_include(<version>)
  #include <version>
#endif
#if defined(__cpp_lib_expected)
  #include <expected>
#endif

using std::get, std::string, std::to_string;
class Foo {
public:
  explicit Foo(int x) : value(x) { static_assert(!std::is_default_constructible_v<Foo>); }
  Foo() = delete;
  int value;
  bool operator==(const Foo &rhs) const { return value == rhs.value; }
  bool operator!=(const Foo &rhs) const { return value != rhs.value; }
  bool operator<(const Foo &rhs) const { return value < rhs.value; }
};

inline std::ostream &operator<<(std::ostream &os, const Foo &foo) {
  os << "Foo(" << foo.value << ")";
  return os;
}

// hash_value via ADL so boost::hash (used by boost::unordered, abseil, ...) finds it.
inline std::size_t hash_value(const Foo &foo) { return std::hash<int>{}(foo.value); }

struct MyString {
  std::string value;
  MyString(std::string value) : value(std::move(value)) {} // NOLINT(*-explicit-constructor)
  MyString() = delete;
  bool operator==(const MyString &rhs) const { return value == rhs.value; }
  bool operator!=(const MyString &rhs) const { return value != rhs.value; }
  bool operator<(const MyString &rhs) const { return value < rhs.value; }
};

inline std::ostream &operator<<(std::ostream &os, const MyString &foo) {
  os << "MyString(" << foo.value << ")";
  return os;
}

inline std::size_t hash_value(const MyString &s) { return std::hash<std::string>{}(s.value); }

struct MyFoo {
  Foo value;
  MyFoo(const Foo &value) : value(value) {} // NOLINT(*-explicit-constructor)
  MyFoo() = delete;
  bool operator==(const MyFoo &rhs) const { return value == rhs.value; }
  bool operator!=(const MyFoo &rhs) const { return value != rhs.value; }
  bool operator<(const MyFoo &rhs) const { return value < rhs.value; }
};

inline std::ostream &operator<<(std::ostream &os, const MyFoo &foo) {
  os << "MyFoo(" << foo.value << ")";
  return os;
}

inline std::size_t hash_value(const MyFoo &f) { return hash_value(f.value); }

namespace std {
template <> struct hash<Foo> {
  size_t operator()(const Foo &foo) const noexcept { return std::hash<int>()(foo.value); }
};

template <> struct hash<MyString> {
  size_t operator()(const MyString &foo) const noexcept { return std::hash<std::string>()(foo.value); }
};

template <> struct hash<MyFoo> {
  size_t operator()(const MyFoo &foo) const noexcept { return std::hash<Foo>()(foo.value); }
};

// UB-by-spec ([namespace.std]): specialising std::hash for std types
// (pair, tuple, unordered_set) is undefined behaviour. All major standard
// libraries accept these specialisations. Test-only: a clean fix requires a
// non-std Hasher plumbed through every TPE_CTOR_IN, but a templated-call-op
// hasher hits a libstdc++-14 init-list dedup bug under -fsanitize, so the
// quarantine stays.
template <class Tuple, std::size_t Index = std::tuple_size<Tuple>::value - 1> struct HashValueImpl {
  static void apply(size_t &seed, const Tuple &tuple) {
    HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
    seed ^= std::hash<typename std::tuple_element<Index, Tuple>::type>()(std::get<Index>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
};
template <class Tuple> struct HashValueImpl<Tuple, 0> {
  static void apply(size_t &seed, const Tuple &tuple) {
    seed ^= std::hash<typename std::tuple_element<0, Tuple>::type>()(std::get<0>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
};
template <typename... TT> struct hash<tuple<TT...>> {
  size_t operator()(const tuple<TT...> &tt) const noexcept {
    size_t seed = 0;
    HashValueImpl<tuple<TT...>>::apply(seed, tt);
    return seed;
  }
};
template <typename T1, typename T2> struct hash<pair<T1, T2>> {
  size_t operator()(const pair<T1, T2> &p) const noexcept {
    size_t h1 = std::hash<T1>()(p.first);
    size_t h2 = std::hash<T2>()(p.second);
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};
template <typename T> struct hash<unordered_set<T>> {
  size_t operator()(const unordered_set<T> &uset) const noexcept {
    size_t seed = 0;
    for (const T &elem : uset)
      seed += std::hash<T>()(elem);
    return seed ^ (std::hash<size_t>()(uset.size()) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
  }
};

} // namespace std

template <typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::list<T> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::deque<T> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::optional<T> &);
template <typename T, typename... R> std::ostream &operator<<(std::ostream &os, const std::unordered_set<T, R...> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::set<T> &);
template <typename K, typename V, typename... R> std::ostream &operator<<(std::ostream &os, const std::unordered_map<K, V, R...> &);

template <typename... Args> std::ostream &operator<<(std::ostream &os, const std::tuple<Args...> &);
template <typename T1, typename T2> std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &);
#if defined(__cpp_lib_expected)
template <typename V, typename E> std::ostream &operator<<(std::ostream &os, const std::expected<V, E> &);
#endif

template <typename T> std::ostream &operator<<(std::ostream &os, const std::optional<T> &xs) {
  if (!xs) os << "std::nullopt";
  else os << "std::optional{" << *xs << "}";
  return os;
}

template <typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &xs) {
  os << "std::vector{";
  for (size_t i = 0; i < xs.size(); ++i) {
    if (i > 0) os << ", ";
    os << xs[i];
  }
  os << "}";
  return os;
}

template <typename T> std::ostream &operator<<(std::ostream &os, const std::deque<T> &xs) {
  os << "std::deque{";
  for (size_t i = 0; i < xs.size(); ++i) {
    if (i > 0) os << ", ";
    os << xs[i];
  }
  os << "}";
  return os;
}

template <typename T, typename... R> std::ostream &operator<<(std::ostream &os, const std::unordered_set<T, R...> &xs) {
  os << "std::unordered_set{";
  for (auto it = xs.begin(); it != xs.end(); ++it) {
    if (it != xs.begin()) os << ", ";
    os << *it;
  }
  os << "}";
  return os;
}

template <typename T> std::ostream &operator<<(std::ostream &os, const std::set<T> &xs) {
  os << "std::set{";
  for (auto it = xs.begin(); it != xs.end(); ++it) {
    if (it != xs.begin()) os << ", ";
    os << *it;
  }
  os << "}";
  return os;
}

template <typename T> std::ostream &operator<<(std::ostream &os, const std::list<T> &xs) {
  os << "std::list{";
  for (auto it = xs.begin(); it != xs.end(); ++it) {
    if (it != xs.begin()) os << ", ";
    os << *it;
  }
  os << "}";
  return os;
}

template <typename K, typename V> std::ostream &operator<<(std::ostream &os, const std::map<K, V> &xs) {
  os << "std::map{";
  for (auto it = xs.begin(); it != xs.end(); ++it) {
    if (it != xs.begin()) os << ", ";
    os << "{" << it->first << ", " << it->second << "}";
  }
  os << "}";
  return os;
}

template <typename K, typename V, typename... R> std::ostream &operator<<(std::ostream &os, const std::unordered_map<K, V, R...> &xs) {
  os << "std::unordered_map{";
  for (auto it = xs.begin(); it != xs.end(); ++it) {
    if (it != xs.begin()) os << ", ";
    os << "{" << it->first << ", " << it->second << "}";
  }
  os << "}";
  return os;
}

template <typename T1, typename T2> std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &xs) {
  os << "std::pair{" << xs.first << ", " << xs.second << "}";
  return os;
}

template <typename... Args> std::ostream &operator<<(std::ostream &os, const std::tuple<Args...> &xs) {
  os << "std::tuple{";
  std::apply([&os](const Args &...tupleArgs) { ((os << tupleArgs << ", "), ...); }, xs);
  return os << "\b\b}"; // \b\b is used to remove the last comma and space
}

template <typename... Ts> std::ostream &operator<<(std::ostream &os, const std::variant<Ts...> &v) {
  os << "std::variant{";
  std::visit([&os](const auto &value) { os << value; }, v);
  os << "}";
  return os;
}

#if defined(__cpp_lib_expected)
template <typename V, typename E> inline std::ostream &operator<<(std::ostream &os, const std::expected<V, E> &e) {
  if (e.has_value()) return os << "std::expected{" << *e << "}";
  return os << "std::unexpect{" << e.error() << "}";
}
#endif
