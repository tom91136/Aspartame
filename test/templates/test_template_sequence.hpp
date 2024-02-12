#include <functional>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#include "../test_base_container1.hpp"
#include "catch2/catch_test_macros.hpp"

#ifndef DISABLE_PREPEND
TEST_CASE(TPE_NAME "_prepend", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto prependOp = [](auto &&x) { return [&](auto &&xs) { return xs OP_ prepend(std::forward<decltype(x)>(x)); }; };
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {1, 2, 3}, {42, 1, 2, 3}, prependOp(42));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {"apple"}, {"pear", "apple"}, prependOp("pear"));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {Foo(1), Foo(2)}, {Foo(7), Foo(1), Foo(2)}, prependOp(Foo(7)));
  #endif

  RUN_CHECK(int, TPE_CTOR_VAR_OUT(int), "", {}, {42}, prependOp(42));
  RUN_CHECK(string, TPE_CTOR_VAR_OUT(string), "", {}, {"pear"}, prependOp("pear"));
  RUN_CHECK(Foo, TPE_CTOR_VAR_OUT(Foo), "", {}, {Foo(7)}, prependOp(Foo(7)));
}
#endif

#ifndef DISABLE_HEAD_MAYBE
TEST_CASE(TPE_NAME "_head_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto reverseOp = [](auto &&xs) { return xs OP_ head_maybe(); };
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, {4}, reverseOp);
  RUN_CHECK(int, std::optional<int>, "", {1}, {1}, reverseOp);
  RUN_CHECK(int, std::optional<int>, "", {}, {}, reverseOp);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, {"banana"}, reverseOp);
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {"apple"}, reverseOp);
  RUN_CHECK(string, std::optional<string>, "", {}, {}, reverseOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3)}, reverseOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {Foo(1)}, reverseOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, {}, reverseOp);
}
#endif

#ifndef DISABLE_LAST_MAYBE
TEST_CASE(TPE_NAME "_last_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto reverseOp = [](auto &&xs) { return xs OP_ last_maybe(); };
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, {5}, reverseOp);
  RUN_CHECK(int, std::optional<int>, "", {1}, {1}, reverseOp);
  RUN_CHECK(int, std::optional<int>, "", {}, {}, reverseOp);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, {"apple"}, reverseOp);
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {"apple"}, reverseOp);
  RUN_CHECK(string, std::optional<string>, "", {}, {}, reverseOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(1)}, reverseOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {Foo(1)}, reverseOp);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, {}, reverseOp);
}
#endif

#ifndef DISABLE_INIT
TEST_CASE(TPE_NAME "_init", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto initOp = [](auto &&xs) { return xs OP_ init(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1}, initOp);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"banana", "cherry"}, initOp);

  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2)}, initOp);
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, initOp);
  RUN_CHECK_ID(int, "", {}, {}, initOp);
  RUN_CHECK_ID(string, "", {"apple"}, {}, initOp);
  RUN_CHECK_ID(string, "", {}, {}, initOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, initOp);
  RUN_CHECK_ID(Foo, "", {}, {}, initOp);
}
#endif

#ifndef DISABLE_TAIL
TEST_CASE(TPE_NAME "_tail", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto tailOp = [](auto &&xs) { return xs OP_ tail(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {2, 3, 1, 5}, tailOp);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"cherry", "apple"}, tailOp);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(2), Foo(1)}, tailOp);
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, tailOp);
  RUN_CHECK_ID(int, "", {}, {}, tailOp);
  RUN_CHECK_ID(string, "", {"apple"}, {}, tailOp);
  RUN_CHECK_ID(string, "", {}, {}, tailOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, tailOp);
  RUN_CHECK_ID(Foo, "", {}, {}, tailOp);
}
#endif

#ifndef DISABLE_AT_MAYBE
TEST_CASE(TPE_NAME "_at_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto at0Op = [](auto &&xs) { return xs OP_ at_maybe(0); };
  auto at1Op = [](auto &&xs) { return xs OP_ at_maybe(1); };
  auto at100Op = [](auto &&xs) { return xs OP_ at_maybe(100); };

  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, {4}, at0Op);
  RUN_CHECK(int, std::optional<int>, "", {1}, {1}, at0Op);
  RUN_CHECK(int, std::optional<int>, "", {}, {}, at0Op);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, {"banana"}, at0Op);
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {"apple"}, at0Op);
  RUN_CHECK(string, std::optional<string>, "", {}, {}, at0Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3)}, at0Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {Foo(1)}, at0Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, {}, at0Op);

  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, {2}, at1Op);
  RUN_CHECK(int, std::optional<int>, "", {1}, {}, at1Op);
  RUN_CHECK(int, std::optional<int>, "", {}, {}, at1Op);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, {"cherry"}, at1Op);
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {}, at1Op);
  RUN_CHECK(string, std::optional<string>, "", {}, {}, at1Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {Foo(2)}, at1Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {}, at1Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, {}, at1Op);

  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 1, 5}, {}, at100Op);
  RUN_CHECK(int, std::optional<int>, "", {1}, {}, at100Op);
  RUN_CHECK(int, std::optional<int>, "", {}, {}, at100Op);
  RUN_CHECK(string, std::optional<string>, "", {"banana", "cherry", "apple"}, {}, at100Op);
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {}, at100Op);
  RUN_CHECK(string, std::optional<string>, "", {}, {}, at100Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(3), Foo(2), Foo(1)}, {}, at100Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {}, at100Op);
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, {}, at100Op);
}
#endif

#ifndef DISABLE_SLICE
TEST_CASE(TPE_NAME "_slice", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](size_t from, size_t to) { return [=](auto &&xs) { return xs OP_ slice(from, to); }; };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {2, 3, 1}, op(1, 4));
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"cherry", "apple"}, op(1, 3));
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(2), Foo(1)}, op(1, 3));

  RUN_CHECK_ID(int, "", {1, 2, 3, 4}, {1, 2, 3, 4}, op(0, 4));
  RUN_CHECK_ID(int, "", {1, 2, 3, 4}, {}, op(4, 4));
  RUN_CHECK_ID(int, "", {1, 2, 3, 4}, {3, 4}, op(2, 5));
  RUN_CHECK_ID(string, "", {"apple", "banana"}, {"apple"}, op(0, 1));
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, op(1, 1));
  RUN_CHECK_ID(int, "", {}, {}, op(0, 0));
  RUN_CHECK_ID(string, "", {"apple"}, {}, op(1, 1));
  RUN_CHECK_ID(string, "", {}, {}, op(0, 0));
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, op(1, 1));
  RUN_CHECK_ID(Foo, "", {}, {}, op(0, 0));
}
#endif

