#include "test_base_includes.hpp"

#include "catch2/catch_test_macros.hpp"
#include "fixtures.hpp"

using namespace aspartame;

TEST_CASE("std::variant_get_maybe") {
  std::variant<Foo, int, string> v{1};
  CHECK((v ^ get_maybe<int>()) == 1);
  CHECK((v ^ get_maybe<Foo>()) == std::nullopt);
  CHECK((v ^ get_maybe<string>()) == std::nullopt);
  v = "aaa";
  CHECK((v ^ get_maybe<int>()) == std::nullopt);
  CHECK((v ^ get_maybe<Foo>()) == std::nullopt);
  CHECK((v ^ get_maybe<string>()) == "aaa");
  v = Foo{42};
  CHECK((v ^ get_maybe<int>()) == std::nullopt);
  CHECK((v ^ get_maybe<string>()) == std::nullopt);
  CHECK((v ^ get_maybe<Foo>()) == Foo{42});
}

TEST_CASE("std::variant_holds") {
  std::variant<Foo, int, string> v{1};
  CHECK((v ^ holds<int>()) == true);
  CHECK((v ^ holds<Foo>()) == false);
  CHECK((v ^ holds<string>()) == false);
  v = "aaa";
  CHECK((v ^ holds<string>()) == true);
  CHECK((v ^ holds<int>()) == false);
  CHECK((v ^ holds<Foo>()) == false);
  v = Foo{42};
  CHECK((v ^ holds<Foo>()) == true);
  CHECK((v ^ holds<int>()) == false);
  CHECK((v ^ holds<string>()) == false);
}

TEST_CASE("std::variant_holds_any") {
  std::variant<Foo, int, string> v{1};
  CHECK((v ^ holds_any<int>()) == true);
  CHECK((v ^ holds_any<Foo>()) == false);
  CHECK((v ^ holds_any<string>()) == false);
  CHECK((v ^ holds_any<string, Foo>()) == false);
  CHECK((v ^ holds_any<string, int, Foo>()) == true);
  CHECK((v ^ holds_any<>()) == false);
  v = "aaa";
  CHECK((v ^ holds_any<string>()) == true);
  CHECK((v ^ holds_any<int>()) == false);
  CHECK((v ^ holds_any<Foo>()) == false);
  CHECK((v ^ holds_any<int, Foo>()) == false);
  CHECK((v ^ holds_any<string, int, Foo>()) == true);
  CHECK((v ^ holds_any<>()) == false);
  v = Foo{42};
  CHECK((v ^ holds_any<string>()) == false);
  CHECK((v ^ holds_any<int>()) == false);
  CHECK((v ^ holds_any<Foo>()) == true);
  CHECK((v ^ holds_any<string, int>()) == false);
  CHECK((v ^ holds_any<string, int, Foo>()) == true);
  CHECK((v ^ holds_any<>()) == false);
}

TEST_CASE("std::variant_narrow") {
  std::variant<Foo, int, string> v{1};
  CHECK((v ^ narrow<Foo, int, string>()) == std::optional<std::variant<Foo, int, string>>{1});
  CHECK((v ^ narrow<Foo, int>()) == std::optional<std::variant<Foo, int>>{{1}});
  CHECK((v ^ narrow<int, Foo>()) == std::optional<std::variant<int, Foo>>{{1}});
  CHECK((v ^ narrow<Foo>()) == std::optional<std::variant<Foo>>{});
  v = "aaa";
  CHECK((v ^ narrow<Foo, int, string>()) == std::optional<std::variant<Foo, int, string>>{"aaa"});
  CHECK((v ^ narrow<Foo, int>()) == std::optional<std::variant<Foo, int>>{});
  CHECK((v ^ narrow<int, Foo>()) == std::optional<std::variant<int, Foo>>{});
  CHECK((v ^ narrow<Foo>()) == std::optional<std::variant<Foo>>{});
  v = Foo{42};
  CHECK((v ^ narrow<Foo, int, string>()) == std::optional<std::variant<Foo, int, string>>{Foo{42}});
  CHECK((v ^ narrow<Foo, int>()) == std::optional<std::variant<Foo, int>>{Foo{42}});
  CHECK((v ^ narrow<int, Foo>()) == std::optional<std::variant<int, Foo>>{Foo{42}});
  CHECK((v ^ narrow<Foo>()) == std::optional<std::variant<Foo>>{Foo{42}});
}

