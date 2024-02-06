#pragma once

#include "fixtures.hpp"

#include "aspartame/fluent.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"

using namespace aspartame;

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef __FILE_NAME__
  #define __FILE_NAME__ __FILE__
#endif

#define RUN_CHECK(T, U, _name, ...)                                                                                                        \
  runTest<T, U>(std::string(TPE_NAME "<" #T "> => " #U " [") + _name + "]", __FILE_NAME__ ":" QUOTE(__LINE__), __VA_ARGS__)
#define RUN_CHECK_ID(T, _name, ...) RUN_CHECK(T, TPE_CTOR_OUT(T), _name, __VA_ARGS__)
#define COMMA ,

template <typename> constexpr bool is_unordered_set_private = false;
template <typename T> constexpr bool is_unordered_set_private<std::unordered_set<T>> = true;

template <typename> constexpr bool is_vector_private = false;
template <typename T> constexpr bool is_vector_private<std::vector<T>> = true;

#ifndef TPE_RUN_TEST

template <typename Element, typename Expected, typename F>
void runTest(const std::string &typeName,       //
             const std::string &location,       //
             std::initializer_list<Element> in, //
             Expected expected,                 //
             F f,                               //
             bool invert = false) {

  if (TPE_INIT_SKIP(in)) return;
  DYNAMIC_SECTION(typeName << " n=" << in.size() << " @ " << location) {
    TPE_CTOR_IN(Element) v = TPE_INIT_TO_CTOR_IN(in);
    TPE_CTOR_IN(Element) copy = v;
    static_assert(std::is_same_v<decltype(f(v)), Expected>, "Bad result type");
    auto actual = f(v);

    std::stringstream expected_stream, actual_stream;
    expected_stream << expected;
    actual_stream << actual;

    INFO((invert ? "!expected = " : "expected =  ") << expected_stream.str());
    INFO("actual =    " << actual_stream.str());

  #ifdef TPE_UNORDERED
    if constexpr (is_unordered_set_private<Expected> || is_vector_private<Expected> || std::is_same_v<Expected, std::string>) {
      std::vector<typename Expected::value_type> expected_sorted{expected.begin(), expected.end()};
      std::vector<typename Expected::value_type> actual_sorted{actual.begin(), actual.end()};
      std::sort(expected_sorted.begin(), expected_sorted.end());
      std::sort(actual_sorted.begin(), actual_sorted.end());

      if (invert) CHECK_FALSE(actual_sorted == expected_sorted);
      else CHECK(actual_sorted == expected_sorted);
    } else {
      if (invert) CHECK_FALSE(actual == expected);
      else CHECK(actual == expected);
    }
  #else
    if (invert) CHECK_FALSE(actual == expected);
    else CHECK(actual == expected);
  #endif

    CHECK((v == copy));
  }
}

#endif