#ifndef DISABLE_INDEX_OF_SLICE
TEST_CASE(TPE_NAME "_index_of_slice", "[" TPE_NAME "][" TPE_GROUP "]") {
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {4, 2, 3, 1, 5}, 0, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(int){}); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {4, 2, 3, 1, 5}, 0, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(int){4, 2});
  });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {4, 2, 3, 1, 5}, 2, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(int){3, 1});
  });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {4, 2, 3, 1, 5}, -1, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(int){1, 2});
  });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {4, 2, 3, 1, 5}, -1, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(int){6}); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana", "cherry", "apple"}, 0,
            [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(string){}); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana", "cherry", "apple"}, 0, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(string){"banana", "cherry"});
  });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana", "cherry", "apple"}, 1, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(string){"cherry", "apple"});
  });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana", "cherry", "apple"}, -1, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(string){"apple", "banana"});
  });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana", "cherry", "apple"}, -1,
            [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(string){"pear"}); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3), Foo(2), Foo(1)}, 0,
            [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){}); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3), Foo(2), Foo(1)}, 0, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){Foo(3), Foo(2)});
  });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3), Foo(2), Foo(1)}, 1, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){Foo(2), Foo(1)});
  });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3), Foo(2), Foo(1)}, -1, [](auto &&xs) {
    return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){Foo(1), Foo(3)});
  });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3), Foo(2), Foo(1)}, -1,
            [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
  #endif

  RUN_CHECK(int, std::make_signed_t<size_t>, "", {6}, 0, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(int){6}); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {5}, -1, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(int){6}); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {}, 0, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(int){}); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(int){6}); });

  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana"}, -1,
            [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(string){"pear"}); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana"}, 0,
            [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(string){"banana"}); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {}, 0, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(string){}); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(string){"pear"}); });

  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3)}, -1, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(4)}, 0, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {}, 0, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){}); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_of_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
}
#endif

#ifndef DISABLE_CONTAINS_SLICE
TEST_CASE(TPE_NAME "_contains_slice", "[" TPE_NAME "][" TPE_GROUP "]") {
  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, bool, "", {4, 2, 3, 1, 5}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){}); });
  RUN_CHECK(int, bool, "", {4, 2, 3, 1, 5}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){4, 2}); });
  RUN_CHECK(int, bool, "", {4, 2, 3, 1, 5}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){3, 1}); });
  RUN_CHECK(int, bool, "", {4, 2, 3, 1, 5}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){1, 2}); });
  RUN_CHECK(int, bool, "", {4, 2, 3, 1, 5}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){6}); });
  RUN_CHECK(string, bool, "", {"banana", "cherry", "apple"}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(string){}); });
  RUN_CHECK(string, bool, "", {"banana", "cherry", "apple"}, true, [](auto &&xs) {
    return xs OP_ contains_slice(TPE_CTOR_IN(string){"banana", "cherry"});
  });
  RUN_CHECK(string, bool, "", {"banana", "cherry", "apple"}, true, [](auto &&xs) {
    return xs OP_ contains_slice(TPE_CTOR_IN(string){"cherry", "apple"});
  });
  RUN_CHECK(string, bool, "", {"banana", "cherry", "apple"}, false, [](auto &&xs) {
    return xs OP_ contains_slice(TPE_CTOR_IN(string){"apple", "banana"});
  });
  RUN_CHECK(string, bool, "", {"banana", "cherry", "apple"}, false,
            [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(string){"pear"}); });
  RUN_CHECK(Foo, bool, "", {Foo(3), Foo(2), Foo(1)}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(Foo){}); });
  RUN_CHECK(Foo, bool, "", {Foo(3), Foo(2), Foo(1)}, true, [](auto &&xs) {
    return xs OP_ contains_slice(TPE_CTOR_IN(Foo){Foo(3), Foo(2)});
  });
  RUN_CHECK(Foo, bool, "", {Foo(3), Foo(2), Foo(1)}, true, [](auto &&xs) {
    return xs OP_ contains_slice(TPE_CTOR_IN(Foo){Foo(2), Foo(1)});
  });
  RUN_CHECK(Foo, bool, "", {Foo(3), Foo(2), Foo(1)}, false, [](auto &&xs) {
    return xs OP_ contains_slice(TPE_CTOR_IN(Foo){Foo(1), Foo(3)});
  });
  RUN_CHECK(Foo, bool, "", {Foo(3), Foo(2), Foo(1)}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
  #endif

  RUN_CHECK(int, bool, "", {6}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){6}); });
  RUN_CHECK(int, bool, "", {5}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){6}); });
  RUN_CHECK(int, bool, "", {}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){}); });
  RUN_CHECK(int, bool, "", {}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(int){6}); });

  RUN_CHECK(string, bool, "", {"banana"}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(string){"pear"}); });
  RUN_CHECK(string, bool, "", {"banana"}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(string){"banana"}); });
  RUN_CHECK(string, bool, "", {}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(string){}); });
  RUN_CHECK(string, bool, "", {}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(string){"pear"}); });

  RUN_CHECK(Foo, bool, "", {Foo(3)}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
  RUN_CHECK(Foo, bool, "", {Foo(4)}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
  RUN_CHECK(Foo, bool, "", {}, true, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(Foo){}); });
  RUN_CHECK(Foo, bool, "", {}, false, [](auto &&xs) { return xs OP_ contains_slice(TPE_CTOR_IN(Foo){Foo(4)}); });
}
#endif

