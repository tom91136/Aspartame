#pragma once

#include <iterator>
#include <optional>
#include <type_traits>
#include <utility>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/StringSet.h>
#include <llvm/ADT/iterator_range.h>

#include <aspartame/all.hpp>

namespace aspartame {

template <typename T> struct enable_pipe<llvm::SmallVectorImpl<T>> : std::true_type {};
template <typename T> struct sequence_traits<llvm::SmallVectorImpl<T>> {
  using value_type = T;
  template <typename U> using rebind = llvm::SmallVector<U>;
  static constexpr bool set_like = false;
};

template <typename T, unsigned N> struct enable_pipe<llvm::SmallVector<T, N>> : std::true_type {};
template <typename T, unsigned N> struct sequence_traits<llvm::SmallVector<T, N>> {
  using value_type = T;
  template <typename U> using rebind = llvm::SmallVector<U, N>;
  static constexpr bool set_like = false;
};

template <typename T, typename Info> struct enable_pipe<llvm::DenseSet<T, Info>> : std::true_type {};
template <typename T, typename Info> struct sequence_traits<llvm::DenseSet<T, Info>> {
  using value_type = T;
  template <typename U> using rebind = llvm::DenseSet<U>;
  static constexpr bool set_like = true;
};

template <typename T, typename Vec, typename Set> struct enable_pipe<llvm::SetVector<T, Vec, Set>> : std::true_type {};
template <typename T, typename Vec, typename Set> struct sequence_traits<llvm::SetVector<T, Vec, Set>> {
  using value_type = T;
  template <typename U> using rebind = llvm::SetVector<U>;
  static constexpr bool set_like = true;
};

template <typename K, typename V, typename Info, typename Bucket>
struct enable_pipe<llvm::DenseMap<K, V, Info, Bucket>> : std::true_type {};
template <typename K, typename V, typename Info, typename Bucket> struct map_traits<llvm::DenseMap<K, V, Info, Bucket>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = llvm::DenseMap<K2, V2>;
  template <typename K2> using key_container = llvm::DenseSet<K2>;
  template <typename V2> using value_container = llvm::SmallVector<V2>;
  template <typename V2> using group_inner = llvm::DenseSet<V2>;
};

template <typename K, typename V, typename Map, typename Vec> struct enable_pipe<llvm::MapVector<K, V, Map, Vec>> : std::true_type {};
template <typename K, typename V, typename Map, typename Vec> struct map_traits<llvm::MapVector<K, V, Map, Vec>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = llvm::MapVector<K2, V2>;
  template <typename K2> using key_container = llvm::SmallVector<K2>;
  template <typename V2> using value_container = llvm::SmallVector<V2>;
  template <typename V2> using group_inner = llvm::SetVector<V2>;
};
// MapVector has no mapped_type alias, so the default map_access::lookup can't
// deduce its return type.
template <typename K, typename V, typename Map, typename Vec> struct map_access<llvm::MapVector<K, V, Map, Vec>> {
  static const llvm::MapVector<K, V, Map, Vec> &entries(const llvm::MapVector<K, V, Map, Vec> &m) { return m; }
  template <typename Kx, typename Vx> static void put(llvm::MapVector<K, V, Map, Vec> &out, Kx &&k, Vx &&v) {
    out.insert({std::forward<Kx>(k), std::forward<Vx>(v)});
  }
  template <typename Key> static std::optional<V> lookup(const llvm::MapVector<K, V, Map, Vec> &m, const Key &key) {
    auto it = m.find(key);
    return it != m.end() ? std::optional<V>{it->second} : std::optional<V>{};
  }
};

template <typename T> struct enable_pipe<llvm::ArrayRef<T>> : std::true_type {};
template <typename T> struct sequence_traits<llvm::ArrayRef<T>> {
  using value_type = T;
  template <typename U> using rebind = llvm::SmallVector<U>;
  static constexpr bool set_like = false;
};

template <> struct enable_pipe<llvm::StringRef> : std::true_type {};
template <> struct sequence_traits<llvm::StringRef> {
  using value_type = char;
  template <typename U> using rebind = llvm::SmallVector<U>;
  static constexpr bool set_like = false;
};

template <unsigned N> struct enable_pipe<llvm::SmallString<N>> : std::true_type {};
template <unsigned N> struct sequence_traits<llvm::SmallString<N>> {
  using value_type = char;
  template <typename U> using rebind = llvm::SmallVector<U, N>;
  static constexpr bool set_like = false;
};

