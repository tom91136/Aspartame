#include "catch2/catch_test_macros.hpp"

#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("view_iota") {

  CHECK((iota(0) | take(10) | to_vector()) == std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  CHECK((iota(1) | take(1) | to_vector()) == std::vector{1});
  CHECK((iota(-1) | take(1) | to_vector()) == std::vector{-1});
  CHECK((iota(-5) | take(5) | to_vector()) == std::vector{-5, -4, -3, -2, -1});
  CHECK((iota(-1) | take(0) | to_vector()) == std::vector<int>{});

  CHECK((iota(0, 10) | to_vector()) == std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  CHECK((iota(1, 1) | to_vector()) == std::vector<int>{});
  CHECK((iota(-1, 1) | to_vector()) == std::vector{-1, 0});
  CHECK((iota(-5, 5) | to_vector()) == std::vector{-5, -4, -3, -2, -1, 0, 1, 2, 3, 4});
  CHECK((iota(-1, 0) | to_vector()) == std::vector{-1});
  CHECK((iota(5, 0) | to_vector()) == std::vector<int>{});
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

TEST_CASE("view_repeat") {
  CHECK((repeat(7) | take(0) | to_vector()) == std::vector<int>{});
  CHECK((repeat(7) | take(1) | to_vector()) == std::vector{7});
  CHECK((repeat(7) | take(4) | to_vector()) == std::vector{7, 7, 7, 7});
  CHECK((repeat(std::string{"x"}) | take(3) | to_vector()) == std::vector<std::string>{"x", "x", "x"});
}

TEST_CASE("view_reverse_lazy") {
  std::vector<int> xs{1, 2, 3, 4, 5};
  CHECK((view(xs.begin(), xs.end()) | reverse() | to_vector()) == std::vector{5, 4, 3, 2, 1});
  CHECK((view(xs.begin(), xs.end()) | reverse() | take(3) | to_vector()) == std::vector{5, 4, 3});
  std::vector<int> empty;
  CHECK((view(empty.begin(), empty.end()) | reverse() | to_vector()) == std::vector<int>{});
}

TEST_CASE("view_fold_right_on_bidirectional") {
  std::vector<int> xs{1, 2, 3, 4, 5};
  // fold_right f 0 [1,2,3,4,5] = f(1, f(2, f(3, f(4, f(5, 0)))))
  // with f(x, acc) = acc*10 + x: f(5,0)=5, f(4,5)=54, f(3,54)=543, f(2,543)=5432, f(1,5432)=54321.
  CHECK((view(xs.begin(), xs.end()) | fold_right(0, [](int x, int acc) { return acc * 10 + x; })) == 54321);
  std::vector<int> empty;
  CHECK((view(empty.begin(), empty.end()) | fold_right(42, [](int x, int acc) { return acc + x; })) == 42);
}

TEST_CASE("view_scan_left_on_any_view") {
  std::vector<int> xs{1, 2, 3, 4};
  CHECK((view(xs.begin(), xs.end()) | scan_left(0, [](int a, int b) { return a + b; })) == std::vector{0, 1, 3, 6, 10});
}

TEST_CASE("view_scan_right_on_bidirectional") {
  std::vector<int> xs{1, 2, 3, 4};
  CHECK((view(xs.begin(), xs.end()) | scan_right(0, [](int x, int acc) { return acc + x; })) == std::vector{10, 9, 7, 4, 0});
}