#ifndef DISABLE_INDEX_OF
TEST_CASE(TPE_NAME "_index_of", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {4, 2, 3, 1, 5}, 0, [](auto &&xs) { return xs OP_ index_of(4); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {1}, 0, [](auto &&xs) { return xs OP_ index_of(1); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {1}, -1, [](auto &&xs) { return xs OP_ index_of(0); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_of(1); });

  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana", "cherry", "apple"}, 0, [](auto &&xs) { return xs OP_ index_of("banana"); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"apple"}, 0, [](auto &&xs) { return xs OP_ index_of("apple"); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"apple"}, -1, [](auto &&xs) { return xs OP_ index_of("banana"); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_of("apple"); });

  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3), Foo(2), Foo(1)}, 0, [](auto &&xs) { return xs OP_ index_of(Foo(3)); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(1)}, 0, [](auto &&xs) { return xs OP_ index_of(Foo(1)); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(1)}, -1, [](auto &&xs) { return xs OP_ index_of(Foo(2)); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_of(Foo(1)); });
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

#ifndef DISABLE_FIND_LAST
TEST_CASE(TPE_NAME "_find_last", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::optional<int>, "", {4, 2, 3, 8, 1}, {8},
            [](auto &&xs) { return xs OP_ find_last([](auto x) { return x % 2 == 0; }); });
  RUN_CHECK(int, std::optional<int>, "", {1}, {1}, [](auto &&xs) { return xs OP_ find_last([](auto x) { return x % 2 != 0; }); });
  RUN_CHECK(int, std::optional<int>, "", {1}, {}, [](auto &&xs) { return xs OP_ find_last([](auto x) { return x % 2 == 0; }); });
  RUN_CHECK(int, std::optional<int>, "", {}, {}, [](auto &&xs) { return xs OP_ find_last([](auto x) { return x % 2 == 0; }); });

  RUN_CHECK(string, std::optional<string>, "", {"banana", "banana", "cherry", "apple"}, {"banana"},
            [](auto &&xs) { return xs OP_ find_last([](auto x) { return x == "banana"; }); });
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {"apple"},
            [](auto &&xs) { return xs OP_ find_last([](auto x) { return x == "apple"; }); });
  RUN_CHECK(string, std::optional<string>, "", {"apple"}, {},
            [](auto &&xs) { return xs OP_ find_last([](auto x) { return x == "banana"; }); });
  RUN_CHECK(string, std::optional<string>, "", {}, {}, [](auto &&xs) { return xs OP_ find_last([](auto x) { return x == "apple"; }); });

  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(6), Foo(2), Foo(1), Foo(1)}, {Foo(2)},
            [](auto &&xs) { return xs OP_ find_last([](auto x) { return x.value % 2 == 0; }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {Foo(1)},
            [](auto &&xs) { return xs OP_ find_last([](auto x) { return x.value % 2 != 0; }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {Foo(1)}, {}, [](auto &&xs) { return xs OP_ find_last([](auto x) { return x.value % 2 == 0; }); });
  RUN_CHECK(Foo, std::optional<Foo>, "", {}, {}, [](auto &&xs) { return xs OP_ find_last([](auto x) { return x.value % 2 == 0; }); });

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P2, std::optional<P2>, name, {{3, 1}, {2, 2}, {1, 3}}, {{2, 2}}, f);
  #endif
    RUN_CHECK(P2, std::optional<P2>, name, {{3, 1}}, {{3, 1}}, f);
    RUN_CHECK(P2, std::optional<P2>, name, {}, {}, f);
  };
  p2("spread", [](auto &&xs) { return xs OP_ find_last([](auto x0, auto x1) { return x0 >= x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ find_last([](auto x) { return get<0>(x) >= get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, std::optional<P3>, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, {{2, 2, 2}}, f);
  #endif
    RUN_CHECK(P3, std::optional<P3>, name, {{3, 1, 3}}, {}, f);
    RUN_CHECK(P3, std::optional<P3>, name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ find_last([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ find_last([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_INDEX_WHERE
TEST_CASE(TPE_NAME "_index_where", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {4, 2, 3, 1, 5}, 0,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == 4; }); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {1}, 0, [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == 1; }); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {1}, -1, [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == 0; }); });
  RUN_CHECK(int, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == 1; }); });

  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"banana", "cherry", "apple"}, 0,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == "banana"; }); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"apple"}, 0,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == "apple"; }); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {"apple"}, -1,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == "banana"; }); });
  RUN_CHECK(string, std::make_signed_t<size_t>, "", {}, -1,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == "apple"; }); });

  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(3), Foo(2), Foo(1)}, 0,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == Foo(3); }); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(1)}, 0,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == Foo(1); }); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {Foo(1)}, -1,
            [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == Foo(2); }); });
  RUN_CHECK(Foo, std::make_signed_t<size_t>, "", {}, -1, [](auto &&xs) { return xs OP_ index_where([](auto x) { return x == Foo(1); }); });

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P2, std::make_signed_t<size_t>, name, {{3, 1}, {2, 2}, {1, 3}}, 1, f);
  #endif
    RUN_CHECK(P2, std::make_signed_t<size_t>, name, {{3, 1}}, -1, f);
    RUN_CHECK(P2, std::make_signed_t<size_t>, name, {}, -1, f);
  };
  p2("spread", [](auto &&xs) { return xs OP_ index_where([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ index_where([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK(P3, std::make_signed_t<size_t>, name, {{3, 1, 3}, {2, 2, 2}, {1, 3, 1}}, 1, f);
  #endif
    RUN_CHECK(P3, std::make_signed_t<size_t>, name, {{3, 1, 3}}, -1, f);
    RUN_CHECK(P3, std::make_signed_t<size_t>, name, {}, -1, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ index_where([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ index_where([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_ZIP_WITH_INDEX
TEST_CASE(TPE_NAME "_zip_with_index", "[" TPE_NAME "][" TPE_GROUP "]") {
  using IntP = TPE_CTOR_OUT(std::pair<int COMMA size_t>);
  using StringP = TPE_CTOR_OUT(std::pair<string COMMA size_t>);
  using FooP = TPE_CTOR_OUT(std::pair<Foo COMMA size_t>);

  auto zipIdxOp = [](auto &&xs) { return xs OP_ zip_with_index<size_t>(); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, IntP, "", {4, 2, 3, 1, 5}, {{4, 0}, {2, 1}, {3, 2}, {1, 3}, {5, 4}}, zipIdxOp);
  RUN_CHECK(string, StringP, "", {"banana", "cherry", "apple"}, {{"banana", 0}, {"cherry", 1}, {"apple", 2}}, zipIdxOp);
  RUN_CHECK(Foo, FooP, "", {Foo(3), Foo(2), Foo(1)}, {{Foo(3), 0}, {Foo(2), 1}, {Foo(1), 2}}, zipIdxOp);
  #endif

  RUN_CHECK(int, IntP, "", {1}, {{1, 0}}, zipIdxOp);
  RUN_CHECK(int, IntP, "", {}, {}, zipIdxOp);

  RUN_CHECK(string, StringP, "", {"apple"}, {{"apple", 0}}, zipIdxOp);
  RUN_CHECK(string, StringP, "", {}, {}, zipIdxOp);

  RUN_CHECK(Foo, FooP, "", {Foo(1)}, {{Foo(1), 0}}, zipIdxOp);
  RUN_CHECK(Foo, FooP, "", {}, {}, zipIdxOp);
}
#endif

#ifndef DISABLE_ZIP
TEST_CASE(TPE_NAME "_zip", "[" TPE_NAME "][" TPE_GROUP "]") {
  using IntP = TPE_CTOR_OUT(std::pair<int COMMA int>);
  using StringP = TPE_CTOR_OUT(std::pair<string COMMA string>);
  using FooP = TPE_CTOR_OUT(std::pair<Foo COMMA Foo>);

  auto zipOp = [](auto &&ys) { return [&](auto &&xs) { return xs OP_ zip(ys); }; };

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, IntP, "", {4, 2, 3}, {{4, 1}, {2, 2}, {3, 3}}, zipOp(TPE_CTOR_OUT(int){1, 2, 3}));
  RUN_CHECK(string, StringP, "", {"banana", "cherry", "apple"}, {{"banana", "a"}, {"cherry", "b"}, {"apple", "c"}},
            zipOp(TPE_CTOR_OUT(string){"a", "b", "c"}));
  RUN_CHECK(Foo, FooP, "", {Foo(3), Foo(2), Foo(1)}, {{Foo(3), Foo(4)}, {Foo(2), Foo(5)}, {Foo(1), Foo(6)}},
            zipOp(TPE_CTOR_OUT(Foo){Foo(4), Foo(5), Foo(6)}));
  #endif

  RUN_CHECK(int, IntP, "", {1}, {{1, 42}}, zipOp(TPE_CTOR_OUT(int){42}));
  RUN_CHECK(int, IntP, "", {}, {}, zipOp(TPE_CTOR_OUT(int){}));
  RUN_CHECK(string, StringP, "", {"apple"}, {{"apple", "b"}}, zipOp(TPE_CTOR_OUT(string){"b"}));
  RUN_CHECK(string, StringP, "", {}, {}, zipOp(TPE_CTOR_OUT(string){}));
  RUN_CHECK(Foo, FooP, "", {Foo(1)}, {{Foo(1), Foo(7)}}, zipOp(TPE_CTOR_OUT(Foo){Foo(7)}));
  RUN_CHECK(Foo, FooP, "", {}, {}, zipOp(TPE_CTOR_OUT(Foo){}));
}
#endif

#ifndef DISABLE_TRANSPOSE
TEST_CASE(TPE_NAME "_transpose", "[" TPE_NAME "][" TPE_GROUP "]") {

  auto transposeOp = [](auto &&xs) { return xs OP_ transpose(); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK(TPE_CTOR_IN(int), TPE_CTOR_IN(TPE_CTOR_IN(int)), "", //
            {{4, 2, 3}, {1, 5, 6}}, {{4, 1}, {2, 5}, {3, 6}}, transposeOp);
  RUN_CHECK(TPE_CTOR_IN(string), TPE_CTOR_IN(TPE_CTOR_IN(string)), "", //
            {{"banana", "cherry"}, {"apple", "mango"}}, {{"banana", "apple"}, {"cherry", "mango"}}, transposeOp);
  RUN_CHECK(TPE_CTOR_IN(Foo), TPE_CTOR_IN(TPE_CTOR_IN(Foo)), "", //
            {{Foo(3), Foo(2)}, {Foo(1), Foo(4)}}, {{Foo(3), Foo(1)}, {Foo(2), Foo(4)}}, transposeOp);
  #endif

  RUN_CHECK(TPE_CTOR_IN(int), TPE_CTOR_IN(TPE_CTOR_IN(int)), "", {{1}}, {{1}}, transposeOp);
  RUN_CHECK(TPE_CTOR_IN(int), TPE_CTOR_IN(TPE_CTOR_IN(int)), "", {}, {}, transposeOp);
  RUN_CHECK(TPE_CTOR_IN(string), TPE_CTOR_IN(TPE_CTOR_IN(string)), "", {{"apple"}}, {{"apple"}}, transposeOp);
  RUN_CHECK(TPE_CTOR_IN(string), TPE_CTOR_IN(TPE_CTOR_IN(string)), "", {}, {}, transposeOp);
  RUN_CHECK(TPE_CTOR_IN(Foo), TPE_CTOR_IN(TPE_CTOR_IN(Foo)), "", {{Foo(1)}}, {{Foo(1)}}, transposeOp);
  RUN_CHECK(TPE_CTOR_IN(Foo), TPE_CTOR_IN(TPE_CTOR_IN(Foo)), "", {}, {}, transposeOp);
}
#endif

#ifndef DISABLE_REVERSE
TEST_CASE(TPE_NAME "_reverse", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto reverseOp = [](auto &&xs) { return xs OP_ reverse(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {5, 1, 3, 2, 4}, reverseOp);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"apple", "cherry", "banana"}, reverseOp);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(1), Foo(2), Foo(3)}, reverseOp);
  #endif
  RUN_CHECK_ID(int, "", {1}, {1}, reverseOp);
  RUN_CHECK_ID(int, "", {}, {}, reverseOp);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, reverseOp);
  RUN_CHECK_ID(string, "", {}, {}, reverseOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, reverseOp);
  RUN_CHECK_ID(Foo, "", {}, {}, reverseOp);
}
#endif

#ifndef DISABLE_SHUFFLE
TEST_CASE(TPE_NAME "_shuffle", "[" TPE_NAME "][" TPE_GROUP "]") {

  auto shuffleOp = [](auto &&xs) {
    std::mt19937 g(42); // NOLINT(*-msc51-cpp)
    return xs OP_ shuffle(g);
  };
  //
  #ifdef TPE_MANY_INIT

  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, shuffleOp, true);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"banana", "cherry", "apple"}, shuffleOp, true);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(3), Foo(2), Foo(1)}, shuffleOp, true);
  #endif
  RUN_CHECK_ID(int, "", {1}, {1}, shuffleOp);
  RUN_CHECK_ID(int, "", {}, {}, shuffleOp);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, shuffleOp);
  RUN_CHECK_ID(string, "", {}, {}, shuffleOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, shuffleOp);
  RUN_CHECK_ID(Foo, "", {}, {}, shuffleOp);
}
#endif

#ifndef DISABLE_SORT
TEST_CASE(TPE_NAME "_sort", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto sortOp = [](auto &&xs) { return xs OP_ sort(); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {1, 2, 3, 4, 5}, sortOp);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple"}, {"apple", "banana", "cherry"}, sortOp);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1)}, {Foo(1), Foo(2), Foo(3)}, sortOp);
  #endif
  RUN_CHECK_ID(int, "", {1}, {1}, sortOp);
  RUN_CHECK_ID(int, "", {}, {}, sortOp);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, sortOp);
  RUN_CHECK_ID(string, "", {}, {}, sortOp);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, sortOp);
  RUN_CHECK_ID(Foo, "", {}, {}, sortOp);

  auto sortCustomCmpOp = [](auto &&xs) { return xs OP_ sort(std::greater<>()); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "customCmp", {4, 2, 3, 1, 5}, {5, 4, 3, 2, 1}, sortCustomCmpOp);
  #endif
  RUN_CHECK_ID(int, "customCmp", {1}, {1}, sortCustomCmpOp);
  RUN_CHECK_ID(int, "customCmp", {}, {}, sortCustomCmpOp);
}
#endif

