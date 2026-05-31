#pragma once

#include <ostream>

#include <QHash>
#include <QList>
#include <QMap>
#include <QString>

inline std::ostream &operator<<(std::ostream &os, const QString &s) { return os << s.toStdString(); }

template <class T> std::ostream &operator<<(std::ostream &os, const QList<T> &xs) {
  os << "QList{";
  for (qsizetype i = 0; i < xs.size(); ++i) {
    if (i > 0) os << ", ";
    os << xs.at(i);
  }
  return os << "}";
}

template <class K, class V> std::ostream &operator<<(std::ostream &os, const QMap<K, V> &xs) {
  os << "QMap{";
  bool first = true;
  for (auto it = xs.constKeyValueBegin(); it != xs.constKeyValueEnd(); ++it) {
    if (!first) os << ", ";
    first = false;
    os << "{" << (*it).first << ", " << (*it).second << "}";
  }
  return os << "}";
}

template <class K, class V> std::ostream &operator<<(std::ostream &os, const QHash<K, V> &xs) {
  os << "QHash{";
  bool first = true;
  for (auto it = xs.constKeyValueBegin(); it != xs.constKeyValueEnd(); ++it) {
    if (!first) os << ", ";
    first = false;
    os << "{" << (*it).first << ", " << (*it).second << "}";
  }
  return os << "}";
}
