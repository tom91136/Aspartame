#define OP_ | // NOLINT(*-macro-parentheses)
#define TPE_GROUP "test_view"
#define TPE_NAME "view"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_MANY_INIT 1
#define TPE_RUN_TEST

#define TPE_TEST_CHAIN(xs) (xs)

#include "catch2/catch_test_macros.hpp"

#include "templates/test_template_view.hpp"
#include "test_base_includes.hpp"

#include <list>

using namespace aspartame;

TEST_CASE("view_sliding_random_access_windows", "[view][sliding]") {
  std::vector<int> xs{1, 2, 3, 4};
  auto windows = xs | sliding(3, 2);
  std::vector<int> first_elements;
  windows | for_each([&](const auto &window) {
    CHECK(window.size() <= 3);
    first_elements.push_back(window[0]);
  });
  CHECK(first_elements == std::vector{1, 3});

  auto first_window = *(xs | sliding(2, 1)).begin();
  first_window[1] = 20;
  CHECK(xs == std::vector{1, 20, 3, 4});

  std::vector<std::vector<int>> oversized;
  xs | sliding(10, 2) | for_each([&](const auto &window) { oversized.emplace_back(window.begin(), window.end()); });
  CHECK(oversized == std::vector<std::vector<int>>{{1, 20, 3, 4}});
}

TEST_CASE("view_sliding_buffered_windows", "[view][sliding]") {
  const std::list<int> xs{1, 2, 3, 4};
  std::vector<std::vector<int>> actual;
  xs | sliding(3, 2) | for_each([&](const auto &window) {
    REQUIRE(window.size() >= 2);
    CHECK(window[0] == static_cast<int>(actual.size() * 2 + 1));
    actual.emplace_back(window.begin(), window.end());
  });
  CHECK(actual == std::vector<std::vector<int>>{{1, 2, 3}, {3, 4}});
}
