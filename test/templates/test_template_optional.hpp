#include <string>
#include <utility>
#include <vector>

#include "catch2/catch_test_macros.hpp"

#include "../test_base_container1.hpp"

TEST_CASE(TPE_NAME "_fold", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::string, "", {1}, "1",
            [](auto xs) { return xs OP_ fold([](auto x) { return std::to_string(x); }, [] { return std::string{"a"}; }); });
  RUN_CHECK(int, std::string, "", {}, "a",
            [](auto xs) { return xs OP_ fold([](auto x) { return std::to_string(x); }, [] { return std::string{"a"}; }); });

  using IntP = std::pair<int, int>;
  RUN_CHECK(IntP, std::string, "", {{1, 1}}, "2",
            [](auto xs) { return xs OP_ fold([](auto x, auto y) { return std::to_string(x + y); }, [] { return std::string{"a"}; }); });
  RUN_CHECK(IntP, std::string, "", {}, "a",
            [](auto xs) { return xs OP_ fold([](auto x, auto y) { return std::to_string(x + y); }, [] { return std::string{"a"}; }); });
}

TEST_CASE(TPE_NAME "_get_or_else", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {1}, 1, [](auto xs) { return xs OP_ get_or_else(42); });
  RUN_CHECK(int, int, "", {}, 42, [](auto xs) { return xs OP_ get_or_else(42); });

  RUN_CHECK(string, string, "", {"1"}, "1", [](auto xs) { return xs OP_ get_or_else(string{"42"}); });
  RUN_CHECK(string, string, "", {}, "42", [](auto xs) { return xs OP_ get_or_else(string{"42"}); });

  RUN_CHECK(Foo, Foo, "", {Foo(1)}, Foo(1), [](auto xs) { return xs OP_ get_or_else(Foo(42)); });
  RUN_CHECK(Foo, Foo, "", {}, Foo(42), [](auto xs) { return xs OP_ get_or_else(Foo(42)); });
}

TEST_CASE(TPE_NAME "_get_or_default", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {1}, 1, [](auto xs) { return xs OP_ get_or_default(); });
  RUN_CHECK(int, int, "", {}, int{}, [](auto xs) { return xs OP_ get_or_default(); });

  RUN_CHECK(string, string, "", {"1"}, "1", [](auto xs) { return xs OP_ get_or_default(); });
  RUN_CHECK(string, string, "", {}, string{}, [](auto xs) { return xs OP_ get_or_default(); });
}

TEST_CASE(TPE_NAME "_or_else_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {1}, TPE_CTOR_OUT(int){1}, [](auto xs) { return xs OP_ or_else_maybe(TPE_CTOR_OUT(int){42}); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){42}, [](auto xs) { return xs OP_ or_else_maybe(TPE_CTOR_OUT(int){42}); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {1}, TPE_CTOR_OUT(int){1}, [](auto xs) { return xs OP_ or_else_maybe(TPE_CTOR_OUT(int){}); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){}, [](auto xs) { return xs OP_ or_else_maybe(TPE_CTOR_OUT(int){}); });
}

TEST_CASE(TPE_NAME "_or_else", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {1}, TPE_CTOR_OUT(int){1},
            [](auto xs) { return xs OP_ or_else([]() { return TPE_CTOR_OUT(int){42}; }); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){42},
            [](auto xs) { return xs OP_ or_else([]() { return TPE_CTOR_OUT(int){42}; }); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {1}, TPE_CTOR_OUT(int){1},
            [](auto xs) { return xs OP_ or_else([]() { return TPE_CTOR_OUT(int){}; }); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){},
            [](auto xs) { return xs OP_ or_else([]() { return TPE_CTOR_OUT(int){}; }); });
}

TEST_CASE(TPE_NAME "_intersect", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {2}, TPE_CTOR_OUT(int){2}, [](auto xs) { return xs OP_ intersect(std::vector<int>{1, 2, 3}); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {5}, TPE_CTOR_OUT(int){}, [](auto xs) { return xs OP_ intersect(std::vector<int>{1, 2, 3}); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){}, [](auto xs) { return xs OP_ intersect(std::vector<int>{1, 2}); });
}

