#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "catch2/catch_test_macros.hpp"

#include "../test_base_container1.hpp"

TEST_CASE(TPE_NAME "_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ map([](int x) { return x * 2; }); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {7}, TPE_CTOR_OUT(int){14}, op);
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_MAKE_ERR(int), op);
}

TEST_CASE(TPE_NAME "_map_error", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ map_error([](const std::string &e) { return static_cast<int>(e.size()); }) OP_ to_optional(); };
  RUN_CHECK(int, std::optional<int>, "", {7}, std::optional<int>{7}, op);
  RUN_CHECK(int, std::optional<int>, "", {}, std::optional<int>{}, op);
}

TEST_CASE(TPE_NAME "_flat_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto step = [](int x) { return TPE_CTOR_OUT(int){x * 2}; };
  auto op = [&](auto xs) { return xs OP_ flat_map(step); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {3}, TPE_CTOR_OUT(int){6}, op);
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_MAKE_ERR(int), op);
}

TEST_CASE(TPE_NAME "_flat_map_error", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto recover = [](const std::string &e) { return TPE_CTOR_OUT(int){static_cast<int>(e.size())}; };
  auto stay_err = [](const std::string &) { return TPE_MAKE_ERR(int); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "recover", {7}, TPE_CTOR_OUT(int){7}, [&](auto xs) { return xs OP_ flat_map_error(recover); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "recover", {}, TPE_CTOR_OUT(int){5}, [&](auto xs) { return xs OP_ flat_map_error(recover); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "stay", {7}, TPE_CTOR_OUT(int){7}, [&](auto xs) { return xs OP_ flat_map_error(stay_err); });
  RUN_CHECK(int, TPE_CTOR_OUT(int), "stay", {}, TPE_MAKE_ERR(int), [&](auto xs) { return xs OP_ flat_map_error(stay_err); });
}

TEST_CASE(TPE_NAME "_or_else", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto recover = [](const std::string &e) { return TPE_CTOR_OUT(int){static_cast<int>(e.size())}; };
  auto op = [&](auto xs) { return xs OP_ or_else(recover); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {7}, TPE_CTOR_OUT(int){7}, op);
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){5}, op);
}

TEST_CASE(TPE_NAME "_or_else_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ or_else_maybe(TPE_CTOR_OUT(int){99}); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {7}, TPE_CTOR_OUT(int){7}, op);
  RUN_CHECK(int, TPE_CTOR_OUT(int), "", {}, TPE_CTOR_OUT(int){99}, op);
}

TEST_CASE(TPE_NAME "_get_or_else", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {7}, 7, [](auto xs) { return xs OP_ get_or_else(42); });
  RUN_CHECK(int, int, "", {}, 42, [](auto xs) { return xs OP_ get_or_else(42); });
}

TEST_CASE(TPE_NAME "_get_or_default", "[" TPE_NAME "][" TPE_GROUP "]") {
  RUN_CHECK(int, int, "", {7}, 7, [](auto xs) { return xs OP_ get_or_default(); });
  RUN_CHECK(int, int, "", {}, 0, [](auto xs) { return xs OP_ get_or_default(); });
}

TEST_CASE(TPE_NAME "_fold", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ fold([](int x) { return std::to_string(x); }, [](const std::string &e) { return e; }); };
  RUN_CHECK(int, std::string, "", {7}, "7", op);
  RUN_CHECK(int, std::string, "", {}, "error", op);
}

TEST_CASE(TPE_NAME "_exists", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ exists([](int x) { return x > 5; }); };
  RUN_CHECK(int, bool, "", {7}, true, op);
  RUN_CHECK(int, bool, "", {3}, false, op);
  RUN_CHECK(int, bool, "", {}, false, op);
}

TEST_CASE(TPE_NAME "_forall", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ forall([](int x) { return x > 5; }); };
  RUN_CHECK(int, bool, "", {7}, true, op);
  RUN_CHECK(int, bool, "", {3}, false, op);
  RUN_CHECK(int, bool, "", {}, true, op);
}

TEST_CASE(TPE_NAME "_none_match", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ none_match([](int x) { return x == 7; }); };
  RUN_CHECK(int, bool, "", {7}, false, op);
  RUN_CHECK(int, bool, "", {3}, true, op);
  RUN_CHECK(int, bool, "", {}, true, op);
}

TEST_CASE(TPE_NAME "_count", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ count([](int x) { return x == 7; }); };
  RUN_CHECK(int, size_t, "", {7}, 1u, op);
  RUN_CHECK(int, size_t, "", {3}, 0u, op);
  RUN_CHECK(int, size_t, "", {}, 0u, op);
}