#ifndef DISABLE_SORT_BY
TEST_CASE(TPE_NAME "_sort_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto sortByOp = sort_by([](auto s) { return s.size(); });
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(string, "", {"b", "aa", "ccc"}, {"b", "aa", "ccc"}, sortByOp);
  #endif
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, sortByOp);
  RUN_CHECK_ID(string, "", {}, {}, sortByOp);

  using IntStringP = std::pair<int, string>;
  auto sortIntStringOp = sort_by([](auto x, auto) { return x; });
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(IntStringP, "", {{3, "b"}, {2, "aa"}, {1, "ccc"}}, {{1, "ccc"}, {2, "aa"}, {3, "b"}}, sortIntStringOp);
  #endif
  RUN_CHECK_ID(IntStringP, "", {{3, "b"}}, {{3, "b"}}, sortIntStringOp);
  RUN_CHECK_ID(IntStringP, "", {}, {}, sortIntStringOp);
}
#endif

#ifndef DISABLE_SPLIT_AT
TEST_CASE(TPE_NAME "_split_at", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto splitAtOpInt = [](size_t idx) { return [=](auto &&xs) { return xs OP_ split_at(idx); }; };
  auto splitAtOpString = [](size_t idx) { return [=](auto &&xs) { return xs OP_ split_at(idx); }; };
  auto splitAtOpFoo = [](size_t idx) { return [=](auto &&xs) { return xs OP_ split_at(idx); }; };

  using IntP = std::pair<TPE_CTOR_OUT(int), TPE_CTOR_OUT(int)>;
  using StringP = std::pair<TPE_CTOR_OUT(string), TPE_CTOR_OUT(string)>;
  using FooP = std::pair<TPE_CTOR_OUT(Foo), TPE_CTOR_OUT(Foo)>;

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, IntP, "", {4, 2, 3, 1, 5}, {{4, 2}, {3, 1, 5}}, splitAtOpInt(2));
  RUN_CHECK(string, StringP, "", {"banana", "cherry", "apple", "mango"}, {{"banana", "cherry"}, {"apple", "mango"}}, splitAtOpString(2));
  RUN_CHECK(Foo, FooP, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {{Foo(3), Foo(2)}, {Foo(1), Foo(4)}}, splitAtOpFoo(2));
  #endif

  RUN_CHECK(int, IntP, "", {1}, {{1}, {}}, splitAtOpInt(1));
  RUN_CHECK(int, IntP, "", {}, {{}, {}}, splitAtOpInt(1));
  RUN_CHECK(string, StringP, "", {"apple"}, {{"apple"}, {}}, splitAtOpString(1));
  RUN_CHECK(string, StringP, "", {}, {{}, {}}, splitAtOpString(1));
  RUN_CHECK(Foo, FooP, "", {Foo(1)}, {{Foo(1)}, {}}, splitAtOpFoo(1));
  RUN_CHECK(Foo, FooP, "", {}, {{}, {}}, splitAtOpFoo(1));

  RUN_CHECK(int, IntP, "", {1}, {{}, {1}}, splitAtOpInt(0));
  RUN_CHECK(int, IntP, "", {}, {{}, {}}, splitAtOpInt(0));
  RUN_CHECK(string, StringP, "", {"apple"}, {{}, {"apple"}}, splitAtOpString(0));
  RUN_CHECK(string, StringP, "", {}, {{}, {}}, splitAtOpString(0));
  RUN_CHECK(Foo, FooP, "", {Foo(1)}, {{}, {Foo(1)}}, splitAtOpFoo(0));
  RUN_CHECK(Foo, FooP, "", {}, {{}, {}}, splitAtOpFoo(0));
}
#endif