TEST_CASE(TPE_NAME "_diff", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {5}, TPE_CTOR_OUT(int){5}, [](auto xs) { return xs OP_ diff(std::vector<int>{1, 2, 3}); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {2}, TPE_CTOR_OUT(int){}, [](auto xs) { return xs OP_ diff(std::vector<int>{1, 2, 3}); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){}, [](auto xs) { return xs OP_ diff(std::vector<int>{1, 2}); });
}

TEST_CASE(TPE_NAME "_symmetric_difference", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, std::vector<int>, "", {2}, {1, 3}, [](auto xs) { return xs OP_ symmetric_difference(std::vector<int>{1, 2, 3}); });
  RUN_CHECK(int, std::vector<int>, "", {5}, {5, 1, 2, 3}, [](auto xs) { return xs OP_ symmetric_difference(std::vector<int>{1, 2, 3}); });
  RUN_CHECK(int, std::vector<int>, "", {}, {1, 2}, [](auto xs) { return xs OP_ symmetric_difference(std::vector<int>{1, 2}); });
}

TEST_CASE(TPE_NAME "_cross", "[" TPE_NAME "][" TPE_GROUP "]") {
  using P = std::pair<int, int>;
  RUN_CHECK(int, std::vector<P>, "", {5}, {{5, 1}, {5, 2}, {5, 3}}, [](auto xs) { return xs OP_ cross(std::vector<int>{1, 2, 3}); });
  RUN_CHECK(int, std::vector<P>, "", {}, {}, [](auto xs) { return xs OP_ cross(std::vector<int>{1, 2}); });
}

TEST_CASE(TPE_NAME "_scan_left", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto plus = [](int a, int b) { return a + b; };
  RUN_CHECK(int, std::vector<int>, "", {3}, {10, 13}, [&](auto xs) { return xs OP_ scan_left(10, plus); });
  RUN_CHECK(int, std::vector<int>, "", {}, {10}, [&](auto xs) { return xs OP_ scan_left(10, plus); });
}

TEST_CASE(TPE_NAME "_scan_right", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto plus = [](int a, int b) { return a + b; };
  RUN_CHECK(int, std::vector<int>, "", {3}, {13, 10}, [&](auto xs) { return xs OP_ scan_right(10, plus); });
  RUN_CHECK(int, std::vector<int>, "", {}, {10}, [&](auto xs) { return xs OP_ scan_right(10, plus); });
}

TEST_CASE(TPE_NAME "_unzip", "[" TPE_NAME "][" TPE_GROUP "]") {
  using P2 = std::pair<int, string>;
  using Expected = std::pair<TPE_CTOR_OUT(int), TPE_CTOR_OUT(string)>;
  RUN_CHECK(P2, Expected, "", {{42, "hi"}}, Expected{TPE_CTOR_OUT(int){42}, TPE_CTOR_OUT(string){"hi"}},
            [](auto xs) { return xs OP_ unzip(); });
  RUN_CHECK(P2, Expected, "", {}, Expected{TPE_CTOR_OUT(int){}, TPE_CTOR_OUT(string){}}, [](auto xs) { return xs OP_ unzip(); });
}

TEST_CASE(TPE_NAME "_combinations", "[" TPE_NAME "][" TPE_GROUP "]") {
  using VV = std::vector<std::vector<int>>;
  RUN_CHECK(int, VV, "", {7}, {{}}, [](auto xs) { return xs OP_ combinations(0); });
  RUN_CHECK(int, VV, "", {7}, {{7}}, [](auto xs) { return xs OP_ combinations(1); });
  RUN_CHECK(int, VV, "", {7}, {}, [](auto xs) { return xs OP_ combinations(2); });
  RUN_CHECK(int, VV, "", {}, {}, [](auto xs) { return xs OP_ combinations(1); });
}

