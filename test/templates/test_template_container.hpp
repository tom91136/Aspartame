#include <functional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catch2/catch_test_macros.hpp"

#include "../test_base_container1.hpp"

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

  int i_val{4};
  string s_val{"a"};
  Foo f_val{42};
  int *i = &i_val;
  string *s = &s_val;
  Foo *f = &f_val;
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
}
#endif

#ifndef DISABLE_REINTERPRET_AS
TEST_CASE(TPE_NAME "_reinterpret_as", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ reinterpret_as<const char *>() OP_ static_as<string>(); };
  int i_val{'*' | ('\0' << 8)};
  int *i = &i_val;
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int *, TPE_CTOR_OUT(string), "", {i, i, i}, {"*", "*", "*"}, op);
  #endif
  RUN_CHECK(int *, TPE_CTOR_OUT(string), "", {i}, {"*"}, op);
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
  p2("single", [](auto &&xs) {
    return xs OP_ flat_map([](auto x) { return TPE_CTOR_OUT(std::decay_t<decltype(get<0>(x))>){get<0>(x) + get<1>(x)}; });
  });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {3 + 1 + 3, 2 + 2 + 2, 1 + 3 + 1}, f);
  #endif
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {{3, 1, 3}}, {3 + 1 + 3}, f);
    RUN_CHECK(P3, TPE_CTOR_OUT(int), name, {}, {}, f);
  };
  p3("spread",
     [](auto &&xs) { return xs OP_ flat_map([](auto x0, auto x1, auto x2) { return TPE_CTOR_OUT(decltype(x0)){x0 + x1 + x2}; }); });
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

#ifndef DISABLE_MIN
TEST_CASE(TPE_NAME "_min", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ min(); };
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, std::optional<int>{1}, op);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, std::optional<string>{"apple"}, op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, std::optional<Foo>{Foo(1)}, op);
  RUN_CHECK(int, std::optional<int>, "", {7}, std::optional<int>{7}, op);
  RUN_CHECK(int, std::optional<int>, "", {}, std::nullopt, op);
  RUN_CHECK(string, std::optional<string>, "", {}, std::nullopt, op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, std::nullopt, op);
}
#endif

#ifndef DISABLE_MAX
TEST_CASE(TPE_NAME "_max", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ max(); };
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, std::optional<int>{5}, op);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, std::optional<string>{"cherry"}, op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, std::optional<Foo>{Foo(3)}, op);
  RUN_CHECK(int, std::optional<int>, "", {7}, std::optional<int>{7}, op);
  RUN_CHECK(int, std::optional<int>, "", {}, std::nullopt, op);
  RUN_CHECK(string, std::optional<string>, "", {}, std::nullopt, op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, std::nullopt, op);
}
#endif

#ifndef DISABLE_MIN_BY
TEST_CASE(TPE_NAME "_min_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, std::optional<int>{5},
            [](auto &&xs) { return xs OP_ min_by([](auto x) { return -x; }); });
  RUN_CHECK(string, std::optional<string>, "", {"bb", "a", "cccc"}, std::optional<string>{"a"},
            [](auto &&xs) { return xs OP_ min_by([](auto x) { return x.size(); }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, std::optional<Foo>{Foo(1)},
            [](auto &&xs) { return xs OP_ min_by([](auto x) { return x.value; }); });
  RUN_CHECK(int, std::optional<int>, "", {}, std::nullopt, [](auto &&xs) { return xs OP_ min_by([](auto x) { return -x; }); });
}
#endif

#ifndef DISABLE_MAX_BY
TEST_CASE(TPE_NAME "_max_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, std::optional<int>{1},
            [](auto &&xs) { return xs OP_ max_by([](auto x) { return -x; }); });
  RUN_CHECK(string, std::optional<string>, "", {"bb", "a", "cccc"}, std::optional<string>{"cccc"},
            [](auto &&xs) { return xs OP_ max_by([](auto x) { return x.size(); }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, std::optional<Foo>{Foo(3)},
            [](auto &&xs) { return xs OP_ max_by([](auto x) { return x.value; }); });
  RUN_CHECK(int, std::optional<int>, "", {}, std::nullopt, [](auto &&xs) { return xs OP_ max_by([](auto x) { return -x; }); });
}
#endif

#if !defined(TPE_UNORDERED)
  #ifndef DISABLE_INDEX_OF_MIN
TEST_CASE(TPE_NAME "_index_of_min", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ index_of_min(); };
    #ifdef TPE_MANY_INIT
  RUN_CHECK(int, std::optional<size_t>, "", {4, 2, 3, 1, 5}, std::optional<size_t>{3}, op);
    #endif
  RUN_CHECK(int, std::optional<size_t>, "", {7}, std::optional<size_t>{0}, op);
  RUN_CHECK(int, std::optional<size_t>, "", {}, std::nullopt, op);
}
  #endif

  #ifndef DISABLE_INDEX_OF_MAX
TEST_CASE(TPE_NAME "_index_of_max", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ index_of_max(); };
    #ifdef TPE_MANY_INIT
  RUN_CHECK(int, std::optional<size_t>, "", {4, 2, 3, 1, 5}, std::optional<size_t>{4}, op);
    #endif
  RUN_CHECK(int, std::optional<size_t>, "", {7}, std::optional<size_t>{0}, op);
  RUN_CHECK(int, std::optional<size_t>, "", {}, std::nullopt, op);
}
  #endif

  #ifndef DISABLE_INDEX_OF_MIN_BY