TEST_CASE(TPE_NAME "_contains", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ contains(7); };
  RUN_CHECK(int, bool, "", {7}, true, op);
  RUN_CHECK(int, bool, "", {3}, false, op);
  RUN_CHECK(int, bool, "", {}, false, op);
}

TEST_CASE(TPE_NAME "_tap_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  using PairR = std::pair<int, TPE_CTOR_OUT(int)>;
  auto op = [](auto xs) {
    int seen = 0;
    auto r = xs OP_ tap_each([&](int x) { seen = x; });
    return PairR{seen, r};
  };
  RUN_CHECK(int, PairR, "", {7}, PairR(7, TPE_CTOR_OUT(int){7}), op);
  RUN_CHECK(int, PairR, "", {}, PairR(0, TPE_MAKE_ERR(int)), op);
}

TEST_CASE(TPE_NAME "_tap_error", "[" TPE_NAME "][" TPE_GROUP "]") {
  using PairR = std::pair<std::string, TPE_CTOR_OUT(int)>;
  auto op = [](auto xs) {
    std::string seen;
    auto r = xs OP_ tap_error([&](const std::string &e) { seen = e; });
    return PairR{seen, r};
  };
  RUN_CHECK(int, PairR, "", {7}, PairR(std::string{}, TPE_CTOR_OUT(int){7}), op);
  RUN_CHECK(int, PairR, "", {}, PairR(std::string{"error"}, TPE_MAKE_ERR(int)), op);
}

TEST_CASE(TPE_NAME "_for_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) {
    int seen = -1;
    xs OP_ for_each([&](int x) { seen = x; });
    return seen;
  };
  RUN_CHECK(int, int, "", {7}, 7, op);
  RUN_CHECK(int, int, "", {}, -1, op);
}

TEST_CASE(TPE_NAME "_to_optional", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ to_optional(); };
  RUN_CHECK(int, std::optional<int>, "", {7}, std::optional<int>{7}, op);
  RUN_CHECK(int, std::optional<int>, "", {}, std::optional<int>{}, op);
}

TEST_CASE(TPE_NAME "_to_optional_error", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ to_optional_error(); };
  RUN_CHECK(int, std::optional<std::string>, "", {7}, std::optional<std::string>{}, op);
  RUN_CHECK(int, std::optional<std::string>, "", {}, std::optional<std::string>{"error"}, op);
}

TEST_CASE(TPE_NAME "_to_vector", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ to_vector(); };
  RUN_CHECK(int, std::vector<int>, "", {7}, std::vector<int>{7}, op);
  RUN_CHECK(int, std::vector<int>, "", {}, std::vector<int>{}, op);
}

TEST_CASE(TPE_NAME "_bimap", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ bimap([](int x) { return x + 1; }, [](const std::string &e) { return e + "!"; }); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "value", {7}, TPE_CTOR_OUT(int){8}, op);
  auto err_op = [](auto xs) {
    return xs OP_ bimap([](int x) { return x; }, [](const std::string &e) { return e + "!"; }) OP_ to_optional_error();
  };
  RUN_CHECK(int, std::optional<std::string>, "error", {}, std::optional<std::string>{"error!"}, err_op);
}

TEST_CASE(TPE_NAME "_ensure", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](auto xs) { return xs OP_ ensure([](int x) { return x > 0; }, [](int) { return std::string{"neg"}; }); };
  RUN_CHECK(int, TPE_CTOR_OUT(int), "pass", {5}, TPE_CTOR_OUT(int){5}, op);
  auto fail_op = [](auto xs) {
    return xs OP_ ensure([](int x) { return x > 0; }, [](int) { return std::string{"neg"}; }) OP_ to_optional_error();
  };
  RUN_CHECK(int, std::optional<std::string>, "fail", {-3}, std::optional<std::string>{"neg"}, fail_op);
  RUN_CHECK(int, std::optional<std::string>, "empty", {}, std::optional<std::string>{"error"}, fail_op);
}

TEST_CASE(TPE_NAME "_pipe_basic", "[" TPE_NAME "][" TPE_GROUP "]") {
  const TPE_CTOR_IN(int) ok{7};
  const TPE_CTOR_IN(int) err = TPE_MAKE_ERR(int);

  REQUIRE((ok | map([](int x) { return x + 1; }) | get_or_else(0)) == 8);
  REQUIRE((err | map([](int x) { return x + 1; }) | get_or_else(99)) == 99);
  REQUIRE((ok | get_or_else(0)) == (ok ^ get_or_else(0)));
  REQUIRE((err | get_or_else(99)) == (err ^ get_or_else(99)));
}
