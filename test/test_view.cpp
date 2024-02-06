#include "aspartame/view.hpp"

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

  DYNAMIC_SECTION(typeName << " n=" << in.size() << " @ " << location) {
    std::vector<Element> v = in;
    std::vector<Element> copy = v;
//    static_assert(std::is_same_v<decltype(f(v)), Expected>, "Bad result type");
    auto actual = f(v);

//    std::stringstream expected_stream, actual_stream;
//    expected_stream << expected;
//    actual_stream << actual;

//    INFO((invert ? "!expected = " : "expected =  ") << expected_stream.str());
//    INFO("actual =    " << actual_stream.str());

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
//    if (invert) CHECK_FALSE(actual == expected);
//    else CHECK(actual == expected);
#endif
    CHECK((v == copy));
  }
}


#define DISABLE_DISTINCT_BY
#define DISABLE_DISTINCT
#define DISABLE_PARTITION
#define DISABLE_COLLECT


#include "templates/test_template_container.hpp"
// #include "templates/test_template_sequence.hpp"
// #include "templates/test_template_any.hpp"