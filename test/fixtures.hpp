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
#include <vector>

using std::get, std::string, std::to_string;
namespace {
class Foo {
public:
  explicit Foo(int x) : value(x) {}
  int value;
  bool operator==(const Foo &rhs) const { return value == rhs.value; }
  bool operator<(const Foo &rhs) const { return value < rhs.value; }
};

std::ostream &operator<<(std::ostream &os, const Foo &foo) {
  os << "Foo(" << foo.value << ")";
  return os;
}

} // namespace

namespace std {
template <> struct hash<Foo> {
  size_t operator()(const Foo &foo) const noexcept {
    // Use std::hash to hash the member 'value'
    return std::hash<int>()(foo.value);
  }
};
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
    // addition is commutative so safe for unordered containers
    for (const T &elem : uset)
      seed += std::hash<T>()(elem);
    return seed ^ (std::hash<size_t>()(uset.size()) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
  }
};

} // namespace std

template <typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::list<T> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::optional<T> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::unordered_set<T> &);
template <typename T> std::ostream &operator<<(std::ostream &os, const std::set<T> &);
template <typename K, typename V> std::ostream &operator<<(std::ostream &os, const std::unordered_map<K, V> &);

template <typename... Args> std::ostream &operator<<(std::ostream &os, const std::tuple<Args...> &);
template <typename T1, typename T2> std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &);

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

template <typename T> std::ostream &operator<<(std::ostream &os, const std::unordered_set<T> &xs) {
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

template <typename K, typename V> std::ostream &operator<<(std::ostream &os, const std::unordered_map<K, V> &xs) {
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
