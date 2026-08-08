#include <memory>

#include "catch2/catch_test_macros.hpp"

#include "test_base_includes.hpp"

using namespace aspartame;

TEST_CASE("view_shared") {
  std::vector<int> ls, rs, all;
  auto [l, r] = std::vector{1, 2, 3, 4, 5, 6} | split_at(3);
  l | tap_each([&](auto x) { ls.push_back(x); }) | for_each([&](auto x) { all.push_back(x); });
  r | tap_each([&](auto x) { rs.push_back(x); }) | for_each([&](auto x) { all.push_back(x); });

  CHECK((l | to_vector()) == std::vector{1, 2, 3});
  CHECK((r | to_vector()) == std::vector{4, 5, 6});
  CHECK((all | to_vector()) == std::vector{1, 2, 3, 4, 5, 6});
}

TEST_CASE("view_shared-non-owning") {
  std::vector source{1, 2, 3, 4, 5, 6};
  std::vector<int> ls, rs, all;
  auto [l, r] = source | split_at(3);
  l | tap_each([&](auto x) { ls.push_back(x); }) | for_each([&](auto x) { all.push_back(x); });
  r | tap_each([&](auto x) { rs.push_back(x); }) | for_each([&](auto x) { all.push_back(x); });

  CHECK((l | to_vector()) == std::vector{1, 2, 3});
  CHECK((r | to_vector()) == std::vector{4, 5, 6});
  CHECK((all | to_vector()) == std::vector{1, 2, 3, 4, 5, 6});
}

TEST_CASE("view_owning") {
  std::vector<int> ys, all;
  std::vector{1, 2, 3, 4, 5, 6}                    //
      | filter([](auto x) { return x > 3; })       //
      | tap_each([&](auto x) { ys.push_back(x); }) //
      | for_each([&](auto x) { all.push_back(x); });

  CHECK((all | to_vector()) == std::vector{4, 5, 6});
  CHECK((ys | to_vector()) == std::vector{4, 5, 6});
}

TEST_CASE("view_non-owning") {
  std::vector source{1, 2, 3, 4, 5, 6};
  std::vector<int> ys, all;
  source                                           //
      | filter([](auto x) { return x > 3; })       //
      | tap_each([&](auto x) { ys.push_back(x); }) //
      | for_each([&](auto x) { all.push_back(x); });

  CHECK((all | to_vector()) == std::vector{4, 5, 6});
  CHECK((ys | to_vector()) == std::vector{4, 5, 6});
}

TEST_CASE("view_owning-copy") {
  auto xs = std::vector{1, 2, 3, 4, 5, 6}          //
            | filter([](auto x) { return x > 3; }) //
            | tap_each([](auto) {});
  auto zs = xs | tap_each([](auto) {});
  CHECK((zs | to_vector()) == std::vector{4, 5, 6});
  CHECK((xs | to_vector()) == std::vector{4, 5, 6});
}

TEST_CASE("view_non-owning-copy") {
  std::vector source{1, 2, 3, 4, 5, 6};
  auto xs = source                                 //
            | filter([](auto x) { return x > 3; }) //
            | tap_each([](auto) {});
  auto zs = xs | tap_each([](auto) {});
  CHECK((zs | to_vector()) == std::vector{4, 5, 6});
  CHECK((xs | to_vector()) == std::vector{4, 5, 6});
}

TEST_CASE("view_owning-observe") {
  auto ys = std::vector{1, 2, 3, 4, 5, 6}       //
            | map([](auto x) { return x * 2; }) //
            | and_then([](auto &x) { return std::list<int>{x.begin(), x.end()}; });
  CHECK(ys == std::list{2, 4, 6, 8, 10, 12});
}

TEST_CASE("view_non-owning-observe") {
  std::vector source{1, 2, 3, 4, 5, 6};
  auto ys = source                              //
            | map([](auto x) { return x * 2; }) //
            | and_then([](auto &x) { return std::list<int>{x.begin(), x.end()}; });
  CHECK(ys == std::list{2, 4, 6, 8, 10, 12});
}

