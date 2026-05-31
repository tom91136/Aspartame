#pragma once

#include <map>
#include <set>
#include <vector>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename K, typename V, typename C, typename A> struct enable_pipe<std::map<K, V, C, A>> : std::true_type {};

template <typename K, typename V, typename C, typename A> struct map_traits<std::map<K, V, C, A>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = std::map<K2, V2>;
  template <typename K2> using key_container = std::set<K2>;
  template <typename V2> using value_container = std::vector<V2>;
  template <typename V2> using group_inner = std::set<V2>;
};

template <typename K, typename V, typename C, typename A> struct monoid_traits<std::map<K, V, C, A>> {
  static std::map<K, V, C, A> empty() { return {}; }
  static std::map<K, V, C, A> combine(std::map<K, V, C, A> a, const std::map<K, V, C, A> &b) {
    a.insert(b.begin(), b.end());
    return a;
  }
};

} // namespace aspartame
