#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

#include "../test_base_container2.hpp"
#include "catch2/catch_test_macros.hpp"

#ifndef DISABLE_MK_STRING
TEST_CASE(TPE_NAME "_mk_string", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto mkStringOp = [](auto && xs) {
    return xs OP_ mk_string("", [](auto x, auto y) {
      std::stringstream ss;
      ss << x << y;
      return ss.str();
    });
  };
  RUN_CHECK(string, int, string, "", {{"banana", 4}, {"cherry", 2}, {"apple", 3}}, "banana4cherry2apple3", mkStringOp);
  RUN_CHECK(string, int, string, "", {{"apple", 1}}, "apple1", mkStringOp);
  RUN_CHECK(string, int, string, "", {}, "", mkStringOp);

  RUN_CHECK(string, string, string, "", {{"banana", "yellow"}, {"cherry", "red"}, {"apple", "green"}}, "bananayellowcherryredapplegreen",
            mkStringOp);
  RUN_CHECK(string, string, string, "", {{"apple", "green"}}, "applegreen", mkStringOp);
  RUN_CHECK(string, string, string, "", {}, "", mkStringOp);

  RUN_CHECK(string, Foo, string, "", {{"banana", Foo(4)}, {"cherry", Foo(2)}, {"apple", Foo(3)}}, "bananaFoo(4)cherryFoo(2)appleFoo(3)",
            mkStringOp);
  RUN_CHECK(string, Foo, string, "", {{"apple", Foo(1)}}, "appleFoo(1)", mkStringOp);
  RUN_CHECK(string, Foo, string, "", {}, "", mkStringOp);

  auto mkStringCustomOp = [](auto && xs) {
    return xs OP_ mk_string(", ", [](auto x, auto y) {
      std::stringstream ss;
      ss << x << y;
      return ss.str();
    });
  };
  RUN_CHECK(string, int, string, "", {{"banana", 4}, {"cherry", 2}, {"apple", 3}}, "banana4, cherry2, apple3", mkStringCustomOp);
  RUN_CHECK(string, int, string, "", {{"apple", 1}}, "apple1", mkStringCustomOp);
  RUN_CHECK(string, int, string, "", {}, "", mkStringCustomOp);

  RUN_CHECK(string, string, string, "", {{"banana", "yellow"}, {"cherry", "red"}, {"apple", "green"}},
            "bananayellow, cherryred, applegreen", mkStringCustomOp);
  RUN_CHECK(string, string, string, "", {{"apple", "green"}}, "applegreen", mkStringCustomOp);
  RUN_CHECK(string, string, string, "", {}, "", mkStringCustomOp);

  RUN_CHECK(string, Foo, string, "", {{"banana", Foo(4)}, {"cherry", Foo(2)}, {"apple", Foo(3)}}, "bananaFoo(4), cherryFoo(2), appleFoo(3)",
            mkStringCustomOp);
  RUN_CHECK(string, Foo, string, "", {{"apple", Foo(1)}}, "appleFoo(1)", mkStringCustomOp);
  RUN_CHECK(string, Foo, string, "", {}, "", mkStringCustomOp);

  auto p2 = [](auto name, auto f) {
    RUN_CHECK(int, int, string, name, {{3, 1}, {2, 2}, {1, 3}}, "4,4,4", f);
    RUN_CHECK(int, int, string, name, {{3, 1}}, "4", f);
    RUN_CHECK(int, int, string, name, {}, "", f);
  };
  p2("spread", [](auto && xs) { return xs OP_ mk_string(",", [](auto x0, auto x1) { return to_string(x0 + x1); }); });
  p2("single", [](auto && xs) { return xs OP_ mk_string(",", [](auto x) { return to_string(get<0>(x) + get<1>(x)); }); });
}
#endif

#ifndef DISABLE_APPEND
TEST_CASE(TPE_NAME "_append", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto appendOp = [](auto x) { return [x](auto && xs) { return xs OP_ append(std::pair(x, x)); }; };

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{1, 1}, {2, 2}, {3, 3}}, {{1, 1}, {2, 2}, {3, 3}, {4, 4}}, appendOp(4));
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}, {"banana", "banana"}},
            {{"apple", "apple"}, {"banana", "banana"}, {"cherry", "cherry"}}, appendOp("cherry"));
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(1), Foo(1)}, {Foo(2), Foo(2)}},
            {{Foo(1), Foo(1)}, {Foo(2), Foo(2)}, {Foo(3), Foo(3)}}, appendOp(Foo(3)));

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {}, {{42, 42}}, appendOp(42));
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {{"pear", "pear"}}, appendOp("pear"));
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {}, {{Foo(7), Foo(7)}}, appendOp(Foo(7)));
}
#endif

#ifndef DISABLE_CONCAT
TEST_CASE(TPE_NAME "_concat", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto concatOp = [](auto x) { return [x](auto && xs) { return xs OP_ concat(x); }; };
  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{1, 1}, {2, 2}, {3, 3}}, {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}},
            concatOp(TPE_CTOR_IN(int, int){{4, 4}, {5, 5}, {6, 6}}));
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}, {"banana", "banana"}},
            {{"apple", "apple"}, {"banana", "banana"}, {"cherry", "cherry"}, {"date", "date"}},
            concatOp(TPE_CTOR_IN(string, string){{"cherry", "cherry"}, {"date", "date"}}));
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(1), Foo(1)}, {Foo(2), Foo(2)}},
            {{Foo(1), Foo(1)}, {Foo(2), Foo(2)}, {Foo(3), Foo(3)}, {Foo(4), Foo(4)}},
            concatOp(TPE_CTOR_IN(Foo, Foo){{Foo(3), Foo(3)}, {Foo(4), Foo(4)}}));

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {}, {{1, 1}}, concatOp(TPE_CTOR_IN(int, int){{1, 1}}));
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {{"pear", "pear"}},
            concatOp(TPE_CTOR_IN(string, string){{"pear", "pear"}}));
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {}, {{Foo(7), Foo(7)}}, concatOp(TPE_CTOR_IN(Foo, Foo){{Foo(7), Foo(7)}}));

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{1, 1}}, {{1, 1}, {1, 1}}, concatOp(TPE_CTOR_IN(int, int){{1, 1}}));
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"pear", "pear"}}, {{"pear", "pear"}, {"pear", "pear"}},
            concatOp(TPE_CTOR_IN(string, string){{"pear", "pear"}}));
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(7), Foo(7)}}, {{Foo(7), Foo(7)}, {Foo(7), Foo(7)}},
            concatOp(TPE_CTOR_IN(Foo, Foo){{Foo(7), Foo(7)}}));
}
#endif

