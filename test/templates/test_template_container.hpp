#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

#include "../test_base_container1.hpp"
#include "aspartame/fluent.hpp"
#include "catch2/catch_test_macros.hpp"

#ifndef DISABLE_MK_STRING
TEST_CASE("mk_string", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs | mk_string(); };
  RUN_CHECK(int, string, "", {4, 2, 3, 1, 5}, "42315", op);
  RUN_CHECK(int, string, "", {1}, "1", op);
  RUN_CHECK(int, string, "", {}, "", op);
  RUN_CHECK(string, string, "", {"banana", "cherry", "apple"}, "bananacherryapple", op);
  RUN_CHECK(string, string, "", {"apple"}, {"apple"}, op);
  RUN_CHECK(string, string, "", {}, {}, op);
  RUN_CHECK(Foo, string, "", {Foo(3), Foo(2), Foo(1)}, "Foo(3)Foo(2)Foo(1)", op);
  RUN_CHECK(Foo, string, "", {Foo(1)}, "Foo(1)", op);
  RUN_CHECK(Foo, string, "", {}, {}, op);

  auto customOp = [](auto xs) { return xs | mk_string(", "); };
  RUN_CHECK(int, string, "", {4, 2, 3, 1, 5}, "4, 2, 3, 1, 5", customOp);
  RUN_CHECK(int, string, "", {1}, "1", customOp);
  RUN_CHECK(int, string, "", {}, "", customOp);
  RUN_CHECK(string, string, "", {"banana", "cherry", "apple"}, "banana, cherry, apple", customOp);
  RUN_CHECK(string, string, "", {"apple"}, {"apple"}, customOp);
  RUN_CHECK(string, string, "", {}, {}, customOp);
  RUN_CHECK(Foo, string, "", {Foo(3), Foo(2), Foo(1)}, "Foo(3), Foo(2), Foo(1)", customOp);
  RUN_CHECK(Foo, string, "", {Foo(1)}, "Foo(1)", customOp);
  RUN_CHECK(Foo, string, "", {}, {}, customOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, string, name, {{3, 1}, {2, 2}, {1, 3}}, "4,4,4", f);
    RUN_CHECK(P2, string, name, {{3, 1}}, "4", f);
    RUN_CHECK(P2, string, name, {}, "", f);
  };
  p2("spread", [](auto xs) { return xs | mk_string(",", [](auto x0, auto x1) { return to_string(x0 + x1); }); });
  p2("single", [](auto xs) { return xs | mk_string(",", [](auto x) { return to_string(get<0>(x) + get<1>(x)); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, string, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, "7,6,5", f);
    RUN_CHECK(P3, string, name, {{3, 1, 3}}, "7", f);
    RUN_CHECK(P3, string, name, {}, "", f);
  };
  p3("spread", [](auto xs) { return xs | mk_string(",", [](auto x0, auto x1, auto x2) { return to_string(x0 + x1 + x2); }); });
  p3("single", [](auto xs) { return xs | mk_string(",", [](auto x) { return to_string(get<0>(x) + get<1>(x) + get<2>(x)); }); });
}
#endif

#ifndef DISABLE_APPEND
TEST_CASE("append", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto x) { return [x](auto xs) { return xs | append(x); }; };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5, 6}, op(6));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple", "mango"}, op("mango"));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1), Foo(4)}, op(Foo(4)));
  #endif

  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {}, {42}, op(42));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {}, {"pear"}, op("pear"));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {}, {Foo(7)}, op(Foo(7)));
}
#endif

#ifndef DISABLE_CONCAT
TEST_CASE("concat", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto x) { return [x](auto xs) { return xs | concat(x); }; };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5, 6, 7, 8}, op(TPE_CTOR_IN(int){6, 7, 8}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple", "mango", "kiwi"},
            op(TPE_CTOR_IN(string){"mango", "kiwi"}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1), Foo(4), Foo(5)},
            op(TPE_CTOR_IN(Foo){Foo(4), Foo(5)}));
  #endif

  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {}, {1}, op(TPE_CTOR_IN(int){1}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {}, {"pear"}, op(TPE_CTOR_IN(string){"pear"}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {}, {Foo(7)}, op(TPE_CTOR_IN(Foo){Foo(7)}));

  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {1}, {1, 1}, op(TPE_CTOR_IN(int){1}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {"pear"}, {"pear", "pear"}, op(TPE_CTOR_IN(string){"pear"}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {Foo(7)}, {Foo(7), Foo(7)}, op(TPE_CTOR_IN(Foo){Foo(7)}));
}
#endif

#ifndef DISABLE_MAP
TEST_CASE("map", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | map([](auto x) { return x * 2; }); };
  auto strOp = [](auto xs) { return xs | map([](auto x) { return x + x; }); };
  auto fooOp = [](auto xs) { return xs | map([](auto x) { return Foo(x.value * 2); }); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {8, 4, 6, 2, 10}, intOp);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"bananabanana", "cherrycherry", "appleapple"}, strOp);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(6), Foo(4), Foo(2)}, fooOp);
  #endif

  RUN_CHECK_ID(int, "", {1}, {2}, intOp);
  RUN_CHECK_ID(int, "", {}, {}, intOp);
  RUN_CHECK_ID(string, "", {"apple"}, {"appleapple"}, strOp);
  RUN_CHECK_ID(string, "", {}, {}, strOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(2)}, fooOp);
  RUN_CHECK_ID(Foo, "", {}, {}, fooOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P2, TPE_CTOR_OUT(int), name, {{3, 1}, {2, 2}, {1, 3}}, {4, 4, 4}, f);
  #endif
    RUN_CHECK(P2, TPE_CTOR_OUT(int), name, {{3, 1}}, {4}, f);
    RUN_CHECK(P2, TPE_CTOR_OUT(int), name, {}, {}, f);
  };
  p2("spread", [](auto xs) { return xs | map([](auto x0, auto x1) { return x0 + x1; }); });
  p2("single", [](auto xs) { return xs | map([](auto x) { return get<0>(x) + get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {7, 6, 5}, f);
  #endif
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}}, {7}, f);
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {}, {}, f);
  };
  p3("spread", [](auto xs) { return xs | map([](auto x0, auto x1, auto x2) { return x0 + x1 + x2; }); });
  p3("single", [](auto xs) { return xs | map([](auto x) { return get<0>(x) + get<1>(x) + get<2>(x); }); });
}
#endif

