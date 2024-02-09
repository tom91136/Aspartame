#include <functional>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#include "../test_base_container1.hpp"
#include "catch2/catch_test_macros.hpp"

#ifndef DISABLE_AND_THEN
TEST_CASE(TPE_NAME "_and_then", "[" TPE_NAME "][" TPE_GROUP "]") {
  using IntP = std::pair<TPE_CTOR_IN(int), TPE_CTOR_IN(int)>;
  using StringP = std::pair<TPE_CTOR_IN(string), TPE_CTOR_IN(string)>;
  using FooP = std::pair<TPE_CTOR_IN(Foo), TPE_CTOR_IN(Foo)>;

  auto op = [](auto &&xs) { return xs ^ and_then([](auto x) { return std::pair{x, x}; }); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, IntP, "", {4, 2, 3, 1, 5}, {{4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}}, op);
  RUN_CHECK(string, StringP, "", {"banana", "cherry", "apple"}, {{{"banana", "cherry", "apple"}}, {{"banana", "cherry", "apple"}}}, op);
  RUN_CHECK(Foo, FooP, "", {Foo(3), Foo(2), Foo(1)}, {{Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}}, op);
  #endif

  RUN_CHECK(int, IntP, "", {1}, {{1}, {1}}, op);
  RUN_CHECK(int, IntP, "", {}, {{}, {}}, op);
  RUN_CHECK(string, StringP, "", {"apple"}, {{{"apple"}}, {{"apple"}}}, op);
  RUN_CHECK(string, StringP, "", {}, {{}, {}}, op);
  RUN_CHECK(Foo, FooP, "", {Foo(1)}, {{Foo(1)}, {Foo(1)}}, op);
  RUN_CHECK(Foo, FooP, "", {}, {}, op);
}
#endif

#ifndef DISABLE_TAP
TEST_CASE(TPE_NAME "_tap", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) {
    std::decay_t<decltype(xs)> ys;
    xs ^ tap([&](auto x) { ys = x; });
    return ys;
  };

  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, op);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple"}, op);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, op);
  #endif

  RUN_CHECK_ID(int, "", {1}, {1}, op);
  RUN_CHECK_ID(int, "", {}, {}, op);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, op);
  RUN_CHECK_ID(string, "", {}, {}, op);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, op);
  RUN_CHECK_ID(Foo, "", {}, {}, op);
}
#endif