#ifndef DISABLE_MAP
TEST_CASE(TPE_NAME "_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  // XXX for maps, we must return a pair
  auto intMapOp = [](auto && xs) { return xs OP_ map([](auto x) { return std::pair(x.first * 2, x.second * 2); }); };
  auto strMapOp = [](auto && xs) { return xs OP_ map([](auto x) { return std::pair(x.first + x.first, x.second + x.second); }); };
  auto fooMapOp = [](auto && xs) { return xs OP_ map([](auto x) { return std::pair(Foo(x.first.value * 2), Foo(x.second.value * 2)); }); };

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{2, 2}, {3, 3}, {1, 1}}, {{4, 4}, {6, 6}, {2, 2}}, intMapOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"banana", "banana"}, {"cherry", "cherry"}},
            {{"bananabanana", "bananabanana"}, {"cherrycherry", "cherrycherry"}}, strMapOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, {{Foo(6), Foo(6)}, {Foo(4), Foo(4)}}, fooMapOp);

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{1, 1}}, {{2, 2}}, intMapOp);
  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {}, {}, intMapOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}}, {{"appleapple", "appleapple"}}, strMapOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {}, strMapOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(1), Foo(1)}}, {{Foo(2), Foo(2)}}, fooMapOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {}, {}, fooMapOp);

  auto p2 = [](auto name, auto f) {
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {{3, 1}, {2, 2}, {1, 3}}, {{4, 4}, {4, 4}, {4, 4}}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {{3, 1}}, {{4, 4}}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {}, {}, f);
  };
  p2("spread", [](auto && xs) { return xs OP_ map([](auto x, auto y) { return std::pair(x + y, x + y); }); });
  p2("single", [](auto && xs) { return xs OP_ map([](auto x) { return std::pair(x.first + x.second, x.first + x.second); }); });
}
#endif

#ifndef DISABLE_COLLECT
TEST_CASE(TPE_NAME "_collect", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intCollectOp = [](auto && xs) {
    return xs OP_ collect([](auto x) -> std::optional<std::pair<int, string>> {
      int value = x.second; // Extract the value part of the pair
      return value % 2 != 0 ? std::nullopt : std::optional{std::pair{x.first, std::to_string(value)}};
    });
  };
  auto strCollectOp = [](auto && xs) {
    return xs OP_ collect([](auto x) -> std::optional<std::pair<string, string>> {
      string value = x.second; // Extract the value part of the pair
      return value == "cherry" ? std::optional{std::pair{value, "a"}} : std::nullopt;
    });
  };
  auto fooCollectOp = [](auto && xs) {
    return xs OP_ collect([](auto x) -> std::optional<std::pair<Foo, string>> {
      Foo value = x.second; // Extract the value part of the pair
      return value.value % 2 != 0 ? std::nullopt : std::optional{std::pair{value, to_string(value.value)}};
    });
  };

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, string), "", {{4, 4}, {2, 2}, {3, 3}, {1, 1}, {5, 5}}, {{4, "4"}, {2, "2"}}, intCollectOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"banana", "banana"}, {"cherry", "cherry"}, {"apple", "apple"}},
            {{"cherry", "a"}}, strCollectOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, string), "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}}, {{Foo(2), "2"}}, fooCollectOp);

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, string), "", {{1, 1}}, {}, intCollectOp);
  RUN_CHECK(int, int, TPE_CTOR_OUT(int, string), "", {}, {}, intCollectOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}}, {}, strCollectOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {}, strCollectOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, string), "", {{Foo(1), Foo(1)}}, {}, fooCollectOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, string), "", {}, {}, fooCollectOp);

  auto p2 = [](auto name, auto f) {
    RUN_CHECK(int, int, TPE_CTOR_OUT(string, int), name, {{3, 1}, {2, 2}, {1, 3}}, {{"4", 4}, {"4", 4}, {"4", 4}}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(string, int), name, {{3, 1}}, {{"4", 4}}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(string, int), name, {}, {}, f);
  };
  p2("spread", [](auto && xs) {
    return xs OP_ collect([](auto x0, auto x1) {
      int x = x0 + x1;
      return x % 2 != 0 ? std::nullopt : std::optional{std::pair{to_string(x), x}};
    });
  });
  p2("single", [](auto && xs) {
    return xs OP_ collect([](auto x) {
      int v = get<0>(x) + get<1>(x);
      return v % 2 != 0 ? std::nullopt : std::optional{std::pair{to_string(v), v}};
    });
  });
}
#endif

#ifndef DISABLE_FILTER
TEST_CASE(TPE_NAME "_filter", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intFilterOp = [](auto && xs) { return xs OP_ filter([](auto x) { return x.second % 2 == 0; }); };
  auto strFilterOp = [](auto && xs) { return xs OP_ filter([](auto x) { return x.second == "cherry"; }); };
  auto fooFilterOp = [](auto && xs) { return xs OP_ filter([](auto x) { return x.second.value % 2 == 0; }); };

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{4, 4}, {2, 2}, {3, 3}, {1, 1}, {5, 5}}, {{4, 4}, {2, 2}}, intFilterOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"banana", "banana"}, {"cherry", "cherry"}, {"apple", "apple"}},
            {{"cherry", "cherry"}}, strFilterOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}}, {{Foo(2), Foo(2)}}, fooFilterOp);

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{1, 1}}, {}, intFilterOp);
  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {}, {}, intFilterOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}}, {}, strFilterOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {}, strFilterOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(1), Foo(1)}}, {}, fooFilterOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {}, {}, fooFilterOp);

  auto p2 = [](auto name, auto f) {
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {{3, 1}, {2, 2}, {1, 3}}, {{2, 2}}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {{3, 1}}, {}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {}, {}, f);
  };
  p2("spread", [](auto && xs) { return xs OP_ filter([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto && xs) { return xs OP_ filter([](auto x) { return get<0>(x) == get<1>(x); }); });
}
#endif

#ifndef DISABLE_BIND
TEST_CASE(TPE_NAME "_bind", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto bindOp = [](auto && xs) {
    return xs OP_ bind([](auto x) { return TPE_CTOR_OUT(std::remove_const_t<decltype(x.first)>, decltype(x.second)){x}; });
  };

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{4, 4}, {2, 2}, {3, 3}, {1, 1}, {5, 5}}, {{4, 4}, {2, 2}, {3, 3}, {1, 1}, {5, 5}},
            bindOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"banana", "banana"}, {"cherry", "cherry"}, {"apple", "apple"}},
            {{"banana", "banana"}, {"cherry", "cherry"}, {"apple", "apple"}}, bindOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}},
            {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}}, bindOp);

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {{1, 1}}, {{1, 1}}, bindOp);
  RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), "", {}, {}, bindOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}}, {{"apple", "apple"}}, bindOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {}, bindOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(1), Foo(1)}}, {{Foo(1), Foo(1)}}, bindOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {}, {}, bindOp);

  auto p2 = [](auto name, auto f) {
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {{3, 1}, {2, 2}, {1, 3}}, {{4, 4}}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {{3, 1}}, {{4, 4}}, f);
    RUN_CHECK(int, int, TPE_CTOR_OUT(int, int), name, {}, {}, f);
  };
  p2("spread", [](auto && xs) {
    return xs OP_ bind([](auto x0, auto x1) { return TPE_CTOR_OUT(decltype(x0), decltype(x1)){{x0 + x1, x0 + x1}}; });
  });
  p2("single", [](auto && xs) {
    return xs OP_ bind([](auto x) {
      return TPE_CTOR_OUT(std::decay_t<decltype(get<0>(x))>,
                          std::decay_t<decltype(get<1>(x))>){{get<0>(x) + get<1>(x), get<0>(x) + get<1>(x)}};
    });
  });
}
#endif

