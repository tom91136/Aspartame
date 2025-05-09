#include <functional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../test_base_container1.hpp"
#include "catch2/catch_test_macros.hpp"

#ifndef DISABLE_MK_STRING
TEST_CASE(TPE_NAME "_mk_string", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ mk_string(); };
  RUN_CHECK(int, string, "", {4, 2, 3, 1, 5}, "42315", op);
  RUN_CHECK(int, string, "", {1}, "1", op);
  RUN_CHECK(int, string, "", {}, "", op);
  RUN_CHECK(string, string, "", {"banana", "cherry", "apple"}, "bananacherryapple", op);
  RUN_CHECK(string, string, "", {"apple"}, "apple", op);
  RUN_CHECK(string, string, "", {}, "", op);
  RUN_CHECK(Foo, string, "", {Foo(3), Foo(2), Foo(1)}, "Foo(3)Foo(2)Foo(1)", op);
  RUN_CHECK(Foo, string, "", {Foo(1)}, "Foo(1)", op);
  RUN_CHECK(Foo, string, "", {}, "", op);

  auto customOp = [](auto &&xs) { return xs OP_ mk_string(", "); };
  RUN_CHECK(int, string, "", {4, 2, 3, 1, 5}, "4, 2, 3, 1, 5", customOp);
  RUN_CHECK(int, string, "", {1}, "1", customOp);
  RUN_CHECK(int, string, "", {}, "", customOp);
  RUN_CHECK(string, string, "", {"banana", "cherry", "apple"}, "banana, cherry, apple", customOp);
  RUN_CHECK(string, string, "", {"apple"}, "apple", customOp);
  RUN_CHECK(string, string, "", {}, "", customOp);
  RUN_CHECK(Foo, string, "", {Foo(3), Foo(2), Foo(1)}, "Foo(3), Foo(2), Foo(1)", customOp);
  RUN_CHECK(Foo, string, "", {Foo(1)}, "Foo(1)", customOp);
  RUN_CHECK(Foo, string, "", {}, "", customOp);

  auto customOp2 = [](auto &&xs) { return xs OP_ mk_string("[", ", ", "]"); };
  RUN_CHECK(int, string, "", {4, 2, 3, 1, 5}, "[4, 2, 3, 1, 5]", customOp2);
  RUN_CHECK(int, string, "", {1}, "[1]", customOp2);
  RUN_CHECK(int, string, "", {}, "[]", customOp2);
  RUN_CHECK(string, string, "", {"banana", "cherry", "apple"}, "[banana, cherry, apple]", customOp2);
  RUN_CHECK(string, string, "", {"apple"}, "[apple]", customOp2);
  RUN_CHECK(string, string, "", {}, "[]", customOp2);
  RUN_CHECK(Foo, string, "", {Foo(3), Foo(2), Foo(1)}, "[Foo(3), Foo(2), Foo(1)]", customOp2);
  RUN_CHECK(Foo, string, "", {Foo(1)}, "[Foo(1)]", customOp2);
  RUN_CHECK(Foo, string, "", {}, "[]", customOp2);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
    RUN_CHECK(P2, string, name, {{3, 1}, {2, 2}, {1, 3}}, "4,4,4", f);
    RUN_CHECK(P2, string, name, {{3, 1}}, "4", f);
    RUN_CHECK(P2, string, name, {}, "", f);
  };
  p2("spread", [](auto &&xs) { return xs OP_ mk_string(",", [](auto x0, auto x1) { return to_string(x0 + x1); }); });
  p2("single", [](auto &&xs) { return xs OP_ mk_string(",", [](auto x) { return to_string(get<0>(x) + get<1>(x)); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, string, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, "7,6,5", f);
    RUN_CHECK(P3, string, name, {{3, 1, 3}}, "7", f);
    RUN_CHECK(P3, string, name, {}, "", f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ mk_string(",", [](auto x0, auto x1, auto x2) { return to_string(x0 + x1 + x2); }); });
  p3("single", [](auto &&xs) { return xs OP_ mk_string(",", [](auto x) { return to_string(get<0>(x) + get<1>(x) + get<2>(x)); }); });
}
#endif

#ifndef DISABLE_APPEND
TEST_CASE(TPE_NAME "_append", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto x) { return [x](auto &&xs) { return xs OP_ append(x); }; };
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
TEST_CASE(TPE_NAME "_concat", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto x) { return [x](auto &&xs) { return xs OP_ concat(x); }; };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5, 6, 7, 8}, op(TPE_CTOR_IN(int){6, 7, 8}));
  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, op(TPE_CTOR_IN(int){}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple", "mango", "kiwi"},
            op(TPE_CTOR_IN(string){"mango", "kiwi"}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple"}, op(TPE_CTOR_IN(string){}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1), Foo(4), Foo(5)},
            op(TPE_CTOR_IN(Foo){Foo(4), Foo(5)}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, op(TPE_CTOR_IN(Foo){}));
  #endif

  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {}, {}, op(TPE_CTOR_IN(int){}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {}, {}, op(TPE_CTOR_IN(string){}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {}, {}, op(TPE_CTOR_IN(Foo){}));

  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {}, {1}, op(TPE_CTOR_IN(int){1}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {}, {"pear"}, op(TPE_CTOR_IN(string){"pear"}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {}, {Foo(7)}, op(TPE_CTOR_IN(Foo){Foo(7)}));

  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {1}, {1, 1}, op(TPE_CTOR_IN(int){1}));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {"pear"}, {"pear", "pear"}, op(TPE_CTOR_IN(string){"pear"}));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {Foo(7)}, {Foo(7), Foo(7)}, op(TPE_CTOR_IN(Foo){Foo(7)}));
}
#endif

