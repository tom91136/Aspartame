#include "catch2/catch_test_macros.hpp"

#include "fixtures.hpp"
#include "test_base_includes.hpp"

#if __has_include(<version>)
  #include <version>
#endif

#define OP_ ^
#define TPE_GROUP "test_vector"
#define TPE_NAME "std::vector"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_sequence.hpp"

#define TPE_INPLACE_SEQ 1
#include "templates/test_template_inplace.hpp"

#if defined(__cpp_lib_expected)
TEST_CASE("vector_partition_map_splits_oks_and_errs", "[std::vector][partition_map]") {
  using namespace aspartame;
  std::vector<int> xs{1, 2, 3, 4, 5};
  auto [errs, oks] = xs ^ partition_map([](int x) -> std::expected<int, std::string> {
                       if (x % 2 == 0) return x * 10;
                       return std::unexpected(std::to_string(x));
                     });
  CHECK(oks == std::vector<int>{20, 40});
  CHECK(errs == std::vector<std::string>{"1", "3", "5"});
}
#endif

TEST_CASE("vector_combine_all_nested_flattens", "[std::vector][combine_all]") {
  using namespace aspartame;
  std::vector<std::vector<int>> xs{{1, 2}, {3, 4}, {5}};
  CHECK((xs ^ combine_all()) == std::vector<int>{1, 2, 3, 4, 5});
}

TEST_CASE("vector_collect_to_supports_move_only_and_map_outputs", "[std::vector][collect_to]") {
  using namespace aspartame;
  const std::vector<int> xs{1, 2, 3, 4};
  const auto lookup = xs ^ collect_to<std::map>([](int x) -> std::optional<std::pair<int, std::string>> {
                        if (x > 2) return std::pair{x, std::to_string(x)};
                        return std::nullopt;
                      });
  CHECK(lookup == std::map<int, std::string>{{3, "3"}, {4, "4"}});

  auto pointers = xs ^ collect_to<std::vector>([](int x) -> std::optional<std::unique_ptr<int>> {
                    if (x == 2) return std::make_unique<int>(x);
                    return std::nullopt;
                  });
  REQUIRE(pointers.size() == 1);
  CHECK(*pointers.front() == 2);
}

TEST_CASE("vector_move_to", "[std::vector][move_to]") {
  std::vector<std::unique_ptr<int>> input;
  input.emplace_back(std::make_unique<int>(1));
  input.emplace_back(std::make_unique<int>(2));
  std::vector<std::unique_ptr<int>> output;

  input | move_to(output);

  CHECK(input[0] == nullptr);
  CHECK(input[1] == nullptr);
  REQUIRE(output.size() == 2);
  CHECK(*output[0] == 1);
  CHECK(*output[1] == 2);
}

TEST_CASE("vector_append_and_move_to_reject_self_alias", "[std::vector][append_to][move_to]") {
  std::vector<std::string> xs{"a", "b"};
  CHECK_THROWS_AS(xs | append_to(xs), std::logic_error);
  CHECK_THROWS_AS(xs | move_to(xs), std::logic_error);
}