#ifndef DISABLE_FLATTEN
TEST_CASE(TPE_NAME "_flatten", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto flattenOp = [](auto && xs) { return xs OP_ flatten(); };
  RUN_CHECK(int, TPE_CTOR_IN(int, int), TPE_CTOR_OUT(int, int), "", {{{1, {{2, 2}, {3, 3}}}, {4, {{5, 5}}}}}, {{2, 2}, {3, 3}, {5, 5}},
            flattenOp);
  RUN_CHECK(string, TPE_CTOR_IN(string, string), TPE_CTOR_OUT(string, string), "",
            {{{"fruits", {{"banana", "yellow"}, {"cherry", "red"}}}, {"veggies", {{"carrot", "orange"}}}}},
            {{"banana", "yellow"}, {"cherry", "red"}, {"carrot", "orange"}}, flattenOp);
  RUN_CHECK(Foo, TPE_CTOR_IN(Foo, Foo), TPE_CTOR_OUT(Foo, Foo), "",
            {{{Foo(1), {{Foo(2), Foo(2)}, {Foo(3), Foo(3)}}}, {Foo(4), {{Foo(5), Foo(5)}}}}},
            {{Foo(2), Foo(2)}, {Foo(3), Foo(3)}, {Foo(5), Foo(5)}}, flattenOp);

  RUN_CHECK(int, TPE_CTOR_IN(int, int), TPE_CTOR_OUT(int, int), "", {{{1, {{2, 2}}}}}, {{2, 2}}, flattenOp);
  RUN_CHECK(int, TPE_CTOR_IN(int, int), TPE_CTOR_OUT(int, int), "", {{{1, {}}}}, {}, flattenOp);
  RUN_CHECK(int, TPE_CTOR_IN(int, int), TPE_CTOR_OUT(int, int), "", {}, {}, flattenOp);

  RUN_CHECK(string, TPE_CTOR_IN(string, string), TPE_CTOR_OUT(string, string), "", {{{"fruits", {{"apple", "green"}}}}},
            {{"apple", "green"}}, flattenOp);
  RUN_CHECK(string, TPE_CTOR_IN(string, string), TPE_CTOR_OUT(string, string), "", {{{"fruits", {}}}}, {}, flattenOp);
  RUN_CHECK(string, TPE_CTOR_IN(string, string), TPE_CTOR_OUT(string, string), "", {}, {}, flattenOp);
  RUN_CHECK(Foo, TPE_CTOR_IN(Foo, Foo), TPE_CTOR_OUT(Foo, Foo), "", {{{Foo(1), {{Foo(1), Foo(1)}}}}}, {{Foo(1), Foo(1)}}, flattenOp);
  RUN_CHECK(Foo, TPE_CTOR_IN(Foo, Foo), TPE_CTOR_OUT(Foo, Foo), "", {{{Foo(1), {}}}}, {}, flattenOp);
  RUN_CHECK(Foo, TPE_CTOR_IN(Foo, Foo), TPE_CTOR_OUT(Foo, Foo), "", {}, {}, flattenOp);
}
#endif

#ifndef DISABLE_EXISTS
TEST_CASE(TPE_NAME "_exists", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intExistOp = [](auto && xs) { return xs OP_ exists([](auto x) { return x.second % 2 == 0; }); };
  auto strExistOp = [](auto && xs) { return xs OP_ exists([](auto x) { return x.second == "cherry"; }); };
  auto fooExistOp = [](auto && xs) { return xs OP_ exists([](auto x) { return x.second.value % 2 == 0; }); };

  RUN_CHECK(int, int, bool, "", {{4, 4}, {2, 2}, {3, 3}, {1, 1}, {5, 5}}, true, intExistOp);
  RUN_CHECK(int, int, bool, "", {{1, 1}}, false, intExistOp);
  RUN_CHECK(int, int, bool, "", {}, false, intExistOp);
  RUN_CHECK(string, string, bool, "", {{"banana", "banana"}, {"cherry", "cherry"}, {"apple", "apple"}}, true, strExistOp);
  RUN_CHECK(string, string, bool, "", {{"apple", "apple"}}, false, strExistOp);
  RUN_CHECK(string, string, bool, "", {}, false, strExistOp);
  RUN_CHECK(Foo, Foo, bool, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}}, true, fooExistOp);
  RUN_CHECK(Foo, Foo, bool, "", {{Foo(1), Foo(1)}}, false, fooExistOp);
  RUN_CHECK(Foo, Foo, bool, "", {}, false, fooExistOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, P2, bool, name, {{{3, 3}, {1, 1}}, {{2, 2}, {2, 2}}, {{1, 1}, {3, 3}}}, true, f);
    RUN_CHECK(P2, P2, bool, name, {{{3, 2}, {1, 1}}}, false, f);
    RUN_CHECK(P2, P2, bool, name, {}, false, f);
  };
  p2("spread", [](auto && xs) { return xs OP_ exists([](auto x0, auto) { return x0.first == x0.second; }); });
  p2("single", [](auto && xs) { return xs OP_ exists([](auto x) { return get<0>(x).first == get<0>(x).second; }); });
}
#endif

#ifndef DISABLE_FORALL
TEST_CASE(TPE_NAME "_forall", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intForallOp = [](auto && xs) { return xs OP_ forall([](auto x) { return x.second % 2 == 0; }); };
  auto strForallOp = [](auto && xs) { return xs OP_ forall([](auto x) { return x.second == "cherry"; }); };
  auto fooForallOp = [](auto && xs) { return xs OP_ forall([](auto x) { return x.second.value % 2 == 0; }); };

  RUN_CHECK(int, int, bool, "", {{4, 4}, {2, 2}, {8, 8}, {0, 0}, {0, 0}}, true, intForallOp);
  RUN_CHECK(int, int, bool, "", {{1, 1}}, false, intForallOp);
  RUN_CHECK(int, int, bool, "", {}, true, intForallOp);
  RUN_CHECK(string, string, bool, "", {{"cherry", "cherry"}, {"cherry", "cherry"}, {"cherry", "cherry"}}, true, strForallOp);
  RUN_CHECK(string, string, bool, "", {{"cherry", "cherry"}, {"apple", "apple"}}, false, strForallOp);
  RUN_CHECK(string, string, bool, "", {}, true, strForallOp);
  RUN_CHECK(Foo, Foo, bool, "", {{Foo(2), Foo(2)}, {Foo(4), Foo(4)}, {Foo(6), Foo(6)}}, true, fooForallOp);
  RUN_CHECK(Foo, Foo, bool, "", {{Foo(1), Foo(1)}, {Foo(2), Foo(2)}}, false, fooForallOp);
  RUN_CHECK(Foo, Foo, bool, "", {}, true, fooForallOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, P2, bool, name, {{{3, 3}, {1, 1}}, {{2, 2}, {2, 2}}, {{1, 1}, {3, 3}}}, true, f);
    RUN_CHECK(P2, P2, bool, name, {{{3, 2}, {2, 2}}}, false, f);
    RUN_CHECK(P2, P2, bool, name, {}, true, f);
  };
  p2("spread", [](auto && xs) { return xs OP_ forall([](auto x0, auto) { return x0.first == x0.second; }); });
  p2("single", [](auto && xs) { return xs OP_ forall([](auto x) { return get<0>(x).first == get<0>(x).second; }); });
}
#endif