#ifndef DISABLE_MAP
TEST_CASE(TPE_NAME "_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ map([](auto x) { return x * 2; }); };
  auto strOp = [](auto &&xs) { return xs OP_ map([](auto x) { return x + x; }); };
  auto fooOp = [](auto &&xs) { return xs OP_ map([](auto x) { return Foo(x.value * 2); }); };
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
  p2("spread", [](auto &&xs) { return xs OP_ map([](auto x0, auto x1) { return x0 + x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ map([](auto x) { return get<0>(x) + get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {7, 6, 5}, f);
  #endif
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}}, {7}, f);
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ map([](auto x0, auto x1, auto x2) { return x0 + x1 + x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ map([](auto x) { return get<0>(x) + get<1>(x) + get<2>(x); }); });
}
#endif

#ifndef DISABLE_STATIC_AS
TEST_CASE(TPE_NAME "_static_as", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ static_as<size_t>(); };
  auto strOp = [](auto &&xs) { return xs OP_ static_as<MyString>(); };
  auto fooOp = [](auto &&xs) { return xs OP_ static_as<MyFoo>(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_OUT(size_t), "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, intOp);
  RUN_CHECK(string, TPE_CTOR_OUT(MyString), "", {"banana", "cherry", "apple"}, //
            {MyString("banana"), MyString("cherry"), MyString("apple")}, strOp);
  RUN_CHECK(Foo, TPE_CTOR_OUT(MyFoo), "", {Foo(3), Foo(2), Foo(1)}, {MyFoo(Foo{3}), MyFoo(Foo{2}), MyFoo(Foo{1})}, fooOp);
  #endif

  RUN_CHECK(int, TPE_CTOR_OUT(size_t), "", {1}, {1}, intOp);
  RUN_CHECK(int, TPE_CTOR_OUT(size_t), "", {}, {}, intOp);
  RUN_CHECK(string, TPE_CTOR_OUT(MyString), "", {"apple"}, {MyString("apple")}, strOp);
  RUN_CHECK(string, TPE_CTOR_OUT(MyString), "", {}, {}, strOp);
  RUN_CHECK(Foo, TPE_CTOR_OUT(MyFoo), "", {Foo(1)}, {MyFoo{Foo(1)}}, fooOp);
  RUN_CHECK(Foo, TPE_CTOR_OUT(MyFoo), "", {}, {}, fooOp);
}
#endif

#ifndef DISABLE_CONST_AS
TEST_CASE(TPE_NAME "_const_as", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ const_as<int *>(); };
  auto strOp = [](auto &&xs) { return xs OP_ const_as<string *>(); };
  auto fooOp = [](auto &&xs) { return xs OP_ const_as<Foo *>(); };

  auto i = new int(4);
  auto s = new string("a");
  auto f = new Foo(42);
  #ifdef TPE_MANY_INIT
  RUN_CHECK(const int *, TPE_CTOR_OUT(int *), "", {i, i, i}, {i, i, i}, intOp);
  RUN_CHECK(const string *, TPE_CTOR_OUT(string *), "", {s, s, s}, {s, s, s}, strOp);
  RUN_CHECK(const Foo *, TPE_CTOR_OUT(Foo *), "", {f, f, f}, {f, f, f}, fooOp);
  #endif

  RUN_CHECK(const int *, TPE_CTOR_OUT(int *), "", {i}, {i}, intOp);
  RUN_CHECK(const int *, TPE_CTOR_OUT(int *), "", {}, {}, intOp);
  RUN_CHECK(const string *, TPE_CTOR_OUT(string *), "", {s}, {s}, strOp);
  RUN_CHECK(const string *, TPE_CTOR_OUT(string *), "", {}, {}, strOp);
  RUN_CHECK(const Foo *, TPE_CTOR_OUT(Foo *), "", {f}, {f}, fooOp);
  RUN_CHECK(const Foo *, TPE_CTOR_OUT(Foo *), "", {}, {}, fooOp);
  delete i;
  delete s;
  delete f;
}
#endif

#ifndef DISABLE_CONST_AS
TEST_CASE(TPE_NAME "_reinterpret_as", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ reinterpret_as<const char *>() OP_ static_as<string>(); };
  auto i = new int('*' | ('\0' << 8));
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int *, TPE_CTOR_OUT(string), "", {i, i, i}, {"*", "*", "*"}, op);
  #endif
  RUN_CHECK(int *, TPE_CTOR_OUT(string), "", {i}, {"*"}, op);
  delete i;
}
#endif

