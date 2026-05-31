#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "fluent.hpp"
#include "traits.hpp"

namespace aspartame {

template <typename K, typename V, typename H, typename E, typename A>
struct enable_pipe<std::unordered_map<K, V, H, E, A>> : std::true_type {};

template <typename K, typename V, typename H, typename E, typename A> struct map_traits<std::unordered_map<K, V, H, E, A>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = std::unordered_map<K2, V2>;
  template <typename K2> using key_container = std::unordered_set<K2>;
  template <typename V2> using value_container = std::vector<V2>;
  template <typename V2> using group_inner = std::unordered_set<V2>;
};

template <typename K, typename V, typename H, typename E, typename A> struct monoid_traits<std::unordered_map<K, V, H, E, A>> {
  static std::unordered_map<K, V, H, E, A> empty() { return {}; }
  static std::unordered_map<K, V, H, E, A> combine(std::unordered_map<K, V, H, E, A> a, const std::unordered_map<K, V, H, E, A> &b) {
    a.insert(b.begin(), b.end());
    return a;
  }
};

} // namespace aspartame
