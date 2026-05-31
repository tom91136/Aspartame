#include <algorithm>
#include <random>
#include <vector>

#ifndef DISABLE_INPLACE_FILTER
TEST_CASE(TPE_NAME "_inplace_filter", "[" TPE_NAME "][inplace]") {
  auto intOp = [](auto &xs) -> auto & { return xs ^= filter([](int x) { return x % 2 == 0; }); };
  auto strOp = [](auto &xs) -> auto & { return xs ^= filter([](const string &s) { return s.size() >= 2; }); };
  auto fooOp = [](auto &xs) -> auto & { return xs ^= filter([](const Foo &f) { return f.value % 2 == 0; }); };

  RUN_CHECK_INPLACE(int, "", {4, 2, 3, 1, 5}, (TPE_CTOR_IN(int){4, 2}), intOp);
  RUN_CHECK_INPLACE(int, "none kept", {1, 3, 5}, (TPE_CTOR_IN(int){}), intOp);
  RUN_CHECK_INPLACE(int, "empty", {}, (TPE_CTOR_IN(int){}), intOp);
  RUN_CHECK_INPLACE(string, "", {"a", "bb", "ccc"}, (TPE_CTOR_IN(string){"bb", "ccc"}), strOp);
  RUN_CHECK_INPLACE(Foo, "", {Foo(4), Foo(1), Foo(2)}, (TPE_CTOR_IN(Foo){Foo(4), Foo(2)}), fooOp);
}
#endif

#ifdef TPE_INPLACE_SEQ

TEST_CASE(TPE_NAME "_inplace_map", "[" TPE_NAME "][inplace]") {
  auto intOp = [](auto &xs) -> auto & { return xs ^= map([](int x) { return x * 2; }); };
  auto strOp = [](auto &xs) -> auto & { return xs ^= map([](const string &s) { return s + "!"; }); };
  auto fooOp = [](auto &xs) -> auto & { return xs ^= map([](const Foo &f) { return Foo(f.value + 1); }); };

  RUN_CHECK_INPLACE(int, "", {1, 2, 3}, (TPE_CTOR_IN(int){2, 4, 6}), intOp);
  RUN_CHECK_INPLACE(int, "empty", {}, (TPE_CTOR_IN(int){}), intOp);
  RUN_CHECK_INPLACE(string, "", {"a", "b"}, (TPE_CTOR_IN(string){"a!", "b!"}), strOp);
  RUN_CHECK_INPLACE(Foo, "", {Foo(1), Foo(2)}, (TPE_CTOR_IN(Foo){Foo(2), Foo(3)}), fooOp);
}

TEST_CASE(TPE_NAME "_inplace_sort", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= sort(); };
  RUN_CHECK_INPLACE(int, "", {4, 2, 3, 1, 5}, (TPE_CTOR_IN(int){1, 2, 3, 4, 5}), op);
  RUN_CHECK_INPLACE(string, "", {"cherry", "apple", "banana"}, (TPE_CTOR_IN(string){"apple", "banana", "cherry"}), op);
  RUN_CHECK_INPLACE(Foo, "", {Foo(3), Foo(1), Foo(2)}, (TPE_CTOR_IN(Foo){Foo(1), Foo(2), Foo(3)}), op);
}

TEST_CASE(TPE_NAME "_inplace_sort_cmp", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= sort([](int a, int b) { return a > b; }); };
  RUN_CHECK_INPLACE(int, "", {4, 2, 3, 1, 5}, (TPE_CTOR_IN(int){5, 4, 3, 2, 1}), op);
}

TEST_CASE(TPE_NAME "_inplace_reverse", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= reverse(); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3, 4}, (TPE_CTOR_IN(int){4, 3, 2, 1}), op);
  RUN_CHECK_INPLACE(int, "empty", {}, (TPE_CTOR_IN(int){}), op);
}

TEST_CASE(TPE_NAME "_inplace_take", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= take(2); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3, 4}, (TPE_CTOR_IN(int){1, 2}), op);
  RUN_CHECK_INPLACE(int, "over", {1}, (TPE_CTOR_IN(int){1}), op);
}

TEST_CASE(TPE_NAME "_inplace_drop", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= drop(2); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3, 4}, (TPE_CTOR_IN(int){3, 4}), op);
  RUN_CHECK_INPLACE(int, "over", {1}, (TPE_CTOR_IN(int){}), op);
}

TEST_CASE(TPE_NAME "_inplace_take_while", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= take_while([](int x) { return x < 3; }); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3, 4, 1}, (TPE_CTOR_IN(int){1, 2}), op);
}

TEST_CASE(TPE_NAME "_inplace_drop_while", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= drop_while([](int x) { return x < 3; }); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3, 4, 1}, (TPE_CTOR_IN(int){3, 4, 1}), op);
}

TEST_CASE(TPE_NAME "_inplace_append", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= append(9); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3}, (TPE_CTOR_IN(int){1, 2, 3, 9}), op);
  RUN_CHECK_INPLACE(int, "empty", {}, (TPE_CTOR_IN(int){9}), op);
}

TEST_CASE(TPE_NAME "_inplace_prepend", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= prepend(0); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3}, (TPE_CTOR_IN(int){0, 1, 2, 3}), op);
}

TEST_CASE(TPE_NAME "_inplace_concat", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= concat(TPE_CTOR_IN(int){4, 5}); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3}, (TPE_CTOR_IN(int){1, 2, 3, 4, 5}), op);
}

TEST_CASE(TPE_NAME "_inplace_distinct", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= distinct(); };
  RUN_CHECK_INPLACE(int, "", {1, 2, 2, 3, 1, 3}, (TPE_CTOR_IN(int){1, 2, 3}), op);
}

TEST_CASE(TPE_NAME "_inplace_distinct_by", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= distinct_by([](int x) { return x % 3; }); };
  RUN_CHECK_INPLACE(int, "", {1, 4, 2, 5, 3}, (TPE_CTOR_IN(int){1, 2, 3}), op);
}

TEST_CASE(TPE_NAME "_inplace_slice", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & { return xs ^= slice(1, 4); };
  RUN_CHECK_INPLACE(int, "", {0, 1, 2, 3, 4, 5}, (TPE_CTOR_IN(int){1, 2, 3}), op);
}

TEST_CASE(TPE_NAME "_inplace_tap_each", "[" TPE_NAME "][inplace]") {
  auto op = [](auto &xs) -> auto & {
    int sum = 0;
    auto &r = (xs ^= tap_each([&](int x) { sum += x; }));
    CHECK(sum == 6);
    return r;
  };
  RUN_CHECK_INPLACE(int, "", {1, 2, 3}, (TPE_CTOR_IN(int){1, 2, 3}), op);
}

TEST_CASE(TPE_NAME "_inplace_shuffle_is_permutation", "[" TPE_NAME "][inplace]") {
  std::mt19937 rng(123);
  TPE_CTOR_IN(int) xs{1, 2, 3, 4, 5, 6, 7, 8};
  TPE_CTOR_IN(int) before = xs;
  xs ^= shuffle(rng);
  std::vector<int> a{xs.begin(), xs.end()}, b{before.begin(), before.end()};
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  CHECK(a == b);
}

#endif // TPE_INPLACE_SEQ
