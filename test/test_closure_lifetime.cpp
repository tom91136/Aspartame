// Pins the closure-lifetime contract: value-arg factories OWN their state by
// value (storable across stack frames); container-arg factories BORROW by
// const reference (caller keeps the source alive).

#include <functional>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "aspartame/all.hpp"

using namespace aspartame;

TEST_CASE("fold_left closure stored with rvalue init", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3, 4};

  auto sum = fold_left(0, [](int a, int b) { return a + b; });
  auto cat = fold_left(std::string{"["}, [](std::string a, int b) { return a + std::to_string(b); });

  REQUIRE((xs ^ sum) == 10);
  REQUIRE((xs ^ cat) == "[1234");
}

TEST_CASE("fold_right closure stored with rvalue init", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3};
  auto op = fold_right(100, [](int x, int a) { return a - x; });

  REQUIRE((xs ^ op) == 94);
}

TEST_CASE("map closure stored with rvalue lambda", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3};
  auto times_ten = map([](int x) { return x * 10; });
  REQUIRE((xs ^ times_ten) == std::vector<int>{10, 20, 30});
}

TEST_CASE("filter closure stored with rvalue predicate", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3, 4, 5};
  auto even = filter([](int x) { return x % 2 == 0; });
  REQUIRE((xs ^ even) == std::vector<int>{2, 4});
}

TEST_CASE("take/drop closure stored with rvalue n", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3, 4, 5};
  auto t = take(2);
  auto d = drop(2);
  REQUIRE((xs ^ t) == std::vector<int>{1, 2});
  REQUIRE((xs ^ d) == std::vector<int>{3, 4, 5});
}

TEST_CASE("at_maybe closure stored with rvalue idx", "[closure-lifetime][own]") {
  std::vector<int> xs{10, 20, 30};
  auto get_first = at_maybe(0);
  auto get_last = at_maybe(2);
  REQUIRE((xs ^ get_first) == std::optional<int>{10});
  REQUIRE((xs ^ get_last) == std::optional<int>{30});
}

TEST_CASE("slice closure stored with rvalue bounds", "[closure-lifetime][own]") {
  std::vector<int> xs{0, 1, 2, 3, 4, 5};
  auto mid = slice(1, 4);
  REQUIRE((xs ^ mid) == std::vector<int>{1, 2, 3});
}

TEST_CASE("sort cmp closure stored with rvalue lambda", "[closure-lifetime][own]") {
  std::vector<int> xs{3, 1, 4, 1, 5, 9, 2, 6};
  auto desc = sort([](int a, int b) { return a > b; });
  REQUIRE((xs ^ desc) == std::vector<int>{9, 6, 5, 4, 3, 2, 1, 1});
}

TEST_CASE("sort_by closure stored", "[closure-lifetime][own]") {
  std::vector<std::string> xs{"foo", "a", "longest", "bb"};
  auto by_len = sort_by([](const std::string &s) { return s.size(); });
  REQUIRE((xs ^ by_len) == std::vector<std::string>{"a", "bb", "foo", "longest"});
}

TEST_CASE("take_while/drop_while closures stored", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3, 4, 1, 2};
  auto under_three = take_while([](int x) { return x < 3; });
  auto over_three = drop_while([](int x) { return x < 3; });
  REQUIRE((xs ^ under_three) == std::vector<int>{1, 2});
  REQUIRE((xs ^ over_three) == std::vector<int>{3, 4, 1, 2});
}

TEST_CASE("mk_string closure stored with rvalue strings", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3};
  auto fmt = mk_string("[", ",", "]");
  auto seq = mk_string(",");
  REQUIRE((xs ^ fmt) == "[1,2,3]");
  REQUIRE((xs ^ seq) == "1,2,3");
}

TEST_CASE("distinct_by / group_by closures stored", "[closure-lifetime][own]") {
  std::vector<std::pair<int, std::string>> xs{{1, "a"}, {2, "b"}, {1, "c"}, {3, "d"}};
  auto by_first = distinct_by([](const std::pair<int, std::string> &p) { return p.first; });
  auto grp = group_by([](const std::pair<int, std::string> &p) { return p.first; });

  auto unique = xs ^ by_first;
  REQUIRE(unique.size() == 3);

  auto groups = xs ^ grp;
  REQUIRE(groups.size() == 3);
  REQUIRE(groups[1].size() == 2);
}

