#include <sstream>

#include "catch2/catch_test_macros.hpp"

#include "../test_base_includes.hpp"

#define TPE_IS_VIEW 1

#define DISABLE_COMBINE_ALL
#define DISABLE_INTERSPERSE
#define DISABLE_TRAVERSE
#define DISABLE_SEQUENCE

template <typename Element, typename Expected, typename F>
void runTest(const std::string &typeName,       //
             const std::string &location,       //
             std::initializer_list<Element> in, //
             Expected expected,                 //
             F f,                               //
             bool invert = false) {

  auto check_direct = [&](auto &&actual, auto &&expected) {
    static_assert(std::is_same_v<std::decay_t<decltype(actual)>, Expected>, "Bad result type");
    if (invert) CHECK_FALSE(actual == expected);
    else CHECK(actual == expected);
  };

  auto check_view = [&](auto &&actual, auto &&expected) {
    using T = typename std::decay_t<decltype(actual)>::value_type;
    if constexpr (::aspartame::is_view<T>) {

      TPE_CTOR_OUT(TPE_CTOR_OUT(typename T::value_type)) actual_xs;
      for (auto it = actual.begin(); it != actual.end(); ++it) {
        auto &&view = *it;
        actual_xs.emplace_back(view.begin(), view.end());
      }
      if (invert) CHECK_FALSE(actual_xs == expected);
      else CHECK(actual_xs == expected);
    } else {
      TPE_CTOR_OUT(T) actual_xs{actual.begin(), actual.end()};
      if (invert) CHECK_FALSE(actual_xs == expected);
      else CHECK(actual_xs == expected);
    }
  };

  auto check = [&](auto &&x) {
    using R = decltype(f(x));

    if constexpr (::aspartame::is_pair<Expected>) {
      if constexpr (::aspartame::is_view<typename R::first_type> && ::aspartame::is_view<typename R::second_type>) {
        auto [l, r] = f(x);
        check_view(l, expected.first);
        check_view(r, expected.second);
      } else {
        check_direct(f(x), expected);
      }
    } else if constexpr (::aspartame::is_view<R>) {
      check_view(f(x), expected);
    } else {
      check_direct(f(x), expected);
    }
  };

  DYNAMIC_SECTION(typeName << " n=" << in.size() << " @ " << location) {
    TPE_CTOR_IN(Element) v = in;
    TPE_CTOR_IN(Element) copy = v;
    check(TPE_TEST_CHAIN(v));
    CHECK((v == copy));
  }
}

#include "test_template_container.hpp"

#define DISABLE_INIT
#define DISABLE_TRANSPOSE
#define DISABLE_CARTESIAN_PRODUCT
#define DISABLE_REVERSE
#define DISABLE_SHUFFLE
#define DISABLE_SORT
#define DISABLE_SORT_BY
#define DISABLE_TAKE_RIGHT
#define DISABLE_DROP_RIGHT
#define DISABLE_FOLD_RIGHT
#define DISABLE_SCAN_LEFT
#define DISABLE_SCAN_RIGHT
#define DISABLE_UNZIP
#define DISABLE_INTERSECT
#define DISABLE_DIFF
#define DISABLE_JOIN_WITH
#define DISABLE_TOP_K
#define DISABLE_BOTTOM_K
#define DISABLE_SYMMETRIC_DIFFERENCE

#include "test_template_sequence.hpp"