#ifndef DISABLE_FIND
TEST_CASE(TPE_NAME "_find", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intForallOp = [](auto &&xs) { return xs OP_ find([](auto x) { return x.second == 8; }); };
  auto strForallOp = [](auto &&xs) { return xs OP_ find([](auto x) { return x.second == "cherry"; }); };
  auto fooForallOp = [](auto &&xs) { return xs OP_ find([](auto x) { return x.second.value == 4; }); };

  using IntP = std::optional<std::pair<int, int>>;
  using StringP = std::optional<std::pair<string, string>>;
  using FooP = std::optional<std::pair<Foo, Foo>>;

  RUN_CHECK(int, int, IntP, "", {{4, 4}, {2, 2}, {8, 8}, {0, 0}, {0, 0}}, {{8, 8}}, intForallOp);
  RUN_CHECK(int, int, IntP, "", {{1, 1}}, {}, intForallOp);
  RUN_CHECK(int, int, IntP, "", {}, {}, intForallOp);
  RUN_CHECK(string, string, StringP, "", {{"cherry", "cherry"}, {"cherry", "cherry"}, {"cherry", "cherry"}}, {{"cherry", "cherry"}},
            strForallOp);
  RUN_CHECK(string, string, StringP, "", {{"cherry", "cherry"}, {"apple", "apple"}}, {{"cherry", "cherry"}}, strForallOp);
  RUN_CHECK(string, string, StringP, "", {}, {}, strForallOp);
  RUN_CHECK(Foo, Foo, FooP, "", {{Foo(2), Foo(2)}, {Foo(4), Foo(4)}, {Foo(6), Foo(6)}}, {{Foo(4), Foo(4)}}, fooForallOp);
  RUN_CHECK(Foo, Foo, FooP, "", {{Foo(1), Foo(1)}, {Foo(2), Foo(2)}}, {}, fooForallOp);
  RUN_CHECK(Foo, Foo, FooP, "", {}, {}, fooForallOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    using P22 = std::optional<std::pair<P2, P2>>;
    RUN_CHECK(P2, P2, P22, name, {{{3, 3}, {1, 1}}, {{2, 2}, {2, 2}}, {{1, 2}, {3, 3}}}, {{{1, 2}, {3, 3}}}, f);
    RUN_CHECK(P2, P2, P22, name, {{{3, 2}, {2, 2}}}, {{{3, 2}, {2, 2}}}, f);
    RUN_CHECK(P2, P2, P22, name, {}, {}, f);
  };
  p2("spread", [](auto &&xs) { return xs OP_ find([](auto x0, auto) { return x0.first != x0.second; }); });
  p2("single", [](auto &&xs) { return xs OP_ find([](auto x) { return get<0>(x).first != get<0>(x).second; }); });
}
#endif

#ifndef DISABLE_REDUCE
TEST_CASE(TPE_NAME "_reduce", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intReduceOp = [](auto && xs) {
    return xs OP_ reduce([](auto acc, auto x) { return std::pair<int, int>{acc.first + x.first, acc.second + x.second}; });
  };
  auto fooReduceOp = [](auto && xs) {
    return xs OP_ reduce([](auto acc, auto x) { return std::pair<int, Foo>{acc.first + x.first, Foo(acc.second.value + x.second.value)}; });
  };
  auto stringReduceOp = [](auto && xs) {
    return xs OP_ reduce([](auto acc, auto x) {
      auto f = acc.first + x.first, s = acc.second + x.second;
      std::sort(f.begin(), f.end());
      std::sort(s.begin(), s.end());
      return std::pair<string, string>{f, s};
    });
  };

  using IntP = std::optional<std::pair<int, int>>;
  using StringP = std::optional<std::pair<string, string>>;
  using FooP = std::optional<std::pair<int, Foo>>;
  RUN_CHECK(int, int, IntP, "", {{1, 4}, {2, 2}, {3, 3}, {4, 1}, {5, 5}}, {{15, 15}}, intReduceOp);
  RUN_CHECK(string, string, StringP, "", {{"a", "banana"}, {"b", "cherry"}, {"c", "apple"}}, {{"abc", "aaaabceehlnnpprry"}},
            stringReduceOp);
  RUN_CHECK(int, Foo, FooP, "", {{1, Foo(3)}, {2, Foo(2)}, {3, Foo(1)}}, {{6, Foo(6)}}, fooReduceOp);

  RUN_CHECK(int, int, IntP, "", {{1, 4}}, {{1, 4}}, intReduceOp);
  RUN_CHECK(string, string, StringP, "", {{"a", "banana"}}, {{"a", "banana"}}, stringReduceOp);
  RUN_CHECK(int, Foo, FooP, "", {{1, Foo(3)}}, {{1, Foo(3)}}, fooReduceOp);

  RUN_CHECK(int, int, IntP, "", {}, std::nullopt, intReduceOp);
  RUN_CHECK(string, string, StringP, "", {}, std::nullopt, stringReduceOp);
  RUN_CHECK(int, Foo, FooP, "", {}, std::nullopt, fooReduceOp);
}
#endif