TEST_CASE(TPE_NAME "_index_of_min_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ index_of_min_by([](auto x) { return -x; }); };
    #ifdef TPE_MANY_INIT
  RUN_CHECK(int, std::optional<size_t>, "", {4, 2, 3, 1, 5}, std::optional<size_t>{4}, op);
    #endif
  RUN_CHECK(int, std::optional<size_t>, "", {7}, std::optional<size_t>{0}, op);
  RUN_CHECK(int, std::optional<size_t>, "", {}, std::nullopt, op);
}
  #endif

  #ifndef DISABLE_INDEX_OF_MAX_BY
TEST_CASE(TPE_NAME "_index_of_max_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ index_of_max_by([](auto x) { return -x; }); };
    #ifdef TPE_MANY_INIT
  RUN_CHECK(int, std::optional<size_t>, "", {4, 2, 3, 1, 5}, std::optional<size_t>{3}, op);
    #endif
  RUN_CHECK(int, std::optional<size_t>, "", {7}, std::optional<size_t>{0}, op);
  RUN_CHECK(int, std::optional<size_t>, "", {}, std::nullopt, op);
}
  #endif
#endif

#ifndef DISABLE_SUM
TEST_CASE(TPE_NAME "_sum", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ sum(); };
  RUN_CHECK(int, int, "", {1, 2, 3, 4}, 10, op);
  RUN_CHECK(int, int, "", {7}, 7, op);
  RUN_CHECK(int, int, "", {}, 0, op);
}
#endif

#ifndef DISABLE_PRODUCT
TEST_CASE(TPE_NAME "_product", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto &&xs) { return xs OP_ product(); };
  RUN_CHECK(int, int, "", {1, 2, 3, 4}, 24, op);
  RUN_CHECK(int, int, "", {5}, 5, op);
  RUN_CHECK(int, int, "", {}, 1, op);
}
#endif

#ifndef DISABLE_SUM_BY
TEST_CASE(TPE_NAME "_sum_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {1, 2, 3}, 12, [](auto &&xs) { return xs OP_ sum_by([](auto x) { return x * 2; }); });
  RUN_CHECK(string, size_t, "", {"a", "bb", "ccc"}, size_t{6}, [](auto &&xs) { return xs OP_ sum_by([](auto x) { return x.size(); }); });
  RUN_CHECK(Foo, int, "", {Foo(1), Foo(2), Foo(3)}, 6, [](auto &&xs) { return xs OP_ sum_by([](auto x) { return x.value; }); });
  RUN_CHECK(int, int, "", {}, 0, [](auto &&xs) { return xs OP_ sum_by([](auto x) { return x * 2; }); });
}
#endif

