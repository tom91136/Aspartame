#pragma once

#include <optional>
#include <utility>
#include <vector>

#include <QHash>
#include <QList>
#include <QMap>
#include <QSet>

#include <aspartame/all.hpp>

namespace aspartame {

template <class T> struct enable_pipe<QList<T>> : std::true_type {};
template <class T> struct sequence_traits<QList<T>> {
  using value_type = T;
  template <class U> using rebind = QList<U>;
  static constexpr bool set_like = false;
};

template <class T> struct enable_pipe<QSet<T>> : std::true_type {};
template <class T> struct sequence_traits<QSet<T>> {
  using value_type = T;
  template <class U> using rebind = QSet<U>;
  static constexpr bool set_like = true;
};

namespace detail_qt {
template <typename M> struct kv_range {
  const M &m;
  auto begin() const { return m.constKeyValueBegin(); }
  auto end() const { return m.constKeyValueEnd(); }
};
} // namespace detail_qt

template <class K, class V> struct enable_pipe<QMap<K, V>> : std::true_type {};
template <class K, class V> struct map_traits<QMap<K, V>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = QMap<K2, V2>;
  template <class K2> using key_container = QList<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = QList<V2>;
};
template <class K, class V> struct map_access<QMap<K, V>> {
  static detail_qt::kv_range<QMap<K, V>> entries(const QMap<K, V> &m) { return {m}; }
  template <class Kx, class Vx> static void put(QMap<K, V> &out, Kx &&k, Vx &&v) { out.insert(std::forward<Kx>(k), std::forward<Vx>(v)); }
  template <class Key> static std::optional<V> lookup(const QMap<K, V> &m, const Key &key) {
    auto it = m.find(key);
    return it != m.end() ? std::optional<V>{it.value()} : std::optional<V>{};
  }
};

template <class K, class V> struct enable_pipe<QHash<K, V>> : std::true_type {};
template <class K, class V> struct map_traits<QHash<K, V>> {
  using key_type = K;
  using mapped_type = V;
  template <class K2, class V2> using rebind = QHash<K2, V2>;
  template <class K2> using key_container = QList<K2>;
  template <class V2> using value_container = std::vector<V2>;
  template <class V2> using group_inner = QList<V2>;
};
template <class K, class V> struct map_access<QHash<K, V>> {
  static detail_qt::kv_range<QHash<K, V>> entries(const QHash<K, V> &m) { return {m}; }
  template <class Kx, class Vx> static void put(QHash<K, V> &out, Kx &&k, Vx &&v) { out.insert(std::forward<Kx>(k), std::forward<Vx>(v)); }
  template <class Key> static std::optional<V> lookup(const QHash<K, V> &m, const Key &key) {
    auto it = m.find(key);
    return it != m.end() ? std::optional<V>{it.value()} : std::optional<V>{};
  }
};

} // namespace aspartame