#ifndef DISABLE_TAP_EACH
TEST_CASE(TPE_NAME "_tap_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto eachOp = [](auto && xs) {
    using P = std::decay_t<typename std::decay_t<decltype(xs)>::value_type>;

    std::vector<std::pair<std::remove_const_t<typename P::first_type>, typename P::second_type>> out;
    auto ys = xs OP_ tap_each([&](auto x) { out.push_back(x); });
    CHECK((ys == xs));
    return out;
  };
  using IntP = std::vector<std::pair<int, int>>;
  using StringP = std::vector<std::pair<string, string>>;
  using FooP = std::vector<std::pair<Foo, Foo>>;

  RUN_CHECK(int, int, IntP, "", {{1, 4}, {2, 2}, {3, 3}, {4, 1}, {5, 5}}, {{1, 4}, {2, 2}, {3, 3}, {4, 1}, {5, 5}}, eachOp);
  RUN_CHECK(int, int, IntP, "", {{1, 1}}, {{1, 1}}, eachOp);
  RUN_CHECK(int, int, IntP, "", {}, {}, eachOp);
  RUN_CHECK(string, string, StringP, "", {{"a", "banana"}, {"b", "cherry"}, {"c", "apple"}},
            {{"a", "banana"}, {"b", "cherry"}, {"c", "apple"}}, eachOp);
  RUN_CHECK(string, string, StringP, "", {{"a", "apple"}}, {{"a", "apple"}}, eachOp);
  RUN_CHECK(string, string, StringP, "", {}, {}, eachOp);
  RUN_CHECK(Foo, Foo, FooP, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}},
            {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}}, eachOp);
  RUN_CHECK(Foo, Foo, FooP, "", {{Foo(1), Foo(1)}}, {{Foo(1), Foo(1)}}, eachOp);
  RUN_CHECK(Foo, Foo, FooP, "", {}, {}, eachOp);

  auto p2 = [](auto name, auto f) {
    RUN_CHECK(int, int, std::vector<int>, name, {{3, 1}, {2, 2}, {1, 3}}, {4, 4, 4}, f);
    RUN_CHECK(int, int, std::vector<int>, name, {{3, 1}}, {4}, f);
    RUN_CHECK(int, int, std::vector<int>, name, {}, {}, f);
  };
  p2("spread", [](auto && xs) {
    std::vector<int> out;
    auto ys = xs OP_ tap_each([&](auto x0, auto x1) { out.push_back(x0 + x1); });
    CHECK((ys == xs));
    return out;
  });
  p2("single", [](auto && xs) {
    std::vector<int> out;
    auto ys = xs OP_ tap_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x)); });
    CHECK((ys == xs));
    return out;
  });
}
#endif

#ifndef DISABLE_FOR_EACH
TEST_CASE(TPE_NAME "_for_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto eachOp = [](auto && xs) {
    using P = std::decay_t<typename std::decay_t<decltype(xs)>::value_type>;
    std::vector<std::pair<std::remove_const_t<typename P::first_type>, typename P::second_type>> out;
    xs OP_ for_each([&](auto x) { out.push_back(x); });
    return out;
  };
  using IntP = std::vector<std::pair<int, int>>;
  using StringP = std::vector<std::pair<string, string>>;
  using FooP = std::vector<std::pair<Foo, Foo>>;

  RUN_CHECK(int, int, IntP, "", {{1, 4}, {2, 2}, {3, 3}, {4, 1}, {5, 5}}, {{1, 4}, {2, 2}, {3, 3}, {4, 1}, {5, 5}}, eachOp);
  RUN_CHECK(int, int, IntP, "", {{1, 1}}, {{1, 1}}, eachOp);
  RUN_CHECK(int, int, IntP, "", {}, {}, eachOp);
  RUN_CHECK(string, string, StringP, "", {{"a", "banana"}, {"b", "cherry"}, {"c", "apple"}},
            {{"a", "banana"}, {"b", "cherry"}, {"c", "apple"}}, eachOp);
  RUN_CHECK(string, string, StringP, "", {{"a", "apple"}}, {{"a", "apple"}}, eachOp);
  RUN_CHECK(string, string, StringP, "", {}, {}, eachOp);
  RUN_CHECK(Foo, Foo, FooP, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}},
            {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}, {Foo(1), Foo(1)}}, eachOp);
  RUN_CHECK(Foo, Foo, FooP, "", {{Foo(1), Foo(1)}}, {{Foo(1), Foo(1)}}, eachOp);
  RUN_CHECK(Foo, Foo, FooP, "", {}, {}, eachOp);

  auto p2 = [](auto name, auto f) {
    RUN_CHECK(int, int, std::vector<int>, name, {{3, 1}, {2, 2}, {1, 3}}, {4, 4, 4}, f);
    RUN_CHECK(int, int, std::vector<int>, name, {{3, 1}}, {4}, f);
    RUN_CHECK(int, int, std::vector<int>, name, {}, {}, f);
  };
  p2("spread", [](auto && xs) {
    std::vector<int> out;
    auto ys = xs OP_ tap_each([&](auto x0, auto x1) { out.push_back(x0 + x1); });
    CHECK((ys == xs));
    return out;
  });
  p2("single", [](auto && xs) {
    std::vector<int> out;
    auto ys = xs OP_ tap_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x)); });
    CHECK((ys == xs));
    return out;
  });
}
#endif