#ifndef DISABLE_COLLECT
TEST_CASE("collect", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) {
    return xs | collect([](auto x) -> std::optional<string> { //
             return x % 2 != 0 ? std::nullopt : std::optional{to_string(x)};
           });
  };
  auto strOp = [](auto xs) {
    return xs | collect([](auto x) -> std::optional<string> { //
             return x == "cherry" ? std::optional{"a"} : std::nullopt;
           });
  };
  auto fooOp = [](auto xs) {
    return xs | collect([](auto x) -> std::optional<string> { //
             return x.value % 2 != 0 ? std::nullopt : std::optional{to_string(x.value)};
           });
  };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_OUT(string), "", {4, 2, 3, 1, 5}, {"4", "2"}, intOp);
  RUN_CHECK(string, TPE_CTOR_OUT(string), "", {"banana", "cherry", "apple"}, {"a"}, strOp);
  RUN_CHECK(Foo, TPE_CTOR_OUT(string), "", {Foo(3), Foo(2), Foo(1)}, {"2"}, fooOp);
  #endif

  RUN_CHECK(int, TPE_CTOR_OUT(string), "", {1}, {}, intOp);
  RUN_CHECK(int, TPE_CTOR_OUT(string), "", {}, {}, intOp);
  RUN_CHECK(string, TPE_CTOR_OUT(string), "", {"apple"}, {}, strOp);
  RUN_CHECK(string, TPE_CTOR_OUT(string), "", {}, {}, strOp);
  RUN_CHECK(Foo, TPE_CTOR_OUT(string), "", {Foo(1)}, {}, fooOp);
  RUN_CHECK(Foo, TPE_CTOR_OUT(string), "", {}, {}, fooOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P2, TPE_CTOR_OUT(string), name, {{3, 1}, {2, 2}, {1, 3}}, {"4", "4", "4"}, f);
  #endif
    RUN_CHECK(P2, TPE_CTOR_OUT(string), name, {{3, 1}}, {"4"}, f);
    RUN_CHECK(P2, TPE_CTOR_OUT(string), name, {}, {}, f);
  };
  p2("spread", [](auto xs) {
    return xs | collect([](auto x0, auto x1) {
             int x = x0 + x1;
             return x % 2 != 0 ? std::nullopt : std::optional{to_string(x)};
           });
  });
  p2("single", [](auto xs) {
    return xs | collect([](auto x) {
             int v = get<0>(x) + get<1>(x);
             return v % 2 != 0 ? std::nullopt : std::optional{to_string(v)};
           });
  });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, TPE_CTOR_OUT(string), name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {"6"}, f);
  #endif
    RUN_CHECK(P3, TPE_CTOR_OUT(string), name, {{3, 1, 3}}, {}, f);
    RUN_CHECK(P3, TPE_CTOR_OUT(string), name, {}, {}, f);
  };
  p3("spread", [](auto xs) {
    return xs | collect([](auto x0, auto x1, auto x2) {
             int x = x0 + x1 + x2;
             return x % 2 != 0 ? std::nullopt : std::optional{to_string(x)};
           });
  });
  p3("single", [](auto xs) {
    return xs | collect([](auto x) {
             int v = get<0>(x) + get<1>(x) + get<2>(x);
             return v % 2 != 0 ? std::nullopt : std::optional{to_string(v)};
           });
  });
}
#endif

