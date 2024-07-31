#include "test_base_includes.hpp"

#include "catch2/catch_test_macros.hpp"

using namespace aspartame;

TEST_CASE("view_iota") {

  CHECK((iota(0) | take(10) | to_vector()) == std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  CHECK((iota(1) | take(1) | to_vector()) == std::vector{1});
  CHECK((iota(-1) | take(1) | to_vector()) == std::vector{-1});
  CHECK((iota(-5) | take(5) | to_vector()) == std::vector{-5, -4, -3, -2, -1});
  CHECK((iota(-1) | take(0) | to_vector()) == std::vector<int>{});

  CHECK((iota(0, 10) | to_vector()) == std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  CHECK((iota(1, 1) | to_vector()) == std::vector{1});
  CHECK((iota(-1, 1) | to_vector()) == std::vector{-1});
  CHECK((iota(-5, 5) | to_vector()) == std::vector{-5, -4, -3, -2, -1});
  CHECK((iota(-1, 0) | to_vector()) == std::vector<int>{});
}

TEST_CASE("view_exclusive") {

  CHECK((exclusive(0, 1, 10) | to_vector()) == std::vector{0});
  CHECK((exclusive(1, 1, 1) | to_vector()) == std::vector<int>{});
  CHECK((exclusive(-1, 1, 1) | to_vector()) == std::vector{-1, 0});
  CHECK((exclusive(-5, 1, 5) | to_vector()) == std::vector{-5, 0});
  CHECK_THROWS((exclusive(-1, 1, 0) | to_vector()));

  CHECK((exclusive(0, 2, 10) | to_vector()) == std::vector{0});
  CHECK((exclusive(1, 2, 1) | to_vector()) == std::vector{1});
  CHECK((exclusive(-1, 2, 1) | to_vector()) == std::vector{-1, 0, 1});
  CHECK((exclusive(-5, 2, 5) | to_vector()) == std::vector{-5, 0});
  CHECK_THROWS((exclusive(-1, 2, 0) | to_vector()));
}

TEST_CASE("view_inclusive") {
  CHECK((inclusive(0, 2, 10) | to_vector()) == std::vector{0});
  CHECK((inclusive(1, 2, 1) | to_vector()) == std::vector{1, 2});
  CHECK((inclusive(-1, 2, 1) | to_vector()) == std::vector{-1, 0, 1, 2});
  CHECK((inclusive(-5, 2, 5) | to_vector()) == std::vector{-5, 0});
  CHECK_THROWS((inclusive(-1, 2, 0) | to_vector()));
}