TEST_CASE("std::variant_fold_partial_return_unique_ptr") {
  std::variant<std::unique_ptr<Foo>, int> v{1};
  CHECK(((v ^ fold_partial([](int x) { return std::make_unique<string>(std::to_string(x)); }) ^ map([](const auto &x) { return *x; })) == //
         "1"));
  CHECK(((v ^ fold_partial([](const std::unique_ptr<Foo> &x) { return std::make_unique<string>(std::to_string(x->value)); })) == //
         std::nullopt));

  v = std::make_unique<Foo>(42);
  CHECK(((v ^ fold_partial([](int x) { return std::make_unique<string>(std::to_string(x)); })) == std::nullopt));
  CHECK(((v ^ fold_partial([](const std::unique_ptr<Foo> &x) { return std::make_unique<string>(std::to_string(x->value)); }) ^
          map([](const auto &x) { return *x; })) == "42"));
}

TEST_CASE("std::variant_fold_total_unique_ptr") {
  std::variant<std::unique_ptr<Foo>, int> v{1};
  CHECK((v ^ fold_total([](const std::unique_ptr<Foo> &x) { return x->value; }, [](int x) { return x; })) == 1);
  v = std::make_unique<Foo>(42);
  CHECK((v ^ fold_total([](const std::unique_ptr<Foo> &x) { return x->value; }, [](int x) { return x; })) == 42);
}

TEST_CASE("std::variant_fold_total") {
  std::variant<Foo, int, string> v{1};
  CHECK((v ^ fold_total([](Foo x) { return x.value; }, [](int x) { return x; }, [](const string &s) { return int(s.length()); })) == 1);
  v = "aaa";
  CHECK((v ^ fold_total([](Foo x) { return x.value; }, [](int x) { return x; }, [](const string &s) { return int(s.length()); })) == 3);
  v = Foo{42};
  CHECK((v ^ fold_total([](Foo x) { return x.value; }, [](int x) { return x; }, [](const string &s) { return int(s.length()); })) == 42);
}

TEST_CASE("std::variant_fold_partial") {
  std::variant<Foo, int, string> v{1};
  CHECK((v ^ fold_partial([](Foo x) { return x.value; }, [](int x) { return x; },
                          [](const string &s) { return static_cast<int>(s.length()); })) == 1);
  v = "aaa";
  CHECK((v ^ fold_partial([](Foo x) { return x.value; }, [](int x) { return x; },
                          [](const string &s) { return static_cast<int>(s.length()); })) == 3);
  v = Foo{42};
  CHECK((v ^ fold_partial([](Foo x) { return x.value; }, [](int x) { return x; },
                          [](const string &s) { return static_cast<int>(s.length()); })) == 42);

  v = 1;
  CHECK((v ^ fold_partial([](Foo x) { return x.value; })) == std::nullopt);
  v = "aaa";
  CHECK((v ^ fold_partial([](Foo x) { return x.value; })) == std::nullopt);
  v = Foo{42};
  CHECK((v ^ fold_partial([](Foo x) { return x.value; })) == 42);
}

TEST_CASE("std::variant_foreach_total") {
  std::variant<Foo, int, string> v{1};
  v ^ foreach_total([](Foo) { FAIL(); }, [](int x) { CHECK(x == 1); }, [](const string &) { FAIL(); });
  v = "aaa";
  v ^ foreach_total([](Foo) { FAIL(); }, [](int) { FAIL(); }, [](const string &s) { CHECK(s == "aaa"); });
  v = Foo{42};
  v ^ foreach_total([](Foo x) { CHECK(x == Foo{42}); }, [](int) { FAIL(); }, [](const string &) { FAIL(); });
}

TEST_CASE("std::variant_foreach_partial") {
  std::variant<Foo, int, string> v{1};
  v ^ foreach_partial([](Foo) { FAIL(); }, [](int x) { CHECK(x == 1); }, [](const string &) { FAIL(); });
  v = "aaa";
  v ^ foreach_partial([](Foo) { FAIL(); }, [](int) { FAIL(); }, [](const string &s) { CHECK(s == "aaa"); });
  v = Foo{42};
  v ^ foreach_partial([](Foo x) { CHECK(x == Foo{42}); }, [](int) { FAIL(); }, [](const string &) { FAIL(); });

  v = 1;
  v ^ foreach_partial([](Foo) { FAIL(); });
  v = "aaa";
  v ^ foreach_partial([](Foo) { FAIL(); });
  v = Foo{42};
  v ^ foreach_partial([](Foo x) { CHECK(x == Foo{42}); });
}
