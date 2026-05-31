#pragma once

#include <sstream>

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"

#include "test_base_harness.hpp"

using namespace ::aspartame;
using std::string;

#define RUN_CHECK(K, V, U, _name, ...)                                                                                                     \
  runTest<K, V, U>(std::string(TPE_NAME "<" #K "," #V "> => " #U " [") + _name + "]", __FILE_NAME__ ":" QUOTE(__LINE__), __VA_ARGS__)
#define COMMA ,

template <typename K, typename V, typename C, typename F>
void runTest(const string &typeName, //
             const string &location, //
             TPE_CTOR_IN(K, V) in,   //
             C expected,             //
             F f,                    //
             bool invert = false) {

  DYNAMIC_SECTION(typeName << " n=" << in.size() << " @ " << location) {
    TPE_CTOR_IN(K, V) v = TPE_INIT_TO_CTOR_IN(in);
    TPE_CTOR_IN(K, V) copy = v;
    static_assert(std::is_same_v<decltype(f(v)), C>, "Bad result type");
    auto actual = f(v);

    std::stringstream expected_stream, actual_stream;
    expected_stream << expected;
    actual_stream << actual;

    INFO((invert ? "!expected = " : "expected =  ") << expected_stream.str());
    INFO("actual =    " << actual_stream.str());

#ifdef TPE_UNORDERED
    if constexpr (harness_detail::wants_sort_compare_v<C>) {
      std::vector<typename C::value_type> expected_sorted{expected.begin(), expected.end()};
      std::vector<typename C::value_type> actual_sorted{actual.begin(), actual.end()};
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