#ifndef DISABLE_NONE_MATCH
TEST_CASE(TPE_NAME "_none_match", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, bool, "", {1, 2, 3}, true, [](auto &&xs) { return xs OP_ none_match([](auto x) { return x > 5; }); });
  RUN_CHECK(int, bool, "", {1, 2, 3}, false, [](auto &&xs) { return xs OP_ none_match([](auto x) { return x == 2; }); });
  RUN_CHECK(string, bool, "", {"a", "bb"}, true, [](auto &&xs) { return xs OP_ none_match([](auto x) { return x.size() > 5; }); });
  RUN_CHECK(Foo, bool, "", {Foo(1), Foo(2)}, false, [](auto &&xs) { return xs OP_ none_match([](auto x) { return x.value == 1; }); });
  RUN_CHECK(int, bool, "", {}, true, [](auto &&xs) { return xs OP_ none_match([](auto x) { return x == 2; }); });
}
#endif

#ifndef DISABLE_COMBINE_ALL
TEST_CASE(TPE_NAME "_combine_all", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {1, 2, 3, 4}, 10, [](auto &&xs) { return xs OP_ combine_all(); });
  RUN_CHECK(int, int, "", {}, 0, [](auto &&xs) { return xs OP_ combine_all(); });
  RUN_CHECK(string, string, "", {"abc", "def"}, "abcdef", [](auto &&xs) { return xs OP_ combine_all(); });
  RUN_CHECK(string, string, "", {}, "", [](auto &&xs) { return xs OP_ combine_all(); });
}
#endif

#ifndef DISABLE_INTERSPERSE
TEST_CASE(TPE_NAME "_intersperse", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intOp = [](auto &&xs) { return xs OP_ intersperse(0); };
  #ifdef TPE_MANY_INIT
  auto strOp = [](auto &&xs) { return xs OP_ intersperse(string{"_"}); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {1, 2, 3}, {1, 0, 2, 0, 3}, intOp);
  RUN_CHECK(string, TPE_CTOR_OUT(string), "", {"a", "b", "c"}, {"a", "_", "b", "_", "c"}, strOp);
  #endif
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {7}, {7}, intOp);
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, {}, intOp);
}
#endif

#ifndef DISABLE_TRAVERSE
TEST_CASE(TPE_NAME "_traverse_optional", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto ok = [](auto &&xs) { return xs OP_ traverse([](int x) -> std::optional<int> { return x * 2; }); };
  auto okStr = [](auto &&xs) { return xs OP_ traverse([](int x) -> std::optional<string> { return std::to_string(x); }); };
  auto okFoo = [](auto &&xs) { return xs OP_ traverse([](int x) -> std::optional<Foo> { return Foo{x}; }); };
  using ResOk = std::optional<TPE_CTOR_OUT(int)>;
  using ResStr = std::optional<TPE_CTOR_OUT(string)>;
  using ResFoo = std::optional<TPE_CTOR_OUT(Foo)>;
  #ifdef TPE_MANY_INIT
  auto none_on3 = [](auto &&xs) {
    return xs OP_ traverse([](int x) -> std::optional<int> { return x == 3 ? std::nullopt : std::optional<int>{x}; });
  };
  auto none_on_first = [](auto &&xs) {
    return xs OP_ traverse([](int x) -> std::optional<int> { return x == 1 ? std::nullopt : std::optional<int>{x}; });
  };
  auto none_on_last = [](auto &&xs) {
    return xs OP_ traverse([](int x) -> std::optional<int> { return x == 4 ? std::nullopt : std::optional<int>{x}; });
  };
  RUN_CHECK(int, ResOk, "all-some", {1, 2, 4}, ResOk{TPE_CTOR_OUT(int){2, 4, 8}}, ok);
  RUN_CHECK(int, ResOk, "none-on-3", {1, 2, 3, 4}, ResOk{}, none_on3);
  RUN_CHECK(int, ResOk, "none-on-first", {1, 2, 3}, ResOk{}, none_on_first);
  RUN_CHECK(int, ResOk, "none-on-last", {1, 2, 3, 4}, ResOk{}, none_on_last);
  RUN_CHECK(int, ResStr, "to-string", {1, 2, 3}, ResStr{TPE_CTOR_OUT(string){"1", "2", "3"}}, okStr);
  RUN_CHECK(int, ResFoo, "to-foo", {1, 2, 3}, ResFoo{TPE_CTOR_OUT(Foo){Foo{1}, Foo{2}, Foo{3}}}, okFoo);
  #endif
  RUN_CHECK(int, ResOk, "empty", {}, ResOk{TPE_CTOR_OUT(int){}}, ok);
  RUN_CHECK(int, ResStr, "empty-str", {}, ResStr{TPE_CTOR_OUT(string){}}, okStr);
}

  #if defined(__cpp_lib_expected) && defined(TPE_INPLACE_SEQ)