#ifndef DISABLE_TAKE
TEST_CASE(TPE_NAME "_take", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto take3 = [](auto &&xs) { return xs OP_ take(3); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2, 3}, take3);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {"banana", "cherry", "apple"}, take3);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {Foo(3), Foo(2), Foo(1)}, take3);
  #endif

  RUN_CHECK_ID(int, "", {1}, {1}, take3);
  RUN_CHECK_ID(int, "", {}, {}, take3);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, take3);
  RUN_CHECK_ID(string, "", {}, {}, take3);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, take3);
  RUN_CHECK_ID(Foo, "", {}, {}, take3);

  auto take0 = [](auto &&xs) { return xs OP_ take(0); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {}, take0);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {}, take0);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {}, take0);
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, take0);
  RUN_CHECK_ID(int, "", {}, {}, take0);
  RUN_CHECK_ID(string, "", {"apple"}, {}, take0);
  RUN_CHECK_ID(string, "", {}, {}, take0);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, take0);
  RUN_CHECK_ID(Foo, "", {}, {}, take0);
}
#endif

#ifndef DISABLE_DROP
TEST_CASE(TPE_NAME "_drop", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto drop3 = [](auto &&xs) { return xs OP_ drop(3); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {1, 5}, drop3);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {"mango"}, drop3);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {Foo(4)}, drop3);
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, drop3);
  RUN_CHECK_ID(int, "", {}, {}, drop3);
  RUN_CHECK_ID(string, "", {"apple"}, {}, drop3);
  RUN_CHECK_ID(string, "", {}, {}, drop3);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, drop3);
  RUN_CHECK_ID(Foo, "", {}, {}, drop3);

  auto drop0 = [](auto &&xs) { return xs OP_ drop(0); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, drop0);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {"banana", "cherry", "apple", "mango"}, drop0);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {Foo(3), Foo(2), Foo(1), Foo(4)}, drop0);
  #endif

  RUN_CHECK_ID(int, "", {1}, {1}, drop0);
  RUN_CHECK_ID(int, "", {}, {}, drop0);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, drop0);
  RUN_CHECK_ID(string, "", {}, {}, drop0);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, drop0);
  RUN_CHECK_ID(Foo, "", {}, {}, drop0);
}
#endif