TEST_CASE("view_concat_owns_temporary_rhs", "[view][concat][ownership]") {
  std::vector<int> xs{1, 2};
  auto result = xs | concat(std::vector<int>{3, 4});
  CHECK((result | to_vector()) == std::vector{1, 2, 3, 4});

  std::vector<std::unique_ptr<int>> move_only;
  move_only.emplace_back(std::make_unique<int>(1));
  auto make_rhs = [] {
    std::vector<std::unique_ptr<int>> rhs;
    rhs.emplace_back(std::make_unique<int>(2));
    return rhs;
  };
  auto move_only_result = move_only | concat(make_rhs());
  CHECK((move_only_result | map([](const auto &x) { return *x; }) | to_vector()) == std::vector{1, 2});
}

TEST_CASE("view_optional_rhs_is_zero_or_one_element", "[view][optional][concat][zip][cross]") {
  std::vector<int> xs{1, 2};
  std::optional<int> some{3};
  std::optional<int> none;

  CHECK((xs | concat(some) | to_vector()) == std::vector{1, 2, 3});
  CHECK((xs | concat(none) | to_vector()) == xs);
  CHECK((xs | zip(some) | to_vector()) == std::vector{std::pair{1, 3}});
  CHECK((xs | zip(none) | to_vector()) == std::vector<std::pair<int, int>>{});
  CHECK((xs | cross(some) | to_vector()) == std::vector{std::pair{1, 3}, std::pair{2, 3}});
  CHECK((xs | cross(none) | to_vector()) == std::vector<std::pair<int, int>>{});

  auto concatenated = xs | concat(std::optional<int>{4});
  auto zipped = xs | zip(std::optional<int>{4});
  auto crossed = xs | cross(std::optional<int>{4});
  CHECK((concatenated | to_vector()) == std::vector{1, 2, 4});
  CHECK((zipped | to_vector()) == std::vector{std::pair{1, 4}});
  CHECK((crossed | to_vector()) == std::vector{std::pair{1, 4}, std::pair{2, 4}});

  xs | zip(some) | for_each([](int &, int &rhs) { rhs = 5; });
  CHECK(some == 5);
  xs | cross(some) | take(1) | for_each([](int &, int &rhs) { rhs = 6; });
  CHECK(some == 6);
  xs | concat(some) | drop(xs.size()) | for_each([](int &rhs) { rhs = 7; });
  CHECK(some == 7);
}

TEST_CASE("view_cross_owns_temporary_rhs", "[view][cross][ownership]") {
  std::vector<int> xs{1, 2};
  auto result = xs | cross(std::vector<int>{3, 4});
  CHECK((result | to_vector()) == std::vector{std::pair{1, 3}, std::pair{1, 4}, std::pair{2, 3}, std::pair{2, 4}});
}

TEST_CASE("view_zip_preserves_references", "[view][zip][ownership]") {
  std::vector<std::unique_ptr<int>> xs;
  xs.emplace_back(std::make_unique<int>(1));
  xs.emplace_back(std::make_unique<int>(2));
  std::vector<int> ys{10, 20};

  xs | zip(ys) | for_each([](std::unique_ptr<int> &x, int &y) {
    *x += y;
    y = 0;
  });
  CHECK(*xs[0] == 11);
  CHECK(*xs[1] == 22);
  CHECK(ys == std::vector{0, 0});

  auto temporary = xs | zip(std::vector<int>{1, 2});
  CHECK((temporary | map([](const auto &x, int y) { return *x + y; }) | to_vector()) == std::vector{12, 24});

  auto make_rhs = [] {
    std::vector<std::unique_ptr<int>> rhs;
    rhs.emplace_back(std::make_unique<int>(3));
    rhs.emplace_back(std::make_unique<int>(4));
    return rhs;
  };
  auto move_only_temporary = ys | zip(make_rhs());
  CHECK((move_only_temporary | map([](int x, const auto &y) { return x + *y; }) | to_vector()) == std::vector{3, 4});
}

TEST_CASE("materialized_sliding_windows_retain_temporary_source", "[view][sliding][ownership]") {
  auto windows = std::vector<int>{1, 2, 3} | sliding(2, 1) | to_vector();
  using Window = typename decltype(windows)::value_type;
  STATIC_REQUIRE(details::is_sharing<std::decay_t<decltype(std::declval<Window>().storage)>>);
  REQUIRE(windows.size() == 2);
  CHECK(windows[0][0] == 1);
  CHECK(windows[0][1] == 2);
  CHECK(windows[1][0] == 2);
  CHECK(windows[1][1] == 3);
}
