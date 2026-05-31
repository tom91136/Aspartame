#pragma once

#include <vector>

#include <parallel_hashmap/phmap.h>

#include <aspartame/all.hpp>

namespace aspartame {

template <class T, class H, class E, class A> struct enable_pipe<phmap::flat_hash_set<T, H, E, A>> : std::true_type {};
template <class T, class H, class E, class A> struct sequence_traits<phmap::flat_hash_set<T, H, E, A>> {
  using value_type = T;
  template <class U> using rebind = phmap::flat_hash_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class V, class H, class E, class A> struct enable_pipe<phmap::flat_hash_map<K, V, H, E, A>> : std::true_type {};
template <class K, class V, class H, class E, class A> struct map_traits<phmap::flat_hash_map<K, V, H, E, A>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = phmap::flat_hash_map<K2, V2>;
  template <class K2> using key_container = phmap::flat_hash_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = phmap::flat_hash_set<V2>;
};

template <class T, class H, class E, class A> struct enable_pipe<phmap::node_hash_set<T, H, E, A>> : std::true_type {};
template <class T, class H, class E, class A> struct sequence_traits<phmap::node_hash_set<T, H, E, A>> {
  using value_type = T;
  template <class U> using rebind = phmap::node_hash_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class V, class H, class E, class A> struct enable_pipe<phmap::node_hash_map<K, V, H, E, A>> : std::true_type {};
template <class K, class V, class H, class E, class A> struct map_traits<phmap::node_hash_map<K, V, H, E, A>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = phmap::node_hash_map<K2, V2>;
  template <class K2> using key_container = phmap::node_hash_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = phmap::node_hash_set<V2>;
};

template <class T, class H, class E, class A, std::size_t N, class M>
struct enable_pipe<phmap::parallel_flat_hash_set<T, H, E, A, N, M>> : std::true_type {};
template <class T, class H, class E, class A, std::size_t N, class M>
struct sequence_traits<phmap::parallel_flat_hash_set<T, H, E, A, N, M>> {
  using value_type = T;
  template <class U> using rebind = phmap::parallel_flat_hash_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class V, class H, class E, class A, std::size_t N, class M>
struct enable_pipe<phmap::parallel_flat_hash_map<K, V, H, E, A, N, M>> : std::true_type {};
template <class K, class V, class H, class E, class A, std::size_t N, class M>
struct map_traits<phmap::parallel_flat_hash_map<K, V, H, E, A, N, M>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = phmap::parallel_flat_hash_map<K2, V2>;
  template <class K2> using key_container = phmap::parallel_flat_hash_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = phmap::parallel_flat_hash_set<V2>;
};

template <class T, class H, class E, class A, std::size_t N, class M>
struct enable_pipe<phmap::parallel_node_hash_set<T, H, E, A, N, M>> : std::true_type {};
template <class T, class H, class E, class A, std::size_t N, class M>
struct sequence_traits<phmap::parallel_node_hash_set<T, H, E, A, N, M>> {
  using value_type = T;
  template <class U> using rebind = phmap::parallel_node_hash_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class V, class H, class E, class A, std::size_t N, class M>
struct enable_pipe<phmap::parallel_node_hash_map<K, V, H, E, A, N, M>> : std::true_type {};
template <class K, class V, class H, class E, class A, std::size_t N, class M>
struct map_traits<phmap::parallel_node_hash_map<K, V, H, E, A, N, M>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = phmap::parallel_node_hash_map<K2, V2>;
  template <class K2> using key_container = phmap::parallel_node_hash_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = phmap::parallel_node_hash_set<V2>;
};

template <class K, class C, class A> struct enable_pipe<phmap::btree_set<K, C, A>> : std::true_type {};
template <class K, class C, class A> struct sequence_traits<phmap::btree_set<K, C, A>> {
  using value_type = K;
  template <class U> using rebind = phmap::btree_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class V, class C, class A> struct enable_pipe<phmap::btree_map<K, V, C, A>> : std::true_type {};
template <class K, class V, class C, class A> struct map_traits<phmap::btree_map<K, V, C, A>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = phmap::btree_map<K2, V2>;
  template <class K2> using key_container = phmap::btree_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = phmap::btree_set<V2>;
};

} // namespace aspartame