#ifndef DISABLE_TAKE_RIGHT
TEST_CASE(TPE_NAME "_take_right", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto takeRight3 = [](auto &&xs) { return xs OP_ take_right(3); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {3, 1, 5}, takeRight3);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {"cherry", "apple", "mango"}, takeRight3);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {Foo(2), Foo(1), Foo(4)}, takeRight3);
  #endif

  RUN_CHECK_ID(int, "", {1}, {1}, takeRight3);
  RUN_CHECK_ID(int, "", {}, {}, takeRight3);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, takeRight3);
  RUN_CHECK_ID(string, "", {}, {}, takeRight3);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, takeRight3);
  RUN_CHECK_ID(Foo, "", {}, {}, takeRight3);

  auto takeRight0 = [](auto &&xs) { return xs OP_ take_right(0); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {}, takeRight0);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {}, takeRight0);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {}, takeRight0);
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, takeRight0);
  RUN_CHECK_ID(int, "", {}, {}, takeRight0);
  RUN_CHECK_ID(string, "", {"apple"}, {}, takeRight0);
  RUN_CHECK_ID(string, "", {}, {}, takeRight0);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, takeRight0);
  RUN_CHECK_ID(Foo, "", {}, {}, takeRight0);
}
#endif

#ifndef DISABLE_DROP_RIGHT
TEST_CASE(TPE_NAME "_drop_right", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto dropRight3 = [](auto &&xs) { return xs OP_ drop_right(3); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2}, dropRight3);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {"banana"}, dropRight3);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {Foo(3)}, dropRight3);
  #endif

  RUN_CHECK_ID(int, "", {1}, {}, dropRight3);
  RUN_CHECK_ID(int, "", {}, {}, dropRight3);
  RUN_CHECK_ID(string, "", {"apple"}, {}, dropRight3);
  RUN_CHECK_ID(string, "", {}, {}, dropRight3);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {}, dropRight3);
  RUN_CHECK_ID(Foo, "", {}, {}, dropRight3);

  auto dropRight0 = [](auto &&xs) { return xs OP_ drop_right(0); };
  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {4, 2, 3, 1, 5}, {4, 2, 3, 1, 5}, dropRight0);
  RUN_CHECK_ID(string, "", {"banana", "cherry", "apple", "mango"}, {"banana", "cherry", "apple", "mango"}, dropRight0);
  RUN_CHECK_ID(Foo, "", {Foo(3), Foo(2), Foo(1), Foo(4)}, {Foo(3), Foo(2), Foo(1), Foo(4)}, dropRight0);
  #endif

  RUN_CHECK_ID(int, "", {1}, {1}, dropRight0);
  RUN_CHECK_ID(int, "", {}, {}, dropRight0);
  RUN_CHECK_ID(string, "", {"apple"}, {"apple"}, dropRight0);
  RUN_CHECK_ID(string, "", {}, {}, dropRight0);
  RUN_CHECK_ID(Foo, "", {Foo(1)}, {Foo(1)}, dropRight0);
  RUN_CHECK_ID(Foo, "", {}, {}, dropRight0);
}
#endif

