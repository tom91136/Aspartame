#include "test_base_includes.hpp"

#include "catch2/catch_test_macros.hpp"

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
            | and_then([](auto &x) {
                return std::list<int>{x.begin(), x.end()};
              });
  CHECK(ys == std::list{2, 4, 6, 8, 10, 12});
};

TEST_CASE("view_non-owning-observe") {
  std::vector source{1, 2, 3, 4, 5, 6};
  auto ys = source                              //
            | map([](auto x) { return x * 2; }) //
            | and_then([](auto &x) {
                return std::list<int>{x.begin(), x.end()};
              });
  CHECK(ys == std::list{2, 4, 6, 8, 10, 12});
};