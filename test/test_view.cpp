#include "test_base_includes.hpp"

#include "catch2/catch_test_macros.hpp"
#include <sstream>

#define OP_ | // NOLINT(*-macro-parentheses)
#define TPE_GROUP "test_view"
#define TPE_NAME "view"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_MANY_INIT 1
#define TPE_RUN_TEST

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

      TPE_CTOR_IN(TPE_CTOR_IN(typename T::value_type)) actual_vector;
      for (auto it = actual.begin(); it != actual.end(); ++it) {
        auto &&view = *it;
        actual_vector.emplace_back(view.begin(), view.end());
      }
      if (invert) CHECK_FALSE(actual_vector == expected);
      else CHECK(actual_vector == expected);
    } else {
      TPE_CTOR_IN(T) actual_vector{actual.begin(), actual.end()};
      if (invert) CHECK_FALSE(actual_vector == expected);
      else CHECK(actual_vector == expected);
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

  DYNAMIC_SECTION(typeName << " n=" << in.size() << " && "
                           << " @ " << location) {
    TPE_CTOR_IN(Element) v = in;
    TPE_CTOR_IN(Element) copy = v;
    check(v);
    CHECK((v == copy));
  }
  DYNAMIC_SECTION(typeName << " n=" << in.size() << " |map(id) "
                           << " @ " << location) {
    TPE_CTOR_IN(Element) v = in;
    TPE_CTOR_IN(Element) copy = v;
    check(v | ::aspartame::map([](auto x) { return x; }));
    CHECK((v == copy));
  }

  DYNAMIC_SECTION(typeName << " n=" << in.size() << " |map(id)|filter(true) "
                           << " @ " << location) {
    TPE_CTOR_IN(Element) v = in;
    TPE_CTOR_IN(Element) copy = v;
    check(v                                              //
          | ::aspartame::map([](auto x) { return x; })     //
          | ::aspartame::filter([](auto) { return true; }) //
    );
    CHECK((v == copy));
  }
}

#define DISABLE_TAP_EACH

#include "templates/test_template_container.hpp"

// #define DISABLE_PREPEND
// #define DISABLE_HEAD_MAYBE
// #define DISABLE_LAST_MAYBE
#define DISABLE_INIT // TODO
#define DISABLE_TAIL
// #define DISABLE_AT_MAYBE
// #define DISABLE_SLICE
#define DISABLE_INDEX_OF_SLICE
#define DISABLE_CONTAINS_SLICE
// #define DISABLE_INDEX_OF
// #define DISABLE_CONTAINS
// #define DISABLE_INDEX_WHERE
#define DISABLE_ZIP_WITH_INDEX // TODO
#define DISABLE_ZIP            // TODO
#define DISABLE_TRANSPOSE
#define DISABLE_REVERSE
#define DISABLE_SHUFFLE
#define DISABLE_SORT
#define DISABLE_SORT_BY
#define DISABLE_SPLIT_AT // TODO
// #define DISABLE_TAKE
// #define DISABLE_DROP
#define DISABLE_TAKE_RIGHT
#define DISABLE_DROP_RIGHT
// #define DISABLE_TAKE_WHILE
// #define DISABLE_DROP_WHILE
// #define DISABLE_FOLD_LEFT
#define DISABLE_FOLD_RIGHT
// #define DISABLE_SLIDING
// #define DISABLE_GROUPED

#include "templates/test_template_sequence.hpp"
// #include "templates/test_template_any.hpp"