#ifndef DISABLE_TAKE_WHILE
TEST_CASE(TPE_NAME "_take_while", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intTakeOp = [](auto &&xs) { return xs OP_ take_while([](auto x) { return x < 3; }); };
  auto stringTakeOp = [](auto &&xs) { return xs OP_ take_while([](auto x) { return x.size() < 3; }); };
  auto fooTakeOp = [](auto &&xs) { return xs OP_ take_while([](auto x) { return x.value < 3; }); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {1, 2, 3, 4, 5}, {1, 2}, intTakeOp);
  RUN_CHECK_ID(string, "", {"a", "ab", "abc", "abcd"}, {"a", "ab"}, stringTakeOp);
  RUN_CHECK_ID(Foo, "", {Foo(1), Foo(2), Foo(3)}, {Foo(1), Foo(2)}, fooTakeOp);
  #endif

  RUN_CHECK_ID(int, "", {4}, {}, intTakeOp);
  RUN_CHECK_ID(int, "", {}, {}, intTakeOp);
  RUN_CHECK_ID(string, "", {"abc"}, {}, stringTakeOp);
  RUN_CHECK_ID(string, "", {}, {}, stringTakeOp);
  RUN_CHECK_ID(Foo, "", {Foo(4)}, {}, fooTakeOp);
  RUN_CHECK_ID(Foo, "", {}, {}, fooTakeOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK_ID(P2, name, {{2, 2}, {3, 1}, {1, 3}}, {{2, 2}}, f);
  #endif
    RUN_CHECK_ID(P2, name, {{3, 1}}, {}, f);
    RUN_CHECK_ID(P2, name, {}, {}, f);
  };
  p2("spread", [](auto &&xs) { return xs OP_ take_while([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ take_while([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK_ID(P3, name, {{2, 2, 2}, {3, 1, 3}, {1, 3, 1}}, {{2, 2, 2}}, f);
  #endif
    RUN_CHECK_ID(P3, name, {{3, 1, 3}}, {}, f);
    RUN_CHECK_ID(P3, name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ take_while([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ take_while([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_DROP_WHILE
TEST_CASE(TPE_NAME "_drop_while", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto intTakeOp = [](auto &&xs) { return xs OP_ drop_while([](auto x) { return x < 3; }); };
  auto stringTakeOp = [](auto &&xs) { return xs OP_ drop_while([](auto x) { return x.size() < 3; }); };
  auto fooTakeOp = [](auto &&xs) { return xs OP_ drop_while([](auto x) { return x.value < 3; }); };

  #ifdef TPE_MANY_INIT
  RUN_CHECK_ID(int, "", {1, 2, 3, 4, 5}, {3, 4, 5}, intTakeOp);
  RUN_CHECK_ID(string, "", {"a", "ab", "abc", "abcd"}, {"abc", "abcd"}, stringTakeOp);
  RUN_CHECK_ID(Foo, "", {Foo(1), Foo(2), Foo(3), Foo(4)}, {Foo(3), Foo(4)}, fooTakeOp);

  #endif

  RUN_CHECK_ID(int, "", {4}, {4}, intTakeOp);
  RUN_CHECK_ID(int, "", {}, {}, intTakeOp);
  RUN_CHECK_ID(string, "", {"abc"}, {"abc"}, stringTakeOp);
  RUN_CHECK_ID(string, "", {}, {}, stringTakeOp);
  RUN_CHECK_ID(Foo, "", {Foo(4)}, {Foo(4)}, fooTakeOp);
  RUN_CHECK_ID(Foo, "", {}, {}, fooTakeOp);

  auto p2 = [](auto name, auto f) {
    using P2 = std::pair<int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK_ID(P2, name, {{2, 2}, {3, 1}, {1, 3}}, {{3, 1}, {1, 3}}, f);
  #endif
    RUN_CHECK_ID(P2, name, {{3, 1}}, {{3, 1}}, f);
    RUN_CHECK_ID(P2, name, {}, {}, f);
  };
  p2("spread", [](auto &&xs) { return xs OP_ drop_while([](auto x0, auto x1) { return x0 == x1; }); });
  p2("single", [](auto &&xs) { return xs OP_ drop_while([](auto x) { return get<0>(x) == get<1>(x); }); });

  auto p3 = [](auto name, auto f) {
    using P3 = std::tuple<int, int, int>;
  #ifdef TPE_MANY_INIT
    RUN_CHECK_ID(P3, name, {{2, 2, 2}, {3, 1, 3}, {1, 3, 1}}, {{3, 1, 3}, {1, 3, 1}}, f);
  #endif
    RUN_CHECK_ID(P3, name, {{3, 1, 3}}, {{3, 1, 3}}, f);
    RUN_CHECK_ID(P3, name, {}, {}, f);
  };
  p3("spread", [](auto &&xs) { return xs OP_ drop_while([](auto x0, auto x1, auto x2) { return x0 == x1 && x1 == x2; }); });
  p3("single", [](auto &&xs) { return xs OP_ drop_while([](auto x) { return get<0>(x) == get<1>(x) && get<1>(x) == get<2>(x); }); });
}
#endif

#ifndef DISABLE_FOLD_LEFT
TEST_CASE(TPE_NAME "_fold_left", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto foldLeftOpInt = [](auto &&xs) {
    return xs OP_ fold_left(std::pair{0, 0}, [](auto a, auto x) { return std::pair{a.first + x, a.second + 1}; });
  };
  auto foldLeftOpString = [](auto &&xs) {
    return xs OP_ fold_left(std::pair{std::string(""), 0}, [](auto a, auto x) { return std::pair{a.first + x, a.second + 1}; });
  };
  auto foldLeftOpFoo = [](auto &&xs) {
    return xs OP_ fold_left(std::pair{Foo(0), 0}, [](auto a, auto x) { return std::pair{Foo(a.first.value + x.value), a.second + 1}; });
  };

  using IntP = std::pair<int, int>;
  using StringP = std::pair<std::string, int>;
  using FooP = std::pair<Foo, int>;

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, IntP, "", {4, 2, 3, 1, 5}, {15, 5}, foldLeftOpInt);
  RUN_CHECK(string, StringP, "", {"banana", "cherry", "apple"}, {"bananacherryapple", 3}, foldLeftOpString);
  RUN_CHECK(Foo, FooP, "", {Foo(3), Foo(2), Foo(1)}, {Foo(6), 3}, foldLeftOpFoo);
  #endif

  RUN_CHECK(int, IntP, "", {1}, {1, 1}, foldLeftOpInt);
  RUN_CHECK(int, IntP, "", {}, {0, 0}, foldLeftOpInt);
  RUN_CHECK(string, StringP, "", {"apple"}, {"apple", 1}, foldLeftOpString);
  RUN_CHECK(string, StringP, "", {}, {"", 0}, foldLeftOpString);
  RUN_CHECK(Foo, FooP, "", {Foo(1)}, {Foo(1), 1}, foldLeftOpFoo);
  RUN_CHECK(Foo, FooP, "", {}, {Foo(0), 0}, foldLeftOpFoo);
}
#endif

#ifndef DISABLE_FOLD_RIGHT
TEST_CASE(TPE_NAME "_fold_right", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto foldRightOpInt = [](auto &&xs) {
    return xs OP_ fold_right(std::pair{0, 0}, [](auto x, auto a) { return std::pair{x + a.first, a.second + 1}; });
  };
  auto foldRightOpString = [](auto &&xs) {
    return xs OP_ fold_right(std::pair{std::string(""), 0}, [](auto x, auto a) { return std::pair{x + a.first, a.second + 1}; });
  };
  auto foldRightOpFoo = [](auto &&xs) {
    return xs OP_ fold_right(std::pair{Foo(0), 0}, [](auto x, auto a) { return std::pair{Foo(x.value + a.first.value), a.second + 1}; });
  };

  using IntP = std::pair<int, int>;
  using StringP = std::pair<std::string, int>;
  using FooP = std::pair<Foo, int>;

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, IntP, "", {4, 2, 3, 1, 5}, {15, 5}, foldRightOpInt);
  RUN_CHECK(string, StringP, "", {"banana", "cherry", "apple"}, {"bananacherryapple", 3}, foldRightOpString);
  RUN_CHECK(Foo, FooP, "", {Foo(3), Foo(2), Foo(1)}, {Foo(6), 3}, foldRightOpFoo);
  #endif

  RUN_CHECK(int, IntP, "", {1}, {1, 1}, foldRightOpInt);
  RUN_CHECK(int, IntP, "", {}, {0, 0}, foldRightOpInt);
  RUN_CHECK(string, StringP, "", {"apple"}, {"apple", 1}, foldRightOpString);
  RUN_CHECK(string, StringP, "", {}, {"", 0}, foldRightOpString);
  RUN_CHECK(Foo, FooP, "", {Foo(1)}, {Foo(1), 1}, foldRightOpFoo);
  RUN_CHECK(Foo, FooP, "", {}, {Foo(0), 0}, foldRightOpFoo);
}
#endif

#ifndef DISABLE_SLIDING
TEST_CASE(TPE_NAME "_sliding", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto slidingOpInt = [](size_t size, size_t step) { return [=](auto &&xs) { return xs OP_ sliding(size, step); }; };
  auto slidingOpString = [](size_t size, size_t step) { return [=](auto &&xs) { return xs OP_ sliding(size, step); }; };
  auto slidingOpFoo = [](size_t size, size_t step) { return [=](auto &&xs) { return xs OP_ sliding(size, step); }; };

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2}, {2, 3}, {3, 1}, {1, 5}}, slidingOpInt(2, 1));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2}, {3, 1}, {5}}, slidingOpInt(2, 2));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2}, {1, 5}}, slidingOpInt(2, 3));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2}, {5}}, slidingOpInt(2, 4));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2}}, slidingOpInt(2, 5));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2}}, slidingOpInt(2, 6));

  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2, 3, 1, 5}}, slidingOpInt(10, 1));

  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {"banana", "cherry", "apple"}, {{"banana", "cherry"}, {"cherry", "apple"}},
            slidingOpString(2, 1));
  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {"banana", "cherry", "apple"}, {{"banana", "cherry", "apple"}},
            slidingOpString(10, 1));

  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {Foo(3), Foo(2), Foo(1)}, {{Foo(3), Foo(2)}, {Foo(2), Foo(1)}}, slidingOpFoo(2, 1));
  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {Foo(3), Foo(2), Foo(1)}, {{Foo(3), Foo(2), Foo(1)}}, slidingOpFoo(10, 1));
  #endif

  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {1}, {{1}}, slidingOpInt(1, 1));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {}, {}, slidingOpInt(1, 1));
  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {"apple"}, {{"apple"}}, slidingOpString(1, 1));
  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {}, {}, slidingOpString(1, 1));
  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {Foo(1)}, {{Foo(1)}}, slidingOpFoo(1, 1));
  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {}, {}, slidingOpFoo(1, 1));
}
#endif

