#pragma once

#include <climits>
#include <string>

#include <llvm/ADT/DenseMapInfo.h>

#include "../fixtures.hpp"

namespace aspartame_test_sentinels {
inline const std::string empty_key = "\x01__aspartame_empty\x01";
inline const std::string tomb_key = "\x01__aspartame_tomb\x01";
} // namespace aspartame_test_sentinels

namespace llvm {

template <> struct DenseMapInfo<std::string> {
  static inline std::string getEmptyKey() { return aspartame_test_sentinels::empty_key; }
  static inline std::string getTombstoneKey() { return aspartame_test_sentinels::tomb_key; }
  static unsigned getHashValue(const std::string &s) { return static_cast<unsigned>(std::hash<std::string>{}(s)); }
  static bool isEqual(const std::string &a, const std::string &b) { return a == b; }
};

template <> struct DenseMapInfo<Foo> {
  static inline Foo getEmptyKey() { return Foo(INT_MAX); }
  static inline Foo getTombstoneKey() { return Foo(INT_MAX - 1); }
  static unsigned getHashValue(const Foo &f) { return static_cast<unsigned>(f.value); }
  static bool isEqual(const Foo &a, const Foo &b) { return a == b; }
};

template <> struct DenseMapInfo<MyString> {
  static inline MyString getEmptyKey() { return MyString{aspartame_test_sentinels::empty_key}; }
  static inline MyString getTombstoneKey() { return MyString{aspartame_test_sentinels::tomb_key}; }
  static unsigned getHashValue(const MyString &s) { return static_cast<unsigned>(std::hash<std::string>{}(s.value)); }
  static bool isEqual(const MyString &a, const MyString &b) { return a == b; }
};

template <> struct DenseMapInfo<MyFoo> {
  static inline MyFoo getEmptyKey() { return MyFoo{Foo(INT_MAX)}; }
  static inline MyFoo getTombstoneKey() { return MyFoo{Foo(INT_MAX - 1)}; }
  static unsigned getHashValue(const MyFoo &f) { return static_cast<unsigned>(f.value.value); }
  static bool isEqual(const MyFoo &a, const MyFoo &b) { return a == b; }
};

} // namespace llvm