TEST_CASE("variant fold_total/fold_partial closures stored", "[closure-lifetime][own]") {
  std::variant<int, std::string> v1{42};
  std::variant<int, std::string> v2{std::string{"hi"}};

  auto show = fold_total([](int x) { return std::to_string(x); }, //
                         [](const std::string &s) { return s; });

  REQUIRE((v1 ^ show) == "42");
  REQUIRE((v2 ^ show) == "hi");
}

TEST_CASE("optional fold/or_else closures stored", "[closure-lifetime][own]") {
  std::optional<int> some{7};
  std::optional<int> none{};

  auto default_zero = get_or_else(0);
  auto stringify = fold([](int x) { return std::to_string(x); }, []() { return std::string{"-"}; });

  REQUIRE((some ^ default_zero) == 7);
  REQUIRE((none ^ default_zero) == 0);
  REQUIRE((some ^ stringify) == "7");
  REQUIRE((none ^ stringify) == "-");
}

namespace {
auto make_adder(int seed) {
  return fold_left(seed, [](int a, int b) { return a + b; });
}

auto make_scaler(int factor) {
  return map([factor](int x) { return x * factor; });
}

auto make_even_filter() {
  return filter([](int x) { return x % 2 == 0; });
}
} // namespace

TEST_CASE("closures returned from a function survive the call site", "[closure-lifetime][own]") {
  std::vector<int> xs{1, 2, 3, 4};

  auto add_10 = make_adder(10);
  auto times_3 = make_scaler(3);
  auto only_evens = make_even_filter();

  REQUIRE((xs ^ add_10) == 20);
  REQUIRE((xs ^ times_3) == std::vector<int>{3, 6, 9, 12});
  REQUIRE((xs ^ only_evens) == std::vector<int>{2, 4});
}

TEST_CASE("stored closures can be invoked many times", "[closure-lifetime][own]") {
  auto sum = fold_left(0, [](int a, int b) { return a + b; });

  REQUIRE((std::vector<int>{1, 2, 3} ^ sum) == 6);
  REQUIRE((std::vector<int>{10, 20} ^ sum) == 30);
  REQUIRE((std::vector<int>{} ^ sum) == 0);
}

TEST_CASE("stored closures work after intervening allocations stomp the stack", "[closure-lifetime][own]") {
  // Dangling-ref to factory's frame would be exposed by overwriting that slot.
  auto sum = fold_left(0, [](int a, int b) { return a + b; });

  {
    volatile char scratch[4096];
    for (size_t i = 0; i < sizeof(scratch); ++i)
      scratch[i] = static_cast<char>(i);
    (void)scratch;
  }

  std::vector<int> xs{1, 2, 3, 4};
  REQUIRE((xs ^ sum) == 10);
}

TEST_CASE("concat closure borrows the container", "[closure-lifetime][borrow]") {
  std::vector<int> xs{1, 2};
  std::vector<int> ys{3, 4};

  auto op = concat(ys);
  REQUIRE((xs ^ op) == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("concat closure borrows the container (in-place ^=)", "[closure-lifetime][borrow]") {
  std::vector<int> xs{1, 2};
  std::vector<int> ys{3, 4};

  auto op = concat(ys);
  xs ^= op;
  REQUIRE(xs == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("zip closure borrows the container", "[closure-lifetime][borrow]") {
  std::vector<int> xs{1, 2, 3};
  std::vector<std::string> ys{"a", "b", "c"};

  auto op = zip(ys);
  auto out = xs ^ op;
  REQUIRE(out.size() == 3);
  REQUIRE(out[1].first == 2);
  REQUIRE(out[1].second == "b");
}

TEST_CASE("index_of_slice / contains_slice closures borrow the container", "[closure-lifetime][borrow]") {
  std::vector<int> xs{1, 2, 3, 4, 5};
  std::vector<int> needle{3, 4};

  auto idx = index_of_slice(needle);
  auto has = contains_slice(needle);
  REQUIRE((xs ^ idx) == 2);
  REQUIRE((xs ^ has) == true);
}