#ifndef DISABLE_GROUPED
TEST_CASE(TPE_NAME "_grouped", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto slidingOpInt = [](size_t size) { return [=](auto &&xs) { return xs OP_ grouped(size); }; };
  auto slidingOpString = [](size_t size) { return [=](auto &&xs) { return xs OP_ grouped(size); }; };
  auto slidingOpFoo = [](size_t size) { return [=](auto &&xs) { return xs OP_ grouped(size); }; };

  #ifdef TPE_MANY_INIT
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2}, {3, 1}, {5}}, slidingOpInt(2));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {4, 2, 3, 1, 5}, {{4, 2, 3, 1, 5}}, slidingOpInt(10));

  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {"banana", "cherry", "apple"}, {{"banana", "cherry"}, {"apple"}},
            slidingOpString(2));
  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {"banana", "cherry", "apple"}, {{"banana", "cherry", "apple"}},
            slidingOpString(10));
  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {Foo(3), Foo(2), Foo(1)}, {{Foo(3), Foo(2)}, {Foo(1)}}, slidingOpFoo(2));
  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {Foo(3), Foo(2), Foo(1)}, {{Foo(3), Foo(2), Foo(1)}}, slidingOpFoo(10));
  #endif

  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {1}, {{1}}, slidingOpInt(1));
  RUN_CHECK(int, TPE_CTOR_OUT(TPE_CTOR_OUT(int)), "", {}, {}, slidingOpInt(1));
  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {"apple"}, {{"apple"}}, slidingOpString(1));
  RUN_CHECK(string, TPE_CTOR_OUT(TPE_CTOR_OUT(string)), "", {}, {}, slidingOpString(1));
  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {Foo(1)}, {{Foo(1)}}, slidingOpFoo(1));
  RUN_CHECK(Foo, TPE_CTOR_OUT(TPE_CTOR_OUT(Foo)), "", {}, {}, slidingOpFoo(1));
}
#endif