#ifndef DISABLE_PARTITION
TEST_CASE(TPE_NAME "_partition", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intPartitionOp = [](auto && xs) { return xs OP_ partition([&](auto x) { return x.second % 2 == 0; }); };
  auto stringPartitionOp = [](auto && xs) { return xs OP_ partition([&](auto x) { return x.second.size() > 1; }); };
  auto fooPartitionOp = [](auto && xs) { return xs OP_ partition([&](auto x) { return x.second.value % 2 == 0; }); };
  using IntP = std::pair<TPE_CTOR_OUT(int, int), TPE_CTOR_OUT(int, int)>;
  using StringP = std::pair<TPE_CTOR_OUT(string, string), TPE_CTOR_OUT(string, string)>;
  using FooP = std::pair<TPE_CTOR_OUT(int, Foo), TPE_CTOR_OUT(int, Foo)>;
  RUN_CHECK(int, int, IntP, "", {{1, 2}, {2, 3}, {3, 1}, {4, 5}, {5, 4}}, {{{1, 2}, {5, 4}}, {{2, 3}, {3, 1}, {4, 5}}}, intPartitionOp);
  RUN_CHECK(string, string, StringP, "", {{"a", "a"}, {"b", "aa"}, {"c", "aaa"}}, {{{"b", "aa"}, {"c", "aaa"}}, {{"a", "a"}}},
            stringPartitionOp);
  RUN_CHECK(int, Foo, FooP, "", {{1, Foo(2)}, {2, Foo(3)}, {3, Foo(1)}, {4, Foo(5)}, {5, Foo(4)}},
            {{{1, Foo(2)}, {5, Foo(4)}}, {{2, Foo(3)}, {3, Foo(1)}, {4, Foo(5)}}}, fooPartitionOp);

  RUN_CHECK(int, int, IntP, "", {{1, 4}}, {{{1, 4}}, {}}, intPartitionOp);
  RUN_CHECK(int, int, IntP, "", {{1, 5}}, {{}, {{1, 5}}}, intPartitionOp);
  RUN_CHECK(int, int, IntP, "", {}, {{}, {}}, intPartitionOp);

  RUN_CHECK(string, string, StringP, "", {{"a", "a"}}, {{}, {{"a", "a"}}}, stringPartitionOp);
  RUN_CHECK(string, string, StringP, "", {{"b", "aa"}}, {{{"b", "aa"}}, {}}, stringPartitionOp);
  RUN_CHECK(string, string, StringP, "", {}, {{}, {}}, stringPartitionOp);

  RUN_CHECK(int, Foo, FooP, "", {{1, Foo(4)}}, {{{1, Foo(4)}}, {}}, fooPartitionOp);
  RUN_CHECK(int, Foo, FooP, "", {{1, Foo(5)}}, {{}, {{1, Foo(5)}}}, fooPartitionOp);
  RUN_CHECK(int, Foo, FooP, "", {}, {{}, {}}, fooPartitionOp);

  auto p2 = [](auto name, auto f) {
    using IntP2 = std::pair<TPE_CTOR_OUT(int, int), TPE_CTOR_OUT(int, int)>;

    RUN_CHECK(int, int, IntP2, name, {{3, 1}, {2, 2}, {1, 3}}, {{{2, 2}}, {{3, 1}, {1, 3}}}, f);
    RUN_CHECK(int, int, IntP2, name, {{3, 1}}, {{}, {{3, 1}}}, f);
    RUN_CHECK(int, int, IntP2, name, {}, {{}, {}}, f);
  };
  p2("spread", [](auto && xs) { return xs OP_ partition([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto && xs) { return xs OP_ partition([](auto x) { return get<0>(x) == get<1>(x); }); });
}
#endif

#ifndef DISABLE_GROUP_MAP_REDUCE
TEST_CASE(TPE_NAME "_group_map_reduce", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto p2 = [](auto intGMROp, auto stringGMROp, auto fooGMROp) {
    RUN_CHECK(int, int, std::unordered_map<int COMMA int>, "", {{13, 3}, {23, 5}, {12, 2}}, {{3, 16}, {2, 4}}, intGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA string>, "", {{"apple", "a"}, {"ant", "b"}, {"banana", "c"}},
              {{"a", "XXab"}, {"b", "cX"}}, stringGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA int>, "", {{Foo(11), Foo(2)}, {Foo(21), Foo(3)}, {Foo(12), Foo(4)}},
              {{Foo(1), 15}, {Foo(2), 12}}, fooGMROp);
    RUN_CHECK(int, int, std::unordered_map<int COMMA int>, "", {{3, 3}}, {{3, 6}}, intGMROp);
    RUN_CHECK(int, int, std::unordered_map<int COMMA int>, "", {}, {}, intGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA string>, "", {{"apple", "a"}}, {{"a", "aX"}}, stringGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA string>, "", {}, {}, stringGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA int>, "", {{Foo(1), Foo(2)}}, {{Foo(1), 6}}, fooGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA int>, "", {}, {}, fooGMROp);
  };

  p2(
      [](auto && xs) {
        return xs OP_ group_map_reduce([](auto x) { return x.first % 10; }, //
                                       [](auto x) { return x.second * 2; }, //
                                       [](auto x, auto y) { return x + y; });
      },
      [](auto && xs) {
        return xs OP_ group_map_reduce([](auto x) { return x.first.substr(0, 1); }, //
                                       [](auto x) { return x.second + "X"; },       //
                                       [](auto x, auto y) {
                                         auto out = x + y;
                                         std::sort(out.begin(), out.end());
                                         return out;
                                       });
      },
      [](auto && xs) {
        return xs OP_ group_map_reduce([](auto x) { return Foo(x.first.value % 10); }, //
                                       [](auto x) { return x.second.value * 3; },      //
                                       [](auto x, auto y) { return x + y; });
      });
  p2(
      [](auto && xs) {
        return xs OP_ group_map_reduce([](auto x, auto) { return x % 10; }, //
                                       [](auto, auto y) { return y * 2; },  //
                                       [](auto x, auto y) { return x + y; });
      },
      [](auto && xs) {
        return xs OP_ group_map_reduce([](auto x, auto) { return x.substr(0, 1); }, //
                                       [](auto, auto y) { return y + "X"; },        //
                                       [](auto x, auto y) {
                                         auto out = x + y;
                                         std::sort(out.begin(), out.end());
                                         return out;
                                       });
      },
      [](auto && xs) {
        return xs OP_ group_map_reduce([](auto x, auto) { return Foo(x.value % 10); }, //
                                       [](auto, auto y) { return y.value * 3; },       //
                                       [](auto x, auto y) { return x + y; });
      });
}
#endif

#ifndef DISABLE_GROUP_MAP
TEST_CASE(TPE_NAME "_group_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto p2 = [](auto intGMROp, auto stringGMROp, auto fooGMROp) {
    RUN_CHECK(int, int, std::unordered_map<int COMMA TPE_CTOR_VAR_OUT(int)>, "", {{13, 3}, {23, 5}, {12, 2}}, {{3, {6, 10}}, {2, {4}}},
              intGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA TPE_CTOR_VAR_OUT(string)>, "",
              {{"apple", "a"}, {"ant", "b"}, {"banana", "c"}}, {{"a", {"aX", "bX"}}, {"b", {"cX"}}}, stringGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA TPE_CTOR_VAR_OUT(int)>, "", {{Foo(11), Foo(2)}, {Foo(21), Foo(3)}, {Foo(12), Foo(4)}},
              {{Foo(1), {6, 9}}, {Foo(2), {12}}}, fooGMROp);
    RUN_CHECK(int, int, std::unordered_map<int COMMA TPE_CTOR_VAR_OUT(int)>, "", {{3, 3}}, {{3, {6}}}, intGMROp);
    RUN_CHECK(int, int, std::unordered_map<int COMMA TPE_CTOR_VAR_OUT(int)>, "", {}, {}, intGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA TPE_CTOR_VAR_OUT(string)>, "", {{"apple", "a"}}, {{"a", {"aX"}}},
              stringGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA TPE_CTOR_VAR_OUT(string)>, "", {}, {}, stringGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA TPE_CTOR_VAR_OUT(int)>, "", {{Foo(1), Foo(2)}}, {{Foo(1), {6}}}, fooGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA TPE_CTOR_VAR_OUT(int)>, "", {}, {}, fooGMROp);
  };

  p2(
      [](auto && xs) {
        return xs OP_ group_map([](auto x) { return x.first % 10; }, //
                                [](auto x) { return x.second * 2; });
      },
      [](auto && xs) {
        return xs OP_ group_map([](auto x) { return x.first.substr(0, 1); }, //
                                [](auto x) { return x.second + "X"; });
      },
      [](auto && xs) {
        return xs OP_ group_map([](auto x) { return Foo(x.first.value % 10); }, //
                                [](auto x) { return x.second.value * 3; });
      });
  p2(
      [](auto && xs) {
        return xs OP_ group_map([](auto x, auto) { return x % 10; }, //
                                [](auto, auto y) { return y * 2; });
      },
      [](auto && xs) {
        return xs OP_ group_map([](auto x, auto) { return x.substr(0, 1); }, //
                                [](auto, auto y) { return y + "X"; });
      },
      [](auto && xs) {
        return xs OP_ group_map([](auto x, auto) { return Foo(x.value % 10); }, //
                                [](auto, auto y) { return y.value * 3; });
      });
}
#endif

#ifndef DISABLE_GROUP_BY
TEST_CASE(TPE_NAME "_group_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto p2 = [](auto intGMROp, auto stringGMROp, auto fooGMROp) {
    using IntP = std::pair<int, int>;
    using StringP = std::pair<string, string>;
    using FooP = std::pair<Foo, Foo>;

    RUN_CHECK(int, int, std::unordered_map<int COMMA TPE_CTOR_VAR_OUT(IntP)>, "", {{13, 3}, {23, 5}, {12, 2}},
              {{3, {{13, 3}, {23, 5}}}, {2, {{12, 2}}}}, intGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA TPE_CTOR_VAR_OUT(StringP)>, "",
              {{"apple", "a"}, {"ant", "b"}, {"banana", "c"}}, {{"a", {{"apple", "a"}, {"ant", "b"}}}, {"b", {{"banana", "c"}}}},
              stringGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA TPE_CTOR_VAR_OUT(FooP)>, "", {{Foo(11), Foo(2)}, {Foo(21), Foo(3)}, {Foo(12), Foo(4)}},
              {{Foo(1), {{Foo(11), Foo(2)}, {Foo(21), Foo(3)}}}, {Foo(2), {{Foo(12), Foo(4)}}}}, fooGMROp);
    RUN_CHECK(int, int, std::unordered_map<int COMMA TPE_CTOR_VAR_OUT(IntP)>, "", {{3, 3}}, {{3, {{3, 3}}}}, intGMROp);
    RUN_CHECK(int, int, std::unordered_map<int COMMA TPE_CTOR_VAR_OUT(IntP)>, "", {}, {}, intGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA TPE_CTOR_VAR_OUT(StringP)>, "", {{"apple", "a"}}, {{"a", {{"apple", "a"}}}},
              stringGMROp);
    RUN_CHECK(string, string, std::unordered_map<string COMMA TPE_CTOR_VAR_OUT(StringP)>, "", {}, {}, stringGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA TPE_CTOR_VAR_OUT(FooP)>, "", {{Foo(1), Foo(2)}}, {{Foo(1), {{Foo(1), Foo(2)}}}},
              fooGMROp);
    RUN_CHECK(Foo, Foo, std::unordered_map<Foo COMMA TPE_CTOR_VAR_OUT(FooP)>, "", {}, {}, fooGMROp);
  };

  p2([](auto && xs) { return xs OP_ group_by([](auto x) { return x.first % 10; }); },
     [](auto && xs) { return xs OP_ group_by([](auto x) { return x.first.substr(0, 1); }); },
     [](auto && xs) { return xs OP_ group_by([](auto x) { return Foo(x.first.value % 10); }); });
  p2([](auto && xs) { return xs OP_ group_by([](auto x, auto) { return x % 10; }); },
     [](auto && xs) { return xs OP_ group_by([](auto x, auto) { return x.substr(0, 1); }); },
     [](auto && xs) { return xs OP_ group_by([](auto x, auto) { return Foo(x.value % 10); }); });
}
#endif