template <typename It> struct enable_pipe<llvm::iterator_range<It>> : std::true_type {};
template <typename It> struct sequence_traits<llvm::iterator_range<It>> {
  using value_type = typename std::iterator_traits<It>::value_type;
  template <typename U> using rebind = llvm::SmallVector<U>;
  static constexpr bool set_like = false;
};

template <typename K, typename V, unsigned N, typename Info, typename Bucket>
struct enable_pipe<llvm::SmallDenseMap<K, V, N, Info, Bucket>> : std::true_type {};
template <typename K, typename V, unsigned N, typename Info, typename Bucket>
struct map_traits<llvm::SmallDenseMap<K, V, N, Info, Bucket>> {
  using key_type = K;
  using mapped_type = V;
  template <typename K2, typename V2> using rebind = llvm::SmallDenseMap<K2, V2, N>;
  template <typename K2> using key_container = llvm::DenseSet<K2>;
  template <typename V2> using value_container = llvm::SmallVector<V2>;
  template <typename V2> using group_inner = llvm::DenseSet<V2>;
};

namespace detail_llvm {
template <typename M> struct string_map_kv_range {
  using value_t = std::pair<llvm::StringRef, const typename M::mapped_type &>;
  const M &m;
  struct iter {
    using iterator_category = std::input_iterator_tag;
    using value_type = value_t;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = value_t;
    typename M::const_iterator it;
    value_t operator*() const { return value_t{it->getKey(), it->getValue()}; }
    iter &operator++() {
      ++it;
      return *this;
    }
    bool operator!=(const iter &o) const { return it != o.it; }
    bool operator==(const iter &o) const { return it == o.it; }
  };
  iter begin() const { return {m.begin()}; }
  iter end() const { return {m.end()}; }
};
} // namespace detail_llvm

template <typename V, typename A> struct enable_pipe<llvm::StringMap<V, A>> : std::true_type {};
template <typename V, typename A> struct map_traits<llvm::StringMap<V, A>> {
  using key_type = llvm::StringRef;
  using mapped_type = V;
  template <typename K2, typename V2>
  using rebind = std::conditional_t<std::is_same_v<K2, llvm::StringRef>, llvm::StringMap<V2>, llvm::DenseMap<K2, V2>>;
  template <typename K2> using key_container = llvm::SmallVector<K2>;
  template <typename V2> using value_container = llvm::SmallVector<V2>;
  template <typename V2> using group_inner = llvm::DenseSet<V2>;
};
template <typename V, typename A> struct map_access<llvm::StringMap<V, A>> {
  static detail_llvm::string_map_kv_range<llvm::StringMap<V, A>> entries(const llvm::StringMap<V, A> &m) { return {m}; }
  template <typename Kx, typename Vx> static void put(llvm::StringMap<V, A> &out, Kx &&k, Vx &&v) {
    out[std::forward<Kx>(k)] = std::forward<Vx>(v);
  }
  template <typename Key> static std::optional<V> lookup(const llvm::StringMap<V, A> &m, const Key &key) {
    auto it = m.find(key);
    return it != m.end() ? std::optional<V>{it->getValue()} : std::optional<V>{};
  }
};

namespace detail_llvm {
template <typename A> struct string_set_range {
  const llvm::StringSet<A> &s;
  struct iter {
    using iterator_category = std::input_iterator_tag;
    using value_type = llvm::StringRef;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = llvm::StringRef;
    typename llvm::StringSet<A>::const_iterator it;
    llvm::StringRef operator*() const { return it->getKey(); }
    iter &operator++() {
      ++it;
      return *this;
    }
    bool operator!=(const iter &o) const { return it != o.it; }
    bool operator==(const iter &o) const { return it == o.it; }
  };
  iter begin() const { return {s.begin()}; }
  iter end() const { return {s.end()}; }
  size_t size() const { return s.size(); }
  bool empty() const { return s.empty(); }
};
} // namespace detail_llvm

template <typename A> struct enable_pipe<llvm::StringSet<A>> : std::true_type {};
template <typename A> struct sequence_traits<llvm::StringSet<A>> {
  using value_type = llvm::StringRef;
  template <typename U> using rebind = llvm::SmallVector<U>;
  static constexpr bool set_like = true;
};
template <typename A> struct sequence_access<llvm::StringSet<A>> {
  static detail_llvm::string_set_range<A> iterate(const llvm::StringSet<A> &s) { return {s}; }
};

} // namespace aspartame
