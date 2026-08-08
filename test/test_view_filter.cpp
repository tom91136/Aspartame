#define OP_ | // NOLINT(*-macro-parentheses)
#define TPE_GROUP "test_view+filter"
#define TPE_NAME "view+filter"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_MANY_INIT 1
#define TPE_RUN_TEST

#define TPE_TEST_CHAIN(xs) ((xs) | ::aspartame::filter([](auto) { return true; }))

#include "catch2/catch_test_macros.hpp"

#include "templates/test_template_view.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("view_filter_preserves_mutable_references", "[view][filter]") {
  std::vector<int> xs{1, 2, 3, 4};
  xs | filter([](int x) { return x % 2 == 0; }) | drop(1) | take(1) | tap_each([](int &x) { x *= 10; }) | for_each([](int &x) { ++x; });
  CHECK(xs == std::vector{1, 2, 3, 41});
}