TEST_CASE(TPE_NAME "_permutations", "[" TPE_NAME "][" TPE_GROUP "]") {
  using VV = std::vector<std::vector<int>>;
  RUN_CHECK(int, VV, "", {7}, {{7}}, [](auto xs) { return xs OP_ permutations(); });
  RUN_CHECK(int, VV, "", {}, {{}}, [](auto xs) { return xs OP_ permutations(); });
}

TEST_CASE(TPE_NAME "_chunk_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  using VV = std::vector<std::vector<int>>;
  auto eq = [](int, int) { return true; };
  RUN_CHECK(int, VV, "", {7}, {{7}}, [&](auto xs) { return xs OP_ chunk_by(eq); });
  RUN_CHECK(int, VV, "", {}, {}, [&](auto xs) { return xs OP_ chunk_by(eq); });
}

TEST_CASE(TPE_NAME "_join_with", "[" TPE_NAME "][" TPE_GROUP "]") {
  using V = std::vector<int>;
  RUN_CHECK(V, V, "", {{1, 2, 3}}, {1, 2, 3}, [](auto xs) { return xs OP_ join_with(V{0}); });
  RUN_CHECK(V, V, "", {}, {}, [](auto xs) { return xs OP_ join_with(V{0}); });
}

TEST_CASE(TPE_NAME "_pipe_basic", "[" TPE_NAME "][" TPE_GROUP "]") {
  REQUIRE((TPE_CTOR_IN(int){2} | map([](auto x) { return x * 3; })) == TPE_CTOR_OUT(int){6});
  REQUIRE((TPE_CTOR_IN(int){} | map([](auto x) { return x * 3; })) == TPE_CTOR_OUT(int){});

  REQUIRE((TPE_CTOR_IN(int){4} | filter([](auto x) { return x % 2 == 0; })) == TPE_CTOR_OUT(int){4});
  REQUIRE((TPE_CTOR_IN(int){3} | filter([](auto x) { return x % 2 == 0; })) == TPE_CTOR_OUT(int){});
  REQUIRE((TPE_CTOR_IN(int){} | filter([](auto x) { return x % 2 == 0; })) == TPE_CTOR_OUT(int){});

  REQUIRE((TPE_CTOR_IN(int){7} | get_or_else(0)) == 7);
  REQUIRE((TPE_CTOR_IN(int){} | get_or_else(42)) == 42);
}

TEST_CASE(TPE_NAME "_pipe_equals_caret", "[" TPE_NAME "][" TPE_GROUP "]") {
  const TPE_CTOR_IN(int) some{7};
  const TPE_CTOR_IN(int) none{};
  REQUIRE((some | map([](auto x) { return x * 2; })) == (some ^ map([](auto x) { return x * 2; })));
  REQUIRE((none | map([](auto x) { return x * 2; })) == (none ^ map([](auto x) { return x * 2; })));
  REQUIRE((some | get_or_else(0)) == (some ^ get_or_else(0)));
  REQUIRE((none | get_or_else(42)) == (none ^ get_or_else(42)));
}

TEST_CASE(TPE_NAME "_ensure", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ ensure([](int x) { return x > 0; }); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "pass", {5}, TPE_CTOR_OUT(int){5}, op);
  RUN_CHECK(int, TPE_CTOR_OUT(int), "fail", {-1}, TPE_CTOR_OUT(int){}, op);
  RUN_CHECK(int, TPE_CTOR_OUT(int), "empty", {}, TPE_CTOR_OUT(int){}, op);
}

TEST_CASE(TPE_NAME "_pipe_chained", "[" TPE_NAME "][" TPE_GROUP "]") {
  const auto result = TPE_CTOR_IN(int){5}                    //
                      | map([](auto x) { return x + 1; })    //
                      | filter([](auto x) { return x > 3; }) //
                      | get_or_else(0);
  REQUIRE(result == 6);

  const auto empty = TPE_CTOR_IN(int){1}                     //
                     | filter([](auto x) { return x > 10; }) //
                     | map([](auto x) { return std::to_string(x); }) | get_or_else(std::string{"none"});
  REQUIRE(empty == "none");
}