#ifndef DISABLE_TO_VECTOR
TEST_CASE(TPE_NAME "_to_vector", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto && xs) { return xs OP_ to_vector(); };

  using IntP = std::pair<int, int>;
  using StringP = std::pair<string, string>;
  using StringFooP = std::pair<string, Foo>;

  RUN_CHECK(int, int, std::vector<IntP>, "", {{1, 4}, {2, 2}, {3, 3}}, {{1, 4}, {2, 2}, {3, 3}}, op);
  RUN_CHECK(string, string, std::vector<StringP>, "", {{"banana", "B"}, {"cherry", "C"}, {"apple", "A"}},
            {{"apple", "A"}, {"banana", "B"}, {"cherry", "C"}}, op);
  RUN_CHECK(string, Foo, std::vector<StringFooP>, "", {{"apple", Foo(1)}, {"banana", Foo(2)}}, {{"apple", Foo(1)}, {"banana", Foo(2)}}, op);

  RUN_CHECK(int, int, std::vector<IntP>, "", {{1, 1}}, {{1, 1}}, op);
  RUN_CHECK(int, int, std::vector<IntP>, "", {}, {}, op);
  RUN_CHECK(string, string, std::vector<StringP>, "", {{"apple", "A"}}, {{"apple", "A"}}, op);
  RUN_CHECK(string, string, std::vector<StringP>, "", {}, {}, op);
  RUN_CHECK(string, Foo, std::vector<StringFooP>, "", {{"apple", Foo(1)}}, {{"apple", Foo(1)}}, op);
  RUN_CHECK(string, Foo, std::vector<StringFooP>, "", {}, {}, op);
}
#endif

#ifndef DISABLE_KEYS
TEST_CASE(TPE_NAME "_keys", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto && xs) { return xs OP_ keys(); };
  RUN_CHECK(int, int, TPE_CTOR_VAR_OUT(int), "", {{2, 2}, {3, 3}, {1, 1}}, {2, 3, 1}, op);
  RUN_CHECK(string, string, TPE_CTOR_VAR_OUT(string), "", {{"banana", "banana"}, {"cherry", "cherry"}}, {"banana", "cherry"}, op);
  RUN_CHECK(Foo, Foo, TPE_CTOR_VAR_OUT(Foo), "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, {Foo(3), Foo(2)}, op);

  RUN_CHECK(int, int, TPE_CTOR_VAR_OUT(int), "", {{1, 1}}, {1}, op);
  RUN_CHECK(int, int, TPE_CTOR_VAR_OUT(int), "", {}, {}, op);
  RUN_CHECK(string, string, TPE_CTOR_VAR_OUT(string), "", {{"apple", "apple"}}, {"apple"}, op);
  RUN_CHECK(string, string, TPE_CTOR_VAR_OUT(string), "", {}, {}, op);
  RUN_CHECK(Foo, Foo, TPE_CTOR_VAR_OUT(Foo), "", {{Foo(1), Foo(1)}}, {Foo(1)}, op);
  RUN_CHECK(Foo, Foo, TPE_CTOR_VAR_OUT(Foo), "", {}, {}, op);
}
#endif

#ifndef DISABLE_VALUES
TEST_CASE(TPE_NAME "_values", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto && xs) { return xs OP_ values(); };
  RUN_CHECK(int, int, std::vector<int>, "", {{2, 2}, {3, 3}, {1, 1}}, {2, 3, 1}, op);
  RUN_CHECK(string, string, std::vector<string>, "", {{"banana", "banana"}, {"cherry", "cherry"}}, {"banana", "cherry"}, op);
  RUN_CHECK(Foo, Foo, std::vector<Foo>, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, {Foo(3), Foo(2)}, op);

  RUN_CHECK(int, int, std::vector<int>, "", {{1, 1}}, {1}, op);
  RUN_CHECK(int, int, std::vector<int>, "", {}, {}, op);
  RUN_CHECK(string, string, std::vector<string>, "", {{"apple", "apple"}}, {"apple"}, op);
  RUN_CHECK(string, string, std::vector<string>, "", {}, {}, op);
  RUN_CHECK(Foo, Foo, std::vector<Foo>, "", {{Foo(1), Foo(1)}}, {Foo(1)}, op);
  RUN_CHECK(Foo, Foo, std::vector<Foo>, "", {}, {}, op);
}
#endif