// std::expected has neither operator< nor std::hash, so the result-container
// types in this case (e.g. set<expected<...>>) only compile for sequence-shaped
// drivers (vector/deque/list). Set/map drivers are gated out.
TEST_CASE(TPE_NAME "_traverse_expected", "[" TPE_NAME "][" TPE_GROUP "]") {
  using ExpInt = std::expected<int, string>;
  using ResExpInt = std::expected<TPE_CTOR_OUT(int), string>;
  using ExpStr = std::expected<string, int>;
  using ResExpStr = std::expected<TPE_CTOR_OUT(string), int>;
  auto ok = [](auto &&xs) { return xs OP_ traverse([](int x) -> ExpInt { return x * 2; }); };
  auto okStr = [](auto &&xs) { return xs OP_ traverse([](int x) -> ExpStr { return std::to_string(x); }); };
    #ifdef TPE_MANY_INIT
  auto err_on3 = [](auto &&xs) {
    return xs OP_ traverse([](int x) -> ExpInt { return x == 3 ? ExpInt{std::unexpected("err-3")} : ExpInt{x}; });
  };
  auto err_on_first = [](auto &&xs) {
    return xs OP_ traverse([](int x) -> ExpInt { return x == 1 ? ExpInt{std::unexpected("err-1")} : ExpInt{x}; });
  };
  RUN_CHECK(int, ResExpInt, "all-ok", {1, 2, 4}, ResExpInt{TPE_CTOR_OUT(int){2, 4, 8}}, ok);
  RUN_CHECK(int, ResExpInt, "err-on-3", {1, 2, 3, 4}, ResExpInt{std::unexpected("err-3")}, err_on3);
  RUN_CHECK(int, ResExpInt, "err-on-first", {1, 2, 3}, ResExpInt{std::unexpected("err-1")}, err_on_first);
  RUN_CHECK(int, ResExpStr, "to-string", {1, 2, 3}, ResExpStr{TPE_CTOR_OUT(string){"1", "2", "3"}}, okStr);
    #endif
  RUN_CHECK(int, ResExpInt, "empty", {}, ResExpInt{TPE_CTOR_OUT(int){}}, ok);
}
  #endif
#endif

#ifndef DISABLE_SEQUENCE
TEST_CASE(TPE_NAME "_sequence", "[" TPE_NAME "][" TPE_GROUP "]") {
  using OptInt = std::optional<int>;
  using OptStr = std::optional<string>;
  using OptFoo = std::optional<Foo>;
  using ResInt = std::optional<TPE_CTOR_OUT(int)>;
  using ResStr = std::optional<TPE_CTOR_OUT(string)>;
  using ResFoo = std::optional<TPE_CTOR_OUT(Foo)>;
  auto op = [](auto &&xs) { return xs OP_ sequence(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(OptInt, ResInt, "int-all-some", {OptInt{1}, OptInt{2}, OptInt{3}}, ResInt{TPE_CTOR_OUT(int){1, 2, 3}}, op);
  RUN_CHECK(OptInt, ResInt, "int-none-mid", {OptInt{1}, std::nullopt, OptInt{3}}, ResInt{}, op);
  RUN_CHECK(OptInt, ResInt, "int-none-first", {std::nullopt, OptInt{2}, OptInt{3}}, ResInt{}, op);
  RUN_CHECK(OptInt, ResInt, "int-none-last", {OptInt{1}, OptInt{2}, std::nullopt}, ResInt{}, op);
  RUN_CHECK(OptInt, ResInt, "int-all-none", {std::nullopt, std::nullopt, std::nullopt}, ResInt{}, op);
  RUN_CHECK(OptStr, ResStr, "str-all-some", {OptStr{"a"}, OptStr{"b"}, OptStr{"c"}}, ResStr{TPE_CTOR_OUT(string){"a", "b", "c"}}, op);
  RUN_CHECK(OptStr, ResStr, "str-none-mid", {OptStr{"a"}, std::nullopt, OptStr{"c"}}, ResStr{}, op);
  RUN_CHECK(OptFoo, ResFoo, "foo-all-some", {OptFoo{Foo{1}}, OptFoo{Foo{2}}, OptFoo{Foo{3}}},
            ResFoo{TPE_CTOR_OUT(Foo){Foo{1}, Foo{2}, Foo{3}}}, op);
  RUN_CHECK(OptFoo, ResFoo, "foo-none-mid", {OptFoo{Foo{1}}, std::nullopt, OptFoo{Foo{3}}}, ResFoo{}, op);
  #endif
  RUN_CHECK(OptInt, ResInt, "empty-int", {}, ResInt{TPE_CTOR_OUT(int){}}, op);
  RUN_CHECK(OptStr, ResStr, "empty-str", {}, ResStr{TPE_CTOR_OUT(string){}}, op);
}

#ifndef DISABLE_SEQUENCE_NESTED
TEST_CASE(TPE_NAME "_sequence_nested_optional", "[" TPE_NAME "][" TPE_GROUP "]") {
  using OptInt = std::optional<int>;
  using OptOptInt = std::optional<OptInt>;
  using ResOptInt = std::optional<TPE_CTOR_OUT(OptInt)>;
  auto op = [](auto &&xs) { return xs OP_ sequence(); };
  #ifdef TPE_MANY_INIT
  // The OUTER optional drives the monad short-circuit; the INNER optional is an
  // opaque element preserved as-is (including when empty).
  RUN_CHECK(OptOptInt, ResOptInt, "nested-all-outer-engaged-inner-some", {OptOptInt{OptInt{1}}, OptOptInt{OptInt{2}}, OptOptInt{OptInt{3}}},
            ResOptInt{TPE_CTOR_OUT(OptInt){OptInt{1}, OptInt{2}, OptInt{3}}}, op);
  RUN_CHECK(OptOptInt, ResOptInt, "nested-outer-engaged-with-empty-inner",
            {OptOptInt{OptInt{1}}, OptOptInt{OptInt{}}, OptOptInt{OptInt{3}}},
            ResOptInt{TPE_CTOR_OUT(OptInt){OptInt{1}, OptInt{}, OptInt{3}}}, op);
  RUN_CHECK(OptOptInt, ResOptInt, "nested-outer-nullopt-short-circuits", {OptOptInt{OptInt{1}}, std::nullopt, OptOptInt{OptInt{3}}},
            ResOptInt{}, op);
  #endif
  RUN_CHECK(OptOptInt, ResOptInt, "nested-empty", {}, ResOptInt{TPE_CTOR_OUT(OptInt){}}, op);
}
#endif

  #if defined(__cpp_lib_expected) && defined(TPE_INPLACE_SEQ)
TEST_CASE(TPE_NAME "_sequence_expected", "[" TPE_NAME "][" TPE_GROUP "]") {
  using ExpInt = std::expected<int, string>;
  using ResInt = std::expected<TPE_CTOR_OUT(int), string>;
  using ExpStr = std::expected<string, int>;
  using ResStr = std::expected<TPE_CTOR_OUT(string), int>;
  auto op = [](auto &&xs) { return xs OP_ sequence(); };
    #ifdef TPE_MANY_INIT
  RUN_CHECK(ExpInt, ResInt, "int-all-ok", {ExpInt{1}, ExpInt{2}, ExpInt{3}}, ResInt{TPE_CTOR_OUT(int){1, 2, 3}}, op);
  RUN_CHECK(ExpInt, ResInt, "int-err-mid", {ExpInt{1}, ExpInt{std::unexpected("err")}, ExpInt{3}}, ResInt{std::unexpected("err")}, op);
  RUN_CHECK(ExpInt, ResInt, "int-err-first", {ExpInt{std::unexpected("first")}, ExpInt{2}, ExpInt{3}}, ResInt{std::unexpected("first")},
            op);
  RUN_CHECK(ExpStr, ResStr, "str-all-ok", {ExpStr{"a"}, ExpStr{"b"}, ExpStr{"c"}}, ResStr{TPE_CTOR_OUT(string){"a", "b", "c"}}, op);
  RUN_CHECK(ExpStr, ResStr, "str-err-mid", {ExpStr{"a"}, ExpStr{std::unexpected(42)}, ExpStr{"c"}}, ResStr{std::unexpected(42)}, op);
    #endif
  RUN_CHECK(ExpInt, ResInt, "empty", {}, ResInt{TPE_CTOR_OUT(int){}}, op);
}
  #endif
#endif
