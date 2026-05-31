#pragma once

#include <vector>

#include <tsl/bhopscotch_map.h>
#include <tsl/bhopscotch_set.h>
#include <tsl/hopscotch_map.h>
#include <tsl/hopscotch_set.h>
#include <tsl/ordered_map.h>
#include <tsl/ordered_set.h>
#include <tsl/robin_map.h>
#include <tsl/robin_set.h>
#include <tsl/sparse_map.h>
#include <tsl/sparse_set.h>

#include <aspartame/all.hpp>

namespace aspartame {

template <class K, class T, class H, class KE, class A, bool SH, class GP>
struct enable_pipe<tsl::robin_map<K, T, H, KE, A, SH, GP>> : std::true_type {};
template <class K, class T, class H, class KE, class A, bool SH, class GP> struct map_traits<tsl::robin_map<K, T, H, KE, A, SH, GP>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = tsl::robin_map<K2, V2>;
  template <class K2> using key_container = tsl::robin_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = tsl::robin_set<V2>;
};

template <class K, class H, class KE, class A, bool SH, class GP>
struct enable_pipe<tsl::robin_set<K, H, KE, A, SH, GP>> : std::true_type {};
template <class K, class H, class KE, class A, bool SH, class GP> struct sequence_traits<tsl::robin_set<K, H, KE, A, SH, GP>> {
  using value_type = K;
  template <class U> using rebind = tsl::robin_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class T, class H, class KE, class A, class GP, tsl::sh::exception_safety ES, tsl::sh::sparsity SP>
struct enable_pipe<tsl::sparse_map<K, T, H, KE, A, GP, ES, SP>> : std::true_type {};
template <class K, class T, class H, class KE, class A, class GP, tsl::sh::exception_safety ES, tsl::sh::sparsity SP>
struct map_traits<tsl::sparse_map<K, T, H, KE, A, GP, ES, SP>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = tsl::sparse_map<K2, V2>;
  template <class K2> using key_container = tsl::sparse_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = tsl::sparse_set<V2>;
};

template <class K, class H, class KE, class A, class GP, tsl::sh::exception_safety ES, tsl::sh::sparsity SP>
struct enable_pipe<tsl::sparse_set<K, H, KE, A, GP, ES, SP>> : std::true_type {};
template <class K, class H, class KE, class A, class GP, tsl::sh::exception_safety ES, tsl::sh::sparsity SP>
struct sequence_traits<tsl::sparse_set<K, H, KE, A, GP, ES, SP>> {
  using value_type = K;
  template <class U> using rebind = tsl::sparse_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class T, class H, class KE, class A, unsigned int N, bool SH, class GP>
struct enable_pipe<tsl::hopscotch_map<K, T, H, KE, A, N, SH, GP>> : std::true_type {};
template <class K, class T, class H, class KE, class A, unsigned int N, bool SH, class GP>
struct map_traits<tsl::hopscotch_map<K, T, H, KE, A, N, SH, GP>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = tsl::hopscotch_map<K2, V2>;
  template <class K2> using key_container = tsl::hopscotch_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = tsl::hopscotch_set<V2>;
};

template <class K, class H, class KE, class A, unsigned int N, bool SH, class GP>
struct enable_pipe<tsl::hopscotch_set<K, H, KE, A, N, SH, GP>> : std::true_type {};
template <class K, class H, class KE, class A, unsigned int N, bool SH, class GP>
struct sequence_traits<tsl::hopscotch_set<K, H, KE, A, N, SH, GP>> {
  using value_type = K;
  template <class U> using rebind = tsl::hopscotch_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class T, class H, class KE, class C, class A, unsigned int N, bool SH, class GP>
struct enable_pipe<tsl::bhopscotch_map<K, T, H, KE, C, A, N, SH, GP>> : std::true_type {};
template <class K, class T, class H, class KE, class C, class A, unsigned int N, bool SH, class GP>
struct map_traits<tsl::bhopscotch_map<K, T, H, KE, C, A, N, SH, GP>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = tsl::bhopscotch_map<K2, V2>;
  template <class K2> using key_container = tsl::bhopscotch_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = tsl::bhopscotch_set<V2>;
};

template <class K, class H, class KE, class C, class A, unsigned int N, bool SH, class GP>
struct enable_pipe<tsl::bhopscotch_set<K, H, KE, C, A, N, SH, GP>> : std::true_type {};
template <class K, class H, class KE, class C, class A, unsigned int N, bool SH, class GP>
struct sequence_traits<tsl::bhopscotch_set<K, H, KE, C, A, N, SH, GP>> {
  using value_type = K;
  template <class U> using rebind = tsl::bhopscotch_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class T, class H, class KE, class A, class IT, class IB>
struct enable_pipe<tsl::ordered_map<K, T, H, KE, A, IT, IB>> : std::true_type {};
template <class K, class T, class H, class KE, class A, class IT, class IB> struct map_traits<tsl::ordered_map<K, T, H, KE, A, IT, IB>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = tsl::ordered_map<K2, V2>;
  template <class K2> using key_container = tsl::ordered_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = tsl::ordered_set<V2>;
};

template <class K, class H, class KE, class A, class IT, class IB>
struct enable_pipe<tsl::ordered_set<K, H, KE, A, IT, IB>> : std::true_type {};
template <class K, class H, class KE, class A, class IT, class IB> struct sequence_traits<tsl::ordered_set<K, H, KE, A, IT, IB>> {
  using value_type = K;
  template <class U> using rebind = tsl::ordered_set<U>;
  static constexpr bool set_like = true;
};

} // namespace aspartame
