#include "test_base_includes.hpp"

#define OP_ ^
#define TPE_GROUP "test_optional"
#define TPE_NAME "std::optional"
#define TPE_CTOR_IN(T) std::optional<T>
#define TPE_CTOR_OUT(T) std::optional<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) (empty(x) ? std::nullopt : std::optional{*x.begin()})
#define TPE_INIT_SKIP(x) ((x).size() > 1)

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_sequence.hpp"

using std::optional;

TEST_CASE(TPE_NAME "_fold", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, string, "", {1}, "1", [](auto xs) { return xs OP_ fold([](auto x) { return std::to_string(x); }, [] { return "a"; }); });
  RUN_CHECK(int, string, "", {}, "a", [](auto xs) { return xs OP_ fold([](auto x) { return std::to_string(x); }, [] { return "a"; }); });

  using IntP = std::pair<int, int>;
  RUN_CHECK(IntP, string, "", {{1, 1}}, "2",
            [](auto xs) { return xs OP_ fold([](auto x, auto y) { return std::to_string(x + y); }, [] { return "a"; }); });
  RUN_CHECK(IntP, string, "", {}, "a",
            [](auto xs) { return xs OP_ fold([](auto x, auto y) { return std::to_string(x + y); }, [] { return "a"; }); });
}

TEST_CASE(TPE_NAME "_get_or_else", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {1}, 1, [](auto xs) { return xs OP_ get_or_else(42); });
  RUN_CHECK(int, int, "", {}, 42, [](auto xs) { return xs OP_ get_or_else(42); });

  RUN_CHECK(string, string, "", {"1"}, "1", [](auto xs) { return xs OP_ get_or_else("42"); });
  RUN_CHECK(string, string, "", {}, "42", [](auto xs) { return xs OP_ get_or_else("42"); });

  RUN_CHECK(Foo, Foo, "", {Foo(1)}, Foo(1), [](auto xs) { return xs OP_ get_or_else(Foo(42)); });
  RUN_CHECK(Foo, Foo, "", {}, Foo(42), [](auto xs) { return xs OP_ get_or_else(Foo(42)); });
}

TEST_CASE(TPE_NAME "_get_or_default", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {1}, 1, [](auto xs) { return xs OP_ get_or_default(); });
  RUN_CHECK(int, int, "", {}, int{}, [](auto xs) { return xs OP_ get_or_default(); });

  RUN_CHECK(string, string, "", {"1"}, "1", [](auto xs) { return xs OP_ get_or_default(); });
  RUN_CHECK(string, string, "", {}, string{}, [](auto xs) { return xs OP_ get_or_default(); });
  // XXX Foo is not default constructible
}

TEST_CASE(TPE_NAME "_or_else_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, optional<int>, "", {1}, {1}, [](auto xs) { return xs OP_ or_else_maybe(optional{42}); });
  RUN_CHECK(int, optional<int>, "", {}, {42}, [](auto xs) { return xs OP_ or_else_maybe(optional{42}); });
  RUN_CHECK(int, optional<int>, "", {1}, {1}, [](auto xs) { return xs OP_ or_else_maybe(std::nullopt); });
  RUN_CHECK(int, optional<int>, "", {}, {}, [](auto xs) { return xs OP_ or_else_maybe(std::nullopt); });

  RUN_CHECK(string, optional<string>, "", {"1"}, {"1"}, [](auto xs) { return xs OP_ or_else_maybe(optional{"42"}); });
  RUN_CHECK(string, optional<string>, "", {}, {"42"}, [](auto xs) { return xs OP_ or_else_maybe(optional{"42"}); });
  RUN_CHECK(string, optional<string>, "", {"1"}, {"1"}, [](auto xs) { return xs OP_ or_else_maybe(std::nullopt); });
  RUN_CHECK(string, optional<string>, "", {}, {}, [](auto xs) { return xs OP_ or_else_maybe(std::nullopt); });

  RUN_CHECK(Foo, optional<Foo>, "", {Foo(1)}, {Foo(1)}, [](auto xs) { return xs OP_ or_else_maybe(optional{Foo(42)}); });
  RUN_CHECK(Foo, optional<Foo>, "", {}, {Foo(42)}, [](auto xs) { return xs OP_ or_else_maybe(optional{Foo(42)}); });
  RUN_CHECK(Foo, optional<Foo>, "", {Foo(1)}, {Foo(1)}, [](auto xs) { return xs OP_ or_else_maybe(std::nullopt); });
  RUN_CHECK(Foo, optional<Foo>, "", {}, {}, [](auto xs) { return xs OP_ or_else_maybe(std::nullopt); });
}

TEST_CASE(TPE_NAME "_or_else", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, optional<int>, "", {1}, {1}, [](auto xs) { return xs OP_ or_else([]() { return optional{42}; }); });
  RUN_CHECK(int, optional<int>, "", {}, {42}, [](auto xs) { return xs OP_ or_else([]() { return optional{42}; }); });
  RUN_CHECK(int, optional<int>, "", {1}, {1}, [](auto xs) { return xs OP_ or_else([]() { return std::nullopt; }); });
  RUN_CHECK(int, optional<int>, "", {}, {}, [](auto xs) { return xs OP_ or_else([]() { return std::nullopt; }); });

  RUN_CHECK(string, optional<string>, "", {"1"}, {"1"}, [](auto xs) { return xs OP_ or_else([]() { return optional{"42"}; }); });
  RUN_CHECK(string, optional<string>, "", {}, {"42"}, [](auto xs) { return xs OP_ or_else([]() { return optional{"42"}; }); });
  RUN_CHECK(string, optional<string>, "", {"1"}, {"1"}, [](auto xs) { return xs OP_ or_else([]() { return std::nullopt; }); });
  RUN_CHECK(string, optional<string>, "", {}, {}, [](auto xs) { return xs OP_ or_else([]() { return std::nullopt; }); });

  RUN_CHECK(Foo, optional<Foo>, "", {Foo(1)}, {Foo(1)}, [](auto xs) { return xs OP_ or_else([]() { return optional{Foo(42)}; }); });
  RUN_CHECK(Foo, optional<Foo>, "", {}, {Foo(42)}, [](auto xs) { return xs OP_ or_else([]() { return optional{Foo(42)}; }); });
  RUN_CHECK(Foo, optional<Foo>, "", {Foo(1)}, {Foo(1)}, [](auto xs) { return xs OP_ or_else([]() { return std::nullopt; }); });
  RUN_CHECK(Foo, optional<Foo>, "", {}, {}, [](auto xs) { return xs OP_ or_else([]() { return std::nullopt; }); });
}