#ifndef DISABLE_COLLECT
TEST_CASE(TPE_NAME "_collect", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) {
    return xs OP_ collect([](auto x) -> std::optional<string> { //
      return x % 2 != 0 ? std::nullopt : std::optional{to_string(x)};
    });
  };
  auto strOp = [](auto &&xs) {
    return xs OP_ collect([](auto x) -> std::optional<string> { //
      return x == "cherry" ? std::optional{"a"} : std::nullopt;
    });
  };
  auto fooOp = [](auto &&xs) {
    return xs OP_ collect([](auto x) -> std::optional<string> { //
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
  p2("spread", [](auto &&xs) {
    return xs OP_ collect([](auto x0, auto x1) {
      int x = x0 + x1;
      return x % 2 != 0 ? std::nullopt : std::optional{to_string(x)};
    });
  });
  p2("single", [](auto &&xs) {
    return xs OP_ collect([](auto x) {
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
  p3("spread", [](auto &&xs) {
    return xs OP_ collect([](auto x0, auto x1, auto x2) {
      int x = x0 + x1 + x2;
      return x % 2 != 0 ? std::nullopt : std::optional{to_string(x)};
    });
  });
  p3("single", [](auto &&xs) {
    return xs OP_ collect([](auto x) {
      int v = get<0>(x) + get<1>(x) + get<2>(x);
      return v % 2 != 0 ? std::nullopt : std::optional{to_string(v)};
    });
  });
}
#endif

#ifndef DISABLE_FILTER
TEST_CASE(TPE_NAME "_filter", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ filter([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto &&xs) { return xs OP_ filter([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto &&xs) { return xs OP_ filter([](auto x) { return x.value % 2 == 0; }); };

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
  p2("spread", [](auto &&xs) { return xs OP_ filter([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ filter([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK_ID(P3, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {{2, 2, 2}}, f);
  #endif
    RUN_CHECK_ID(P3, name, {{3, 3, 3}}, {{3, 3, 3}}, f);
    RUN_CHECK_ID(P3, name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ filter([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ filter([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_FLAT_MAP
TEST_CASE(TPE_NAME "_flat_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto bindOp = [](auto &&xs) { return xs OP_ flat_map([](auto x) { return TPE_CTOR_OUT(decltype(x)){x}; }); };
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
  p2("spread", [](auto &&xs) { return xs OP_ flat_map([](auto x0, auto x1) { return TPE_CTOR_OUT(decltype(x0)){x0 + x1}; }); });
  p2("single",
     [](auto &&xs) { return xs OP_ flat_map([](auto x) { return TPE_CTOR_OUT(std::decay_t<decltype(get<0>(x))>){get<0>(x) + get<1>(x)}; }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {3 + 1 + 3, 2 + 2 + 2, 1 + 3 + 1}, f);
  #endif
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}}, {3 + 1 + 3}, f);
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ flat_map([](auto x0, auto x1, auto x2) { return TPE_CTOR_OUT(decltype(x0)){x0 + x1 + x2}; }); });
  p3("single", [](auto &&xs) {
    return xs OP_ flat_map([](auto x) { return TPE_CTOR_OUT(std::decay_t<decltype(get<0>(x))>){get<0>(x) + get<1>(x) + get<2>(x)}; });
  });
}
#endif

#ifndef DISABLE_FLATTEN
TEST_CASE(TPE_NAME "_flatten", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto flattenOp = [](auto &&xs) { return xs OP_ flatten(); };
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
TEST_CASE(TPE_NAME "_distinct", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ distinct(); };
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
TEST_CASE(TPE_NAME "_distinct_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ distinct_by([](auto x) { return x % 10; }); };
  auto strOp = [](auto &&xs) { return xs OP_ distinct_by([](auto x) { return x.length(); }); };
  auto fooOp = [](auto &&xs) { return xs OP_ distinct_by([](auto x) { return x.value % 10; }); };
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
TEST_CASE(TPE_NAME "_count", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ count([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto &&xs) { return xs OP_ count([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto &&xs) { return xs OP_ count([](auto x) { return x.value % 2 == 0; }); };
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
  p2("spread", [](auto &&xs) { return xs OP_ count([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ count([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, size_t, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, 1, f);
    RUN_CHECK(P3, size_t, name, {{3, 1, 3}}, 0, f);
    RUN_CHECK(P3, size_t, name, {}, 0, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ count([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ count([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_EXISTS
TEST_CASE(TPE_NAME "_exists", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ exists([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto &&xs) { return xs OP_ exists([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto &&xs) { return xs OP_ exists([](auto x) { return x.value % 2 == 0; }); };
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
  p2("spread", [](auto &&xs) { return xs OP_ exists([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ exists([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, bool, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, true, f);
    RUN_CHECK(P3, bool, name, {{3, 1, 3}}, false, f);
    RUN_CHECK(P3, bool, name, {}, false, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ exists([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ exists([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_FORALL
TEST_CASE(TPE_NAME "_forall", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ forall([](auto x) { return x % 2 == 0; }); };
  auto strOp = [](auto &&xs) { return xs OP_ forall([](auto x) { return x == "cherry"; }); };
  auto fooOp = [](auto &&xs) { return xs OP_ forall([](auto x) { return x.value % 2 == 0; }); };
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
  p2("spread", [](auto &&xs) { return xs OP_ forall([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ forall([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, bool, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, false, f);
    RUN_CHECK(P3, bool, name, {{3, 3, 3}}, true, f);
    RUN_CHECK(P3, bool, name, {}, true, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ forall([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ forall([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_FIND
TEST_CASE(TPE_NAME "_find", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, {4}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == 4; }); });
  RUN_CHECK(int, std::optional<int>, "", {1}, {1}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == 1; }); });
  RUN_CHECK(int, std::optional<int>, "", {1}, {}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == 0; }); });
  RUN_CHECK(int, std::optional<int>, "", {}, {}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == 1; }); });

  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, {"banana"},
            [](auto &&xs) { return xs OP_ find([](auto x) { return x == "banana"; }); });
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {"apple"},
            [](auto &&xs) { return xs OP_ find([](auto x) { return x == "apple"; }); });
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == "banana"; }); });
  RUN_CHECK(string, std::optional<string>, "", {}, {}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == "apple"; }); });

  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3)},
            [](auto &&xs) { return xs OP_ find([](auto x) { return x == Foo(3); }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {Foo(1)}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == Foo(1); }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == Foo(2); }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, {}, [](auto &&xs) { return xs OP_ find([](auto x) { return x == Foo(1); }); });

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P2, std::optional<P2>, name, {{3, 1}, {2, 2}, {3, 4}}, {{2, 2}}, f);
  #endif
    RUN_CHECK(P2, std::optional<P2>, name, {{3, 1}}, {}, f);
    RUN_CHECK(P2, std::optional<P2>, name, {}, {}, f);
  };
  p2("spread", [](auto &&xs) { return xs OP_ find([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ find([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, std::optional<P3>, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {{2, 2, 2}}, f);
  #endif
    RUN_CHECK(P3, std::optional<P3>, name, {{3, 1, 3}}, {}, f);
    RUN_CHECK(P3, std::optional<P3>, name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ find([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ find([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_CONTAINS
TEST_CASE(TPE_NAME "_contains", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, bool, "", {4, 2, 3, 1, 5}, true, [](auto &&xs) { return xs OP_ contains(4); });
  RUN_CHECK(int, bool, "", {1}, true, [](auto &&xs) { return xs OP_ contains(1); });
  RUN_CHECK(int, bool, "", {1}, false, [](auto &&xs) { return xs OP_ contains(0); });
  RUN_CHECK(int, bool, "", {}, false, [](auto &&xs) { return xs OP_ contains(1); });

  RUN_CHECK(string, bool, "", {"banana", "cherry", "apple"}, true, [](auto &&xs) { return xs OP_ contains("banana"); });
  RUN_CHECK(string, bool, "", {"apple"}, true, [](auto &&xs) { return xs OP_ contains("apple"); });
  RUN_CHECK(string, bool, "", {"apple"}, false, [](auto &&xs) { return xs OP_ contains("banana"); });
  RUN_CHECK(string, bool, "", {}, false, [](auto &&xs) { return xs OP_ contains("apple"); });

  RUN_CHECK(Foo, bool, "", {Foo(3), Foo(2), Foo(1)}, true, [](auto &&xs) { return xs OP_ contains(Foo(3)); });
  RUN_CHECK(Foo, bool, "", {Foo(1)}, true, [](auto &&xs) { return xs OP_ contains(Foo(1)); });
  RUN_CHECK(Foo, bool, "", {Foo(1)}, false, [](auto &&xs) { return xs OP_ contains(Foo(2)); });
  RUN_CHECK(Foo, bool, "", {}, false, [](auto &&xs) { return xs OP_ contains(Foo(1)); });
}
#endif

#ifndef DISABLE_REDUCE
TEST_CASE(TPE_NAME "_reduce", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ reduce([](auto acc, auto x) { return acc + x; }); };
  auto fooOp = [](auto &&xs) { return xs OP_ reduce([](auto acc, auto x) { return Foo(acc.value + x.value); }); };
  auto stringOp = [](auto &&xs) {
    return xs OP_ reduce([](auto acc, auto x) {
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
TEST_CASE(TPE_NAME "_tap_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &xs) {
    std::vector<std::decay_t<typename std::decay_t<decltype(xs)>::value_type>> out;
    auto f = [&](auto x) { out.push_back(x); };
  #ifdef TPE_IS_VIEW
    CHECK((xs OP_ tap_each(f) OP_ to_vector()) == (xs OP_ to_vector()));
  #else
    CHECK(((xs OP_ tap_each(f)) == xs));
  #endif
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
  p2("spread", [](auto &xs) {
    std::vector<int> out;
    auto f = [&](auto x0, auto x1) { out.push_back(x0 + x1); };
  #ifdef TPE_IS_VIEW
    CHECK((xs OP_ tap_each(f) OP_ to_vector()) == (xs OP_ to_vector()));
  #else
    CHECK(((xs OP_ tap_each(f)) == xs));
  #endif
    return out;
  });
  p2("single", [](auto &xs) {
    std::vector<int> out;
    auto f = [&](auto x) { out.push_back(get<0>(x) + get<1>(x)); };
  #ifdef TPE_IS_VIEW
    CHECK((xs OP_ tap_each(f) OP_ to_vector()) == (xs OP_ to_vector()));
  #else
    CHECK(((xs OP_ tap_each(f)) == xs));
  #endif
    return out;
  });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {7, 6, 5}, f);
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}}, {7}, f);
    RUN_CHECK(P3, std::vector<int>, name, {}, {}, f);
  };
  p3("spread", [](auto &xs) {
    std::vector<int> out;
    auto f = [&](auto x0, auto x1, auto x2) { out.push_back(x0 + x1 + x2); };
  #ifdef TPE_IS_VIEW
    CHECK((xs OP_ tap_each(f) OP_ to_vector()) == (xs OP_ to_vector()));
  #else
    CHECK(((xs OP_ tap_each(f)) == xs));
  #endif
    return out;
  });
  p3("single", [](auto &xs) {
    std::vector<int> out;
    auto f = [&](auto x) { out.push_back(get<0>(x) + get<1>(x) + get<2>(x)); };
  #ifdef TPE_IS_VIEW
    CHECK((xs OP_ tap_each(f) OP_ to_vector()) == (xs OP_ to_vector()));
  #else
    CHECK(((xs OP_ tap_each(f)) == xs));
  #endif
    return out;
  });
}
#endif

#ifndef DISABLE_FOR_EACH
TEST_CASE(TPE_NAME "_for_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) {
    std::vector<std::decay_t<typename std::decay_t<decltype(xs)>::value_type>> out;
    xs OP_ for_each([&](auto x) { out.push_back(x); });
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
  p2("spread", [](auto &&xs) {
    std::vector<int> out;
    xs OP_ for_each([&](auto x0, auto x1) { out.push_back(x0 + x1); });
    return out;
  });
  p2("single", [](auto &&xs) {
    std::vector<int> out;
    xs OP_ for_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x)); });
    return out;
  });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {7, 6, 5}, f);
    RUN_CHECK(P3, std::vector<int>, name, {{3, 1, 3}}, {7}, f);
    RUN_CHECK(P3, std::vector<int>, name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) {
    std::vector<int> out;
    xs OP_ for_each([&](auto x0, auto x1, auto x2) { out.push_back(x0 + x1 + x2); });
    return out;
  });
  p3("single", [](auto &&xs) {
    std::vector<int> out;
    xs OP_ for_each([&](auto x) { out.push_back(get<0>(x) + get<1>(x) + get<2>(x)); });
    return out;
  });
}
#endif

#ifndef DISABLE_PARTITION
TEST_CASE(TPE_NAME "_partition", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ partition([&](auto x) { return x % 2 == 0; }); };
  auto stringOp = [](auto &&xs) { return xs OP_ partition([&](auto x) { return x.size() > 1; }); };
  auto fooOp = [](auto &&xs) { return xs OP_ partition([&](auto x) { return x.value % 2 == 0; }); };
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
  p2("spread", [](auto &&xs) { return xs OP_ partition([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ partition([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
    using IntP3 = std::pair<TPE_CTOR_OUT(P3), TPE_CTOR_OUT(P3)>;

  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, IntP3, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {{{2, 2, 2}}, {{3, 1, 3}, {1, 3, 1}}}, f);
  #endif
    RUN_CHECK(P3, IntP3, name, {{3, 1, 3}}, {{}, {{3, 1, 3}}}, f);
    RUN_CHECK(P3, IntP3, name, {}, {{}, {}}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ partition([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ partition([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_GROUP_MAP_REDUCE
TEST_CASE(TPE_NAME "_group_map_reduce", "[" TPE_NAME "][" TPE_GROUP "]") {

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
      [](auto &&xs) {
        return xs OP_ group_map_reduce([](auto x) { return std::to_string(get<0>(x)); }, //
                                       [](auto x) { return get<1>(x) * 2; },             //
                                       [](auto x, auto y) { return x + y; });
      },
      [](auto &&xs) {
        return xs OP_ group_map_reduce([](auto x) { return get<0>(x).substr(0, 1); }, //
                                       [](auto x) { return get<1>(x) + "X"; },        //
                                       [](auto x, auto y) {
                                         auto out = x + y;
                                         std::sort(out.begin(), out.end());
                                         return out;
                                       });
      },
      [](auto &&xs) {
        return xs OP_ group_map_reduce([](auto x, auto) { return std::to_string(x.value); }, //
                                       [](auto, auto y) { return y.value * 3; },             //
                                       [](auto x, auto y) { return x + y; });
      });
  p2(
      [](auto &&xs) {
        return xs OP_ group_map_reduce([](auto x, auto) { return std::to_string(x); }, //
                                       [](auto, auto y) { return y * 2; },             //
                                       [](auto x, auto y) { return x + y; });
      },
      [](auto &&xs) {
        return xs OP_ group_map_reduce([](auto x) { return get<0>(x).substr(0, 1); }, //
                                       [](auto, auto y) { return y + "X"; },          //
                                       [](auto x, auto y) {
                                         auto out = x + y;
                                         std::sort(out.begin(), out.end());
                                         return out;
                                       });
      },
      [](auto &&xs) {
        return xs OP_ group_map_reduce([](auto x, auto) { return std::to_string(x.value); }, //
                                       [](auto, auto y) { return y.value * 3; },             //
                                       [](auto x, auto y) { return x + y; });
      });
}
#endif

#ifndef DISABLE_GROUP_MAP
TEST_CASE(TPE_NAME "_group_map", "[" TPE_NAME "][" TPE_GROUP "]") {

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
  p2([](auto &&xs) { return xs OP_ group_map([](auto x) { return std::to_string(get<0>(x)); }, [](auto x) { return get<1>(x) * 2; }); },
     [](auto &&xs) { return xs OP_ group_map([](auto x) { return get<0>(x).substr(0, 1); }, [](auto x) { return get<1>(x) + "X"; }); },
     [](auto &&xs) {
       return xs OP_ group_map([](auto x, auto) { return std::to_string(x.value); }, [](auto, auto y) { return y.value * 3; });
     });
  p2([](auto &&xs) { return xs OP_ group_map([](auto x, auto) { return std::to_string(x); }, [](auto, auto y) { return y * 2; }); },
     [](auto &&xs) { return xs OP_ group_map([](auto x) { return get<0>(x).substr(0, 1); }, [](auto, auto y) { return y + "X"; }); },
     [](auto &&xs) {
       return xs OP_ group_map([](auto x, auto) { return std::to_string(x.value); }, [](auto, auto y) { return y.value * 3; });
     });
}
#endif

#ifndef DISABLE_GROUP_BY
TEST_CASE(TPE_NAME "_group_by", "[" TPE_NAME "][" TPE_GROUP "]") {
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
  p2([](auto &&xs) { return xs OP_ group_by([](auto x) { return std::to_string(get<0>(x)); }); },
     [](auto &&xs) { return xs OP_ group_by([](auto x) { return get<0>(x).substr(0, 1); }); },
     [](auto &&xs) { return xs OP_ group_by([](auto x, auto) { return std::to_string(x.value); }); });
  p2([](auto &&xs) { return xs OP_ group_by([](auto x, auto) { return std::to_string(x); }); },
     [](auto &&xs) { return xs OP_ group_by([](auto x) { return get<0>(x).substr(0, 1); }); },
     [](auto &&xs) { return xs OP_ group_by([](auto x, auto) { return std::to_string(x.value); }); });
}
#endif

#ifndef DISABLE_TO_VECTOR
TEST_CASE(TPE_NAME "_to_vector", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ to_vector(); };

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

#ifndef DISABLE_TO_UNARY
TEST_CASE(TPE_NAME "_to_unary", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ to<std::vector>(); };

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

#ifndef DISABLE_TO_BINARY
TEST_CASE(TPE_NAME "_to_binary", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ to<std::map>(); };

  using IntP = std::pair<int, int>;
  using StringP = std::pair<string, string>;
  using StringFooP = std::pair<string, Foo>;

  using IntM = std::map<int, int>;
  using StringM = std::map<string, string>;
  using StringFooM = std::map<string, Foo>;

  RUN_CHECK(IntP, IntM, "", {{1, 4}, {2, 2}, {3, 3}}, {{1, 4}, {2, 2}, {3, 3}}, op);
  RUN_CHECK(StringP, StringM, "", {{"banana", "B"}, {"cherry", "C"}, {"apple", "A"}}, {{"apple", "A"}, {"banana", "B"}, {"cherry", "C"}},
            op);
  RUN_CHECK(StringFooP, StringFooM, "", {{"apple", Foo(1)}, {"banana", Foo(2)}}, {{"apple", Foo(1)}, {"banana", Foo(2)}}, op);

  RUN_CHECK(IntP, IntM, "", {{1, 1}}, {{1, 1}}, op);
  RUN_CHECK(IntP, IntM, "", {}, {}, op);
  RUN_CHECK(StringP, StringM, "", {{"apple", "A"}}, {{"apple", "A"}}, op);
  RUN_CHECK(StringP, StringM, "", {}, {}, op);
  RUN_CHECK(StringFooP, StringFooM, "", {{"apple", Foo(1)}}, {{"apple", Foo(1)}}, op);
  RUN_CHECK(StringFooP, StringFooM, "", {}, {}, op);
}
#endif
