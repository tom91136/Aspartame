#pragma once

#include <cstddef>
#include <vector>

#include <absl/container/btree_map.h>
#include <absl/container/btree_set.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/container/inlined_vector.h>
#include <absl/container/node_hash_map.h>
#include <absl/container/node_hash_set.h>

#include <aspartame/all.hpp>

namespace aspartame {

template <class T, std::size_t N, class A> struct enable_pipe<absl::InlinedVector<T, N, A>> : std::true_type {};
template <class T, std::size_t N, class A> struct sequence_traits<absl::InlinedVector<T, N, A>> {
  using value_type = T;
  template <class U> using rebind = absl::InlinedVector<U, N>;
  static constexpr bool set_like = false;
};

template <class T, class H, class E, class A> struct enable_pipe<absl::flat_hash_set<T, H, E, A>> : std::true_type {};
template <class T, class H, class E, class A> struct sequence_traits<absl::flat_hash_set<T, H, E, A>> {
  using value_type = T;
  template <class U> using rebind = absl::flat_hash_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class C, class A> struct enable_pipe<absl::btree_set<K, C, A>> : std::true_type {};
template <class K, class C, class A> struct sequence_traits<absl::btree_set<K, C, A>> {
  using value_type = K;
  template <class U> using rebind = absl::btree_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class V, class H, class E, class A> struct enable_pipe<absl::flat_hash_map<K, V, H, E, A>> : std::true_type {};
template <class K, class V, class H, class E, class A> struct map_traits<absl::flat_hash_map<K, V, H, E, A>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = absl::flat_hash_map<K2, V2>;
  template <class K2> using key_container = absl::flat_hash_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = absl::flat_hash_set<V2>;
};

template <class K, class V, class C, class A> struct enable_pipe<absl::btree_map<K, V, C, A>> : std::true_type {};
template <class K, class V, class C, class A> struct map_traits<absl::btree_map<K, V, C, A>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = absl::btree_map<K2, V2>;
  template <class K2> using key_container = absl::btree_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = absl::btree_set<V2>;
};

template <class T, class H, class E, class A> struct enable_pipe<absl::node_hash_set<T, H, E, A>> : std::true_type {};
template <class T, class H, class E, class A> struct sequence_traits<absl::node_hash_set<T, H, E, A>> {
  using value_type = T;
  template <class U> using rebind = absl::node_hash_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class V, class H, class E, class A> struct enable_pipe<absl::node_hash_map<K, V, H, E, A>> : std::true_type {};
template <class K, class V, class H, class E, class A> struct map_traits<absl::node_hash_map<K, V, H, E, A>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = absl::node_hash_map<K2, V2>;
  template <class K2> using key_container = absl::node_hash_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = absl::node_hash_set<V2>;
};

} // namespace aspartame
