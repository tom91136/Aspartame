#pragma once

#include <cstddef>
#include <vector>

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/optional.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <aspartame/all.hpp>

namespace aspartame {

template <class T, std::size_t N, class A, class O> struct enable_pipe<boost::container::small_vector<T, N, A, O>> : std::true_type {};
template <class T, std::size_t N, class A, class O> struct sequence_traits<boost::container::small_vector<T, N, A, O>> {
  using value_type = T;
  template <class U> using rebind = boost::container::small_vector<U, N>;
  static constexpr bool set_like = false;
};

template <class K, class C, class AC> struct enable_pipe<boost::container::flat_set<K, C, AC>> : std::true_type {};
template <class K, class C, class AC> struct sequence_traits<boost::container::flat_set<K, C, AC>> {
  using value_type = K;
  template <class U> using rebind = boost::container::flat_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class T, class C, class AC> struct enable_pipe<boost::container::flat_map<K, T, C, AC>> : std::true_type {};
template <class K, class T, class C, class AC> struct map_traits<boost::container::flat_map<K, T, C, AC>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = boost::container::flat_map<K2, V2>;
  template <class K2> using key_container = boost::container::flat_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = boost::container::flat_set<V2>;
};

template <class K, class H, class KE, class A> struct enable_pipe<boost::unordered_flat_set<K, H, KE, A>> : std::true_type {};
template <class K, class H, class KE, class A> struct sequence_traits<boost::unordered_flat_set<K, H, KE, A>> {
  using value_type = K;
  template <class U> using rebind = boost::unordered_flat_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class T, class H, class KE, class A> struct enable_pipe<boost::unordered_flat_map<K, T, H, KE, A>> : std::true_type {};
template <class K, class T, class H, class KE, class A> struct map_traits<boost::unordered_flat_map<K, T, H, KE, A>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = boost::unordered_flat_map<K2, V2>;
  template <class K2> using key_container = boost::unordered_flat_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = boost::unordered_flat_set<V2>;
};

template <class T> struct optional_traits<boost::optional<T>> {
  using value_type = T;
  template <class U> using rebind = boost::optional<U>;
  static bool engaged(const boost::optional<T> &o) noexcept { return static_cast<bool>(o); }
  static const T &get(const boost::optional<T> &o) noexcept { return *o; }
};

template <class K, class H, class KE, class A> struct enable_pipe<boost::unordered_set<K, H, KE, A>> : std::true_type {};
template <class K, class H, class KE, class A> struct sequence_traits<boost::unordered_set<K, H, KE, A>> {
  using value_type = K;
  template <class U> using rebind = boost::unordered_set<U>;
  static constexpr bool set_like = true;
};

template <class K, class T, class H, class KE, class A> struct enable_pipe<boost::unordered_map<K, T, H, KE, A>> : std::true_type {};
template <class K, class T, class H, class KE, class A> struct map_traits<boost::unordered_map<K, T, H, KE, A>> {
  using key_type = K;
  using mapped_type = T;
  template <class K2, class V2> using rebind = boost::unordered_map<K2, V2>;
  template <class K2> using key_container = boost::unordered_set<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = boost::unordered_set<V2>;
};

} // namespace aspartame