#ifndef DISABLE_FILTER
TEST_CASE("filter", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | filter([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto xs) { return xs | filter([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto xs) { return xs | filter([](auto x) { return x.value % 2 == 0; }); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2}, intOp);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"cherry"}, strOp);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(2)}, fooOp);
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, intOp);
  RUN_CHECK_ID(int, "", {}, {}, intOp);
  RUN_CHECK_ID(string, "", {"apple"}, {}, strOp);
  RUN_CHECK_ID(string, "", {}, {}, strOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, fooOp);
  RUN_CHECK_ID(Foo, "", {}, {}, fooOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK_ID(P2, name, {{3, 1}, {2, 2}, {1, 3}}, {{2, 2}}, f);
  #endif
    RUN_CHECK_ID(P2, name, {{3, 1}}, {}, f);
    RUN_CHECK_ID(P2, name, {}, {}, f);
  };
  p2("spread", [](auto xs) { return xs | filter([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto xs) { return xs | filter([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK_ID(P3, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {{2, 2, 2}}, f);
  #endif
    RUN_CHECK_ID(P3, name, {{3, 3, 3}}, {{3,3,3}}, f);
    RUN_CHECK_ID(P3, name, {}, {}, f);
  };
  p3("spread", [](auto xs) { return xs | filter([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto xs) { return xs | filter([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_BIND
TEST_CASE("bind", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto bindOp = [](auto xs) { return xs | bind([](auto x) { return TPE_CTOR_OUT(decltype(x)){x}; }); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, bindOp);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple"}, bindOp);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, bindOp);
  #endif
  RUN_CHECK_ID(int, "", {1}, {1}, bindOp);
  RUN_CHECK_ID(int, "", {}, {}, bindOp);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, bindOp);
  RUN_CHECK_ID(string, "", {}, {}, bindOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, bindOp);
  RUN_CHECK_ID(Foo, "", {}, {}, bindOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P2, TPE_CTOR_OUT(int), name, {{3, 1}, {2, 2}, {1, 3}}, {3 + 1, 2 + 2, 1 + 3}, f);
  #endif
    RUN_CHECK(P2, TPE_CTOR_OUT(int), name, {{3, 1}}, {3 + 1}, f);
    RUN_CHECK(P2, TPE_CTOR_OUT(int), name, {}, {}, f);
  };
  p2("spread", [](auto xs) { return xs | bind([](auto x0, auto x1) { return TPE_CTOR_OUT(decltype(x0)){x0 + x1}; }); });
  p2("single",
     [](auto xs) { return xs | bind([](auto x) { return TPE_CTOR_OUT(std::decay_t<decltype(get<0>(x))>){get<0>(x) + get<1>(x)}; }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {3 + 1 + 3, 2 + 2 + 2, 1 + 3 + 1}, f);
  #endif
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}}, {3 + 1 + 3}, f);
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {}, {}, f);
  };
  p3("spread", [](auto xs) { return xs | bind([](auto x0, auto x1, auto x2) { return TPE_CTOR_OUT(decltype(x0)){x0 + x1 + x2}; }); });
  p3("single", [](auto xs) {
    return xs | bind([](auto x) { return TPE_CTOR_OUT(std::decay_t<decltype(get<0>(x))>){get<0>(x) + get<1>(x) + get<2>(x)}; });
  });
}
#endif

#ifndef DISABLE_FLATTEN
TEST_CASE("flatten", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto flattenOp = [](auto xs) { return xs | flatten(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(TPE_CTOR_IN(int), TPE_CTOR_OUT(int), "", {{4, 2, 3}, {1, 5}}, {4, 2, 3, 1, 5}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(string), TPE_CTOR_OUT(string), "", {{"banana", "cherry"}, {"apple"}}, {"banana", "cherry", "apple"}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(Foo), TPE_CTOR_OUT(Foo), "", {{Foo(3), Foo(2)}, {Foo(1)}}, {Foo(3), Foo(2), Foo(1)}, flattenOp);
  #endif

  RUN_CHECK(TPE_CTOR_IN(int), TPE_CTOR_OUT(int), "", {{1}}, {1}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(int), TPE_CTOR_OUT(int), "", {{}}, {}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(int), TPE_CTOR_OUT(int), "", {}, {}, flattenOp);

  RUN_CHECK(TPE_CTOR_IN(string), TPE_CTOR_OUT(string), "", {{"apple"}}, {"apple"}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(string), TPE_CTOR_OUT(string), "", {{}}, {}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(string), TPE_CTOR_OUT(string), "", {}, {}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(Foo), TPE_CTOR_OUT(Foo), "", {{Foo(1)}}, {Foo(1)}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(Foo), TPE_CTOR_OUT(Foo), "", {{}}, {}, flattenOp);
  RUN_CHECK(TPE_CTOR_IN(Foo), TPE_CTOR_OUT(Foo), "", {}, {}, flattenOp);
}
#endif

#ifndef DISABLE_DISTINCT
TEST_CASE("distinct", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs | distinct(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {3, 2, 4, 5, 2, 13, 4}, {3, 2, 4, 5, 13}, op);
  RUN_CHECK_ID(string, "", {"a", "b", "c", "a"}, {"a", "b", "c"}, op);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, op);
  #endif

  RUN_CHECK_ID(int, "", {1}, {1}, op);
  RUN_CHECK_ID(int, "", {}, {}, op);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, op);
  RUN_CHECK_ID(string, "", {}, {}, op);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, op);
  RUN_CHECK_ID(Foo, "", {}, {}, op);
}
#endif

#ifndef DISABLE_DISTINCT_BY
TEST_CASE("distinct_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | distinct_by([](auto x) { return x % 10; }); };
  auto strOp = [](auto xs) { return xs | distinct_by([](auto x) { return x.length(); }); };
  auto fooOp = [](auto xs) { return xs | distinct_by([](auto x) { return x.value % 10; }); };
  #ifdef TPE_MANY_INIT
    #ifdef TPE_UNORDERED
  RUN_CHECK_ID(int, "", {13, 22, 34, 45, 13}, {13, 22, 34, 45}, intOp);
  RUN_CHECK_ID(string, "", {"a", "aa", "aaa"}, {"a", "aa", "aaa"}, strOp);
  RUN_CHECK_ID(Foo, "", {Foo(13), Foo(22), Foo(31), Foo(15)}, {Foo(13), Foo(22), Foo(31), Foo(15)}, fooOp);
    #else
  RUN_CHECK_ID(int, "", {13, 22, 34, 45, 52, 13, 24}, {13, 22, 34, 45}, intOp);
  RUN_CHECK_ID(string, "", {"apple", "banana", "carrot", "ape", "berry", "cherry"}, {"apple", "banana", "ape"}, strOp);
  RUN_CHECK_ID(Foo, "", {Foo(13), Foo(22), Foo(31), Foo(41), Foo(15)}, {Foo(13), Foo(22), Foo(31), Foo(15)}, fooOp);
    #endif
  #endif

  RUN_CHECK_ID(int, "", {1}, {1}, intOp);
  RUN_CHECK_ID(int, "", {}, {}, intOp);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, strOp);
  RUN_CHECK_ID(string, "", {}, {}, strOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, fooOp);
  RUN_CHECK_ID(Foo, "", {}, {}, fooOp);
}
#endif

#ifndef DISABLE_COUNT
TEST_CASE("count", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | count([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto xs) { return xs | count([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto xs) { return xs | count([](auto x) { return x.value % 2 == 0; }); };
  RUN_CHECK(int, size_t, "", {4, 2, 3, 1, 5}, 2, intOp);
  RUN_CHECK(int, size_t, "", {1}, 0, intOp);
  RUN_CHECK(int, size_t, "", {}, 0, intOp);
  RUN_CHECK(string, size_t, "", {"banana", "cherry", "apple"}, 1, strOp);
  RUN_CHECK(string, size_t, "", {"apple"}, 0, strOp);
  RUN_CHECK(string, size_t, "", {}, 0, strOp);
  RUN_CHECK(Foo, size_t, "", {Foo(3), Foo(2), Foo(1)}, 1, fooOp);
  RUN_CHECK(Foo, size_t, "", {Foo(1)}, 0, fooOp);
  RUN_CHECK(Foo, size_t, "", {}, 0, fooOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, size_t, name, {{3, 1}, {2, 2}, {1, 3}}, 1, f);
    RUN_CHECK(P2, size_t, name, {{3, 1}}, 0, f);
    RUN_CHECK(P2, size_t, name, {}, 0, f);
  };
  p2("spread", [](auto xs) { return xs | count([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto xs) { return xs | count([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, size_t, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, 1, f);
    RUN_CHECK(P3, size_t, name, {{3, 1, 3}}, 0, f);
    RUN_CHECK(P3, size_t, name, {}, 0, f);
  };
  p3("spread", [](auto xs) { return xs | count([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto xs) { return xs | count([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_EXISTS
TEST_CASE("exists", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | exists([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto xs) { return xs | exists([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto xs) { return xs | exists([](auto x) { return x.value % 2 == 0; }); };
  RUN_CHECK(int, bool, "", {4, 2, 3, 1, 5}, true, intOp);
  RUN_CHECK(int, bool, "", {1}, false, intOp);
  RUN_CHECK(int, bool, "", {}, false, intOp);
  RUN_CHECK(string, bool, "", {"banana", "cherry", "apple"}, true, strOp);
  RUN_CHECK(string, bool, "", {"apple"}, false, strOp);
  RUN_CHECK(string, bool, "", {}, false, strOp);
  RUN_CHECK(Foo, bool, "", {Foo(3), Foo(2), Foo(1)}, true, fooOp);
  RUN_CHECK(Foo, bool, "", {Foo(1)}, false, fooOp);
  RUN_CHECK(Foo, bool, "", {}, false, fooOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, bool, name, {{3, 1}, {2, 2}, {1, 3}}, true, f);
    RUN_CHECK(P2, bool, name, {{3, 1}}, false, f);
    RUN_CHECK(P2, bool, name, {}, false, f);
  };
  p2("spread", [](auto xs) { return xs | exists([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto xs) { return xs | exists([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, bool, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, true, f);
    RUN_CHECK(P3, bool, name, {{3, 1, 3}}, false, f);
    RUN_CHECK(P3, bool, name, {}, false, f);
  };
  p3("spread", [](auto xs) { return xs | exists([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto xs) { return xs | exists([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_FORALL
TEST_CASE("forall", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | forall([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto xs) { return xs | forall([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto xs) { return xs | forall([](auto x) { return x.value % 2 == 0; }); };
  RUN_CHECK(int, bool, "", {4, 2, 8, 0, 0}, true, intOp);
  RUN_CHECK(int, bool, "", {1}, false, intOp);
  RUN_CHECK(int, bool, "", {}, true, intOp);
  RUN_CHECK(string, bool, "", {"cherry", "cherry", "cherry"}, true, strOp);
  RUN_CHECK(string, bool, "", {"cherry", "apple"}, false, strOp);
  RUN_CHECK(string, bool, "", {}, true, strOp);
  RUN_CHECK(Foo, bool, "", {Foo(2), Foo(4), Foo(6)}, true, fooOp);
  RUN_CHECK(Foo, bool, "", {Foo(1), Foo(2)}, false, fooOp);
  RUN_CHECK(Foo, bool, "", {}, true, fooOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, bool, name, {{3, 1}, {2, 2}, {1, 3}}, false, f);
    RUN_CHECK(P2, bool, name, {{2, 2}}, true, f);
    RUN_CHECK(P2, bool, name, {}, true, f);
  };
  p2("spread", [](auto xs) { return xs | forall([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto xs) { return xs | forall([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, bool, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, false, f);
    RUN_CHECK(P3, bool, name, {{3, 3, 3}}, true, f);
    RUN_CHECK(P3, bool, name, {}, true, f);
  };
  p3("spread", [](auto xs) { return xs | forall([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto xs) { return xs | forall([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_REDUCE
TEST_CASE("reduce", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | reduce([](auto acc, auto x) { return acc + x; }); };
  auto fooOp = [](auto xs) { return xs | reduce([](auto acc, auto x) { return Foo(acc.value + x.value); }); };
  auto stringOp = [](auto xs) {
    return xs | reduce([](auto acc, auto x) {
             auto out = acc + x;
             std::sort(out.begin(), out.end());
             return out;
           });
  };

  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, std::optional<int>{15}, intOp);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, std::optional<string>{"aaaabceehlnnpprry"}, stringOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, std::optional<Foo>{Foo(6)}, fooOp);

  RUN_CHECK(int, std::optional<int>, "", {4}, std::optional<int>{4}, intOp);
  RUN_CHECK(string, std::optional<string>, "", {"banana"}, std::optional<string>{"banana"}, stringOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3)}, std::optional<Foo>{Foo(3)}, fooOp);

  RUN_CHECK(int, std::optional<int>, "", {}, std::nullopt, intOp);
  RUN_CHECK(string, std::optional<string>, "", {}, std::nullopt, stringOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, std::nullopt, fooOp);
}
#endif

#ifndef DISABLE_TAP_EACH
TEST_CASE("tap_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) {
    std::vector<std::decay_t<typename decltype(xs)::value_type>> out;
    auto ys = xs | tap_each([&](auto &x) { out.push_back(x); });
    CHECK((ys == xs));
    return out;
  };
  RUN_CHECK(int, std::vector<int>, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, op);
  RUN_CHECK(int, std::vector<int>, "", {1}, {1}, op);
  RUN_CHECK(int, std::vector<int>, "", {}, {}, op);
  RUN_CHECK(string, std::vector<string>, "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple"}, op);
  RUN_CHECK(string, std::vector<string>, "", {"apple"}, {"apple"}, op);
  RUN_CHECK(string, std::vector<string>, "", {}, {}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {Foo(1)}, {Foo(1)}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {}, {}, op);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, std::vector<int>, name, {{3, 1}, {2, 2}, {1, 3}}, {4, 4, 4}, f);
    RUN_CHECK(P2, std::vector<int>, name, {{3, 1}}, {4}, f);
    RUN_CHECK(P2, std::vector<int>, name, {}, {}, f);
  };
  p2("spread", [](auto xs) {
    std::vector<int> out;
    auto ys = xs | tap_each([&](auto x0, auto x1) { out.push_back(x0 + x1); });
    CHECK((ys == xs));
    return out;
  });
  p2("single", [](auto xs) {
    std::vector<int> out;
    auto ys = xs | tap_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x)); });
    CHECK((ys == xs));
    return out;
  });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {7, 6, 5}, f);
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}}, {7}, f);
    RUN_CHECK(P3, std::vector<int>, name, {}, {}, f);
  };
  p3("spread", [](auto xs) {
    std::vector<int> out;
    auto ys = xs | tap_each([&](auto x0, auto x1, auto x2) { out.push_back(x0 + x1 + x2); });
    CHECK((ys == xs));
    return out;
  });
  p3("single", [](auto xs) {
    std::vector<int> out;
    auto ys = xs | tap_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x) + get<2>(x)); });
    CHECK((ys == xs));
    return out;
  });
}
#endif

#ifndef DISABLE_FOR_EACH
TEST_CASE("for_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) {
    std::vector<std::decay_t<typename decltype(xs)::value_type>> out;
    xs | for_each([&](auto x) { out.push_back(x); });
    return out;
  };
  RUN_CHECK(int, std::vector<int>, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, op);
  RUN_CHECK(int, std::vector<int>, "", {1}, {1}, op);
  RUN_CHECK(int, std::vector<int>, "", {}, {}, op);
  RUN_CHECK(string, std::vector<string>, "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple"}, op);
  RUN_CHECK(string, std::vector<string>, "", {"apple"}, {"apple"}, op);
  RUN_CHECK(string, std::vector<string>, "", {}, {}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {Foo(1)}, {Foo(1)}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {}, {}, op);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, std::vector<int>, name, {{3, 1}, {2, 2}, {1, 3}}, {4, 4, 4}, f);
    RUN_CHECK(P2, std::vector<int>, name, {{3, 1}}, {4}, f);
    RUN_CHECK(P2, std::vector<int>, name, {}, {}, f);
  };
  p2("spread", [](auto xs) {
    std::vector<int> out;
    xs | for_each([&](auto x0, auto x1) { out.push_back(x0 + x1); });
    return out;
  });
  p2("single", [](auto xs) {
    std::vector<int> out;
    xs | for_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x)); });
    return out;
  });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {7, 6, 5}, f);
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}}, {7}, f);
    RUN_CHECK(P3, std::vector<int>, name, {}, {}, f);
  };
  p3("spread", [](auto xs) {
    std::vector<int> out;
    xs | for_each([&](auto x0, auto x1, auto x2) { out.push_back(x0 + x1 + x2); });
    return out;
  });
  p3("single", [](auto xs) {
    std::vector<int> out;
    xs | for_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x) + get<2>(x)); });
    return out;
  });
}
#endif

#ifndef DISABLE_PARTITION
TEST_CASE("partition", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto xs) { return xs | partition([&](auto x) { return x % 2 == 0; }); };
  auto stringOp = [](auto xs) { return xs | partition([&](auto x) { return x.size() > 1; }); };
  auto fooOp = [](auto xs) { return xs | partition([&](auto x) { return x.value % 2 == 0; }); };
  using IntP = std::pair<TPE_CTOR_OUT(int), TPE_CTOR_OUT(int)>;
  using StringP = std::pair<TPE_CTOR_OUT(string), TPE_CTOR_OUT(string)>;
  using FooP = std::pair<TPE_CTOR_OUT(Foo), TPE_CTOR_OUT(Foo)>;
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, IntP, "", {2, 3, 1, 5, 4}, {{2, 4}, {3, 1, 5}}, intOp);
  RUN_CHECK(string, StringP, "", {"a", "aa", "aaa"}, {{"aa", "aaa"}, {"a"}}, stringOp);
  RUN_CHECK(Foo, FooP, "", {Foo(2), Foo(3), Foo(1), Foo(5), Foo(4)}, {{Foo(2), Foo(4)}, {Foo(3), Foo(1), Foo(5)}}, fooOp);
  #endif

  RUN_CHECK(int, IntP, "", {4}, {{4}, {}}, intOp);
  RUN_CHECK(int, IntP, "", {5}, {{}, {5}}, intOp);
  RUN_CHECK(int, IntP, "", {}, {{}, {}}, intOp);

  RUN_CHECK(string, StringP, "", {"a"}, {{}, {"a"}}, stringOp);
  RUN_CHECK(string, StringP, "", {"aa"}, {{"aa"}, {}}, stringOp);
  RUN_CHECK(string, StringP, "", {}, {{}, {}}, stringOp);

  RUN_CHECK(Foo, FooP, "", {Foo(4)}, {{Foo(4)}, {}}, fooOp);
  RUN_CHECK(Foo, FooP, "", {Foo(5)}, {{}, {Foo(5)}}, fooOp);
  RUN_CHECK(Foo, FooP, "", {}, {{}, {}}, fooOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    using IntP2 = std::pair<TPE_CTOR_OUT(P2), TPE_CTOR_OUT(P2)>;

  #ifdef TPE_MANY_INIT
    RUN_CHECK(P2, IntP2, name, {{3, 1}, {2, 2}, {1, 3}}, {{{2, 2}}, {{3, 1}, {1, 3}}}, f);
  #endif
    RUN_CHECK(P2, IntP2, name, {{3, 1}}, {{}, {{3, 1}}}, f);
    RUN_CHECK(P2, IntP2, name, {}, {{}, {}}, f);
  };
  p2("spread", [](auto xs) { return xs | partition([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto xs) { return xs | partition([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    using IntP3 = std::pair<TPE_CTOR_OUT(P3), TPE_CTOR_OUT(P3)>;

  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, IntP3, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {{{2, 2, 2}}, {{3, 1, 3}, {1, 3, 1}}}, f);
  #endif
    RUN_CHECK(P3, IntP3, name, {{3, 1, 3}}, {{}, {{3, 1, 3}}}, f);
    RUN_CHECK(P3, IntP3, name, {}, {{}, {}}, f);
  };
  p3("spread", [](auto xs) { return xs | partition([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto xs) { return xs | partition([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_GROUP_MAP_REDUCE
TEST_CASE("group_map_reduce", "[" TPE_NAME "][" TPE_GROUP "]") {

  auto p2 = [](auto intOp, auto stringOp, auto fooOp) {
    using IntP = std::pair<int, int>;
    using StringP = std::pair<string, string>;
    using FooP = std::pair<Foo, Foo>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(IntP, std::unordered_map<string COMMA int>, "", {{3, 3}, {3, 5}, {2, 2}}, {{"3", 16}, {"2", 4}}, intOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA string>, "", {{"apple", "a"}, {"ant", "b"}, {"banana", "c"}},
              {{"a", "XXab"}, {"b", "cX"}}, stringOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA int>, "", {{Foo(1), Foo(2)}, {Foo(1), Foo(3)}, {Foo(2), Foo(4)}},
              {{"1", 15}, {"2", 12}}, fooOp);
  #endif
    RUN_CHECK(IntP, std::unordered_map<string COMMA int>, "", {{3, 3}}, {{"3", 6}}, intOp);
    RUN_CHECK(IntP, std::unordered_map<string COMMA int>, "", {}, {}, intOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA string>, "", {{"apple", "a"}}, {{"a", "aX"}}, stringOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA string>, "", {}, {}, stringOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA int>, "", {{Foo(1), Foo(2)}}, {{"1", 6}}, fooOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA int>, "", {}, {}, fooOp);
  };
  p2(
      [](auto xs) {
        return xs | group_map_reduce([](auto x) { return std::to_string(get<0>(x)); }, //
                                     [](auto x) { return get<1>(x) * 2; },             //
                                     [](auto x, auto y) { return x + y; });
      },
      [](auto xs) {
        return xs | group_map_reduce([](auto x) { return get<0>(x).substr(0, 1); }, //
                                     [](auto x) { return get<1>(x) + "X"; },        //
                                     [](auto x, auto y) {
                                       auto out = x + y;
                                       std::sort(out.begin(), out.end());
                                       return out;
                                     });
      },
      [](auto xs) {
        return xs | group_map_reduce([](auto x, auto) { return std::to_string(x.value); }, //
                                     [](auto, auto y) { return y.value * 3; },             //
                                     [](auto x, auto y) { return x + y; });
      });
  p2(
      [](auto xs) {
        return xs | group_map_reduce([](auto x, auto) { return std::to_string(x); }, //
                                     [](auto, auto y) { return y * 2; },             //
                                     [](auto x, auto y) { return x + y; });
      },
      [](auto xs) {
        return xs | group_map_reduce([](auto x) { return get<0>(x).substr(0, 1); }, //
                                     [](auto, auto y) { return y + "X"; },          //
                                     [](auto x, auto y) {
                                       auto out = x + y;
                                       std::sort(out.begin(), out.end());
                                       return out;
                                     });
      },
      [](auto xs) {
        return xs | group_map_reduce([](auto x, auto) { return std::to_string(x.value); }, //
                                     [](auto, auto y) { return y.value * 3; },             //
                                     [](auto x, auto y) { return x + y; });
      });
}
#endif

#ifndef DISABLE_GROUP_MAP
TEST_CASE("group_map", "[" TPE_NAME "][" TPE_GROUP "]") {

  auto p2 = [](auto intOp, auto stringOp, auto fooOp) {
    using IntP = std::pair<int, int>;
    using StringP = std::pair<string, string>;
    using FooP = std::pair<Foo, Foo>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(IntP, std::unordered_map<string COMMA TPE_CTOR_OUT(int)>, "", {{3, 3}, {3, 5}, {2, 2}}, {{"3", {6, 10}}, {"2", {4}}}, intOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA TPE_CTOR_OUT(string)>, "", {{"apple", "a"}, {"ant", "b"}, {"banana", "c"}},
              {{"a", {"aX", "bX"}}, {"b", {"cX"}}}, stringOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA TPE_CTOR_OUT(int)>, "", {{Foo(1), Foo(2)}, {Foo(1), Foo(3)}, {Foo(2), Foo(4)}},
              {{"1", {6, 9}}, {"2", {12}}}, fooOp);
  #endif
    RUN_CHECK(IntP, std::unordered_map<string COMMA TPE_CTOR_OUT(int)>, "", {{3, 3}}, {{"3", {6}}}, intOp);
    RUN_CHECK(IntP, std::unordered_map<string COMMA TPE_CTOR_OUT(int)>, "", {}, {}, intOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA TPE_CTOR_OUT(string)>, "", {{"apple", "a"}}, {{"a", {"aX"}}}, stringOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA TPE_CTOR_OUT(string)>, "", {}, {}, stringOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA TPE_CTOR_OUT(int)>, "", {{Foo(1), Foo(2)}}, {{"1", {6}}}, fooOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA TPE_CTOR_OUT(int)>, "", {}, {}, fooOp);
  };
  p2([](auto xs) { return xs | group_map([](auto x) { return std::to_string(get<0>(x)); }, [](auto x) { return get<1>(x) * 2; }); },
     [](auto xs) { return xs | group_map([](auto x) { return get<0>(x).substr(0, 1); }, [](auto x) { return get<1>(x) + "X"; }); },
     [](auto xs) {
       return xs | group_map([](auto x, auto) { return std::to_string(x.value); }, [](auto, auto y) { return y.value * 3; });
     });
  p2([](auto xs) { return xs | group_map([](auto x, auto) { return std::to_string(x); }, [](auto, auto y) { return y * 2; }); },
     [](auto xs) { return xs | group_map([](auto x) { return get<0>(x).substr(0, 1); }, [](auto, auto y) { return y + "X"; }); },
     [](auto xs) {
       return xs | group_map([](auto x, auto) { return std::to_string(x.value); }, [](auto, auto y) { return y.value * 3; });
     });
}
#endif

#ifndef DISABLE_GROUP_BY
TEST_CASE("group_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto p2 = [](auto intOp, auto stringOp, auto fooOp) {
    using IntP = std::pair<int, int>;
    using StringP = std::pair<string, string>;
    using FooP = std::pair<Foo, Foo>;
  #ifdef TPE_MANY_INIT

    RUN_CHECK(IntP, std::unordered_map<string COMMA TPE_CTOR_OUT(IntP)>, "", {{3, 3}, {3, 5}, {2, 2}},
              {{"3", {{3, 3}, {3, 5}}}, {"2", {{2, 2}}}}, intOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA TPE_CTOR_OUT(StringP)>, "", {{"apple", "a"}, {"ant", "b"}, {"banana", "c"}},
              {{"a", {{"apple", "a"}, {"ant", "b"}}}, {"b", {{"banana", "c"}}}}, stringOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA TPE_CTOR_OUT(FooP)>, "", {{Foo(1), Foo(2)}, {Foo(1), Foo(3)}, {Foo(2), Foo(4)}},
              {{"1", TPE_CTOR_OUT(FooP){{Foo(1), Foo(2)}, {Foo(1), Foo(3)}}}, {"2", TPE_CTOR_OUT(FooP){{Foo(2), Foo(4)}}}}, fooOp);
  #endif
    RUN_CHECK(IntP, std::unordered_map<string COMMA TPE_CTOR_OUT(IntP)>, "", {{3, 3}}, {{"3", {{3, 3}}}}, intOp);
    RUN_CHECK(IntP, std::unordered_map<string COMMA TPE_CTOR_OUT(IntP)>, "", {}, {}, intOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA TPE_CTOR_OUT(StringP)>, "", {{"apple", "a"}}, {{"a", {{"apple", "a"}}}}, stringOp);
    RUN_CHECK(StringP, std::unordered_map<string COMMA TPE_CTOR_OUT(StringP)>, "", {}, {}, stringOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA TPE_CTOR_OUT(FooP)>, "", {{Foo(1), Foo(2)}}, {{"1", {{Foo(1), Foo(2)}}}}, fooOp);
    RUN_CHECK(FooP, std::unordered_map<string COMMA TPE_CTOR_OUT(FooP)>, "", {}, {}, fooOp);
  };
  p2([](auto xs) { return xs | group_by([](auto x) { return std::to_string(get<0>(x)); }); },
     [](auto xs) { return xs | group_by([](auto x) { return get<0>(x).substr(0, 1); }); },
     [](auto xs) { return xs | group_by([](auto x, auto) { return std::to_string(x.value); }); });
  p2([](auto xs) { return xs | group_by([](auto x, auto) { return std::to_string(x); }); },
     [](auto xs) { return xs | group_by([](auto x) { return get<0>(x).substr(0, 1); }); },
     [](auto xs) { return xs | group_by([](auto x, auto) { return std::to_string(x.value); }); });
}
#endif

#ifndef DISABLE_TO_VECTOR
TEST_CASE("to_vector", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs | to_vector(); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, std::vector<int>, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, op);
  RUN_CHECK(string, std::vector<string>, "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple"}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, op);
  #endif

  RUN_CHECK(int, std::vector<int>, "", {1}, {1}, op);
  RUN_CHECK(int, std::vector<int>, "", {}, {}, op);
  RUN_CHECK(string, std::vector<string>, "", {"apple"}, {"apple"}, op);
  RUN_CHECK(string, std::vector<string>, "", {}, {}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {Foo(1)}, {Foo(1)}, op);
  RUN_CHECK(Foo, std::vector<Foo>, "", {}, {}, op);
}
#endif