#ifndef DISABLE_MAP_KEYS
TEST_CASE(TPE_NAME "_map_keys", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto && xs) { return xs OP_ map_keys([](auto x) { return std::to_string(x * 2); }); };
  auto strOp = [](auto && xs) { return xs OP_ map_keys([](auto x) { return x + x; }); };
  auto fooOp = [](auto && xs) { return xs OP_ map_keys([](auto x) { return Foo(x.value * 2); }); };

  RUN_CHECK(int, int, TPE_CTOR_OUT(string, int), "", {{2, 2}, {3, 3}, {1, 1}}, {{"4", 2}, {"6", 3}, {"2", 1}}, intOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"banana", "banana"}, {"cherry", "cherry"}},
            {{"bananabanana", "banana"}, {"cherrycherry", "cherry"}}, strOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, {{Foo(6), Foo(3)}, {Foo(4), Foo(2)}}, fooOp);

  RUN_CHECK(int, int, TPE_CTOR_OUT(string, int), "", {{1, 1}}, {{"2", 1}}, intOp);
  RUN_CHECK(int, int, TPE_CTOR_OUT(string, int), "", {}, {}, intOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}}, {{"appleapple", "apple"}}, strOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {}, strOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(1), Foo(1)}}, {{Foo(2), Foo(1)}}, fooOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {}, {}, fooOp);
  auto p2 = [](auto name, auto f) {
    using IntP2 = std::pair<int, int>;
    RUN_CHECK(IntP2, string, TPE_CTOR_OUT(int, string), name, {{{3, 1}, "a"}, {{2, 1}, "b"}, {{2, 3}, "c"}},
        {{4, "a"}, {3, "b"}, {5, "c"}}, f);
    RUN_CHECK(IntP2, string, TPE_CTOR_OUT(int, string), name, {{{3, 1}, "a"}}, {{4, "a"}}, f);
    RUN_CHECK(IntP2, string, TPE_CTOR_OUT(int, string), name, {}, {}, f);
  };
  p2("spread", [](auto && xs) { return xs OP_ map_keys([](auto x, auto y) { return x + y; }); });
  p2("single", [](auto && xs) { return xs OP_ map_keys([](auto x) { return x.first + x.second; }); });
}
#endif

#ifndef DISABLE_MAP_VALUES
TEST_CASE(TPE_NAME "_map_values", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto && xs) { return xs OP_ map_values([](auto x) { return std::to_string(x * 2); }); };
  auto strOp = [](auto && xs) { return xs OP_ map_values([](auto x) { return x + x; }); };
  auto fooOp = [](auto && xs) { return xs OP_ map_values([](auto x) { return Foo(x.value * 2); }); };

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, string), "", {{2, 2}, {3, 3}, {1, 1}}, {{2, "4"}, {3, "6"}, {1, "2"}}, intOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"banana", "banana"}, {"cherry", "cherry"}},
            {{"banana", "bananabanana"}, {"cherry", "cherrycherry"}}, strOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, {{Foo(3), Foo(6)}, {Foo(2), Foo(4)}}, fooOp);

  RUN_CHECK(int, int, TPE_CTOR_OUT(int, string), "", {{1, 1}}, {{1, "2"}}, intOp);
  RUN_CHECK(int, int, TPE_CTOR_OUT(int, string), "", {}, {}, intOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {{"apple", "apple"}}, {{"apple", "appleapple"}}, strOp);
  RUN_CHECK(string, string, TPE_CTOR_OUT(string, string), "", {}, {}, strOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {{Foo(1), Foo(1)}}, {{Foo(1), Foo(2)}}, fooOp);
  RUN_CHECK(Foo, Foo, TPE_CTOR_OUT(Foo, Foo), "", {}, {}, fooOp);
  auto p2 = [](auto name, auto f) {
    using StringP2 = std::pair<string, string>;
    RUN_CHECK(int, StringP2, TPE_CTOR_OUT(int, char), name, {{1, {"a", "b"}}, {2, {"b", "c"}}, {3, {"c", "d"}}},
              {{{1, 'b'}, {2, 'c'}, {3, 'd'}}}, f);
    RUN_CHECK(int, StringP2, TPE_CTOR_OUT(int, char), name, {{1, {"a", "b"}}}, {{1, 'b'}}, f);
    RUN_CHECK(int, StringP2, TPE_CTOR_OUT(int, char), name, {}, {}, f);
  };
  p2("spread", [](auto && xs) { return xs OP_ map_values([](auto, auto y) { return y[0]; }); });
  p2("single", [](auto && xs) { return xs OP_ map_values([](auto x) { return x.second[0]; }); });
}
#endif

#ifndef DISABLE_GET
TEST_CASE(TPE_NAME "_get", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto k) { return [=](auto && xs) { return xs OP_ get(k); }; };

  RUN_CHECK(int, int, std::optional<int>, "", {{2, 2}, {3, 3}, {1, 1}}, {3}, op(3));
  RUN_CHECK(int, int, std::optional<int>, "", {{2, 2}, {3, 3}, {1, 1}}, {}, op(10));

  RUN_CHECK(string, string, std::optional<string>, "", {{"banana", "banana"}, {"cherry", "cherry"}}, {"cherry"}, op("cherry"));
  RUN_CHECK(string, string, std::optional<string>, "", {{"banana", "banana"}, {"cherry", "cherry"}}, {}, op(""));
  RUN_CHECK(Foo, Foo, std::optional<Foo>, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, {Foo(3)}, op(Foo(3)));
  RUN_CHECK(Foo, Foo, std::optional<Foo>, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, {}, op(Foo(42)));

  RUN_CHECK(int, int, std::optional<int>, "", {}, {}, op(0));
  RUN_CHECK(string, string, std::optional<string>, "", {}, {}, op(""));
  RUN_CHECK(Foo, Foo, std::optional<Foo>, "", {}, {}, op(Foo(1)));
}
#endif

#ifndef DISABLE_GET_OR_DEFAULT
TEST_CASE(TPE_NAME "_get_or_default", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto k, auto x) { return [=](auto && xs) { return xs OP_ get_or_default(k, x); }; };

  RUN_CHECK(int, int, int, "", {{2, 2}, {3, 3}, {1, 1}}, 3, op(3, 42));
  RUN_CHECK(int, int, int, "", {{2, 2}, {3, 3}, {1, 1}}, 42, op(10, 42));

  RUN_CHECK(string, string, string, "", {{"banana", "banana"}, {"cherry", "cherry"}}, "cherry", op("cherry", "a"));
  RUN_CHECK(string, string, string, "", {{"banana", "banana"}, {"cherry", "cherry"}}, "a", op("", "a"));
  RUN_CHECK(Foo, Foo, Foo, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, Foo(3), op(Foo(3), Foo(42)));
  RUN_CHECK(Foo, Foo, Foo, "", {{Foo(3), Foo(3)}, {Foo(2), Foo(2)}}, Foo(42), op(Foo(42), Foo(42)));

  RUN_CHECK(int, int, int, "", {}, 1, op(0, 1));
  RUN_CHECK(string, string, string, "", {}, "a", op("", "a"));
  RUN_CHECK(Foo, Foo, Foo, "", {}, Foo(42), op(Foo(1), Foo(42)));
}
#endif