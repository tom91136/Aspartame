#include "test_base_includes.hpp"

#include "catch2/catch_test_macros.hpp"
#include "fixtures.hpp"

using namespace aspartame;

TEST_CASE("std::variant_get") {
  std::variant<Foo, int, std::string> v{1};
  CHECK((v ^ get<int>()) == 1);
  CHECK((v ^ get<Foo>()) == std::nullopt);
  CHECK((v ^ get<std::string>()) == std::nullopt);
  v = "aaa";
  CHECK((v ^ get<int>()) == std::nullopt);
  CHECK((v ^ get<Foo>()) == std::nullopt);
  CHECK((v ^ get<std::string>()) == "aaa");
  v = Foo{42};
  CHECK((v ^ get<int>()) == std::nullopt);
  CHECK((v ^ get<std::string>()) == std::nullopt);
  CHECK((v ^ get<Foo>()) == Foo{42});
}

TEST_CASE("std::variant_fold_total_return_unique_ptr") {
  std::variant<std::unique_ptr<Foo>, int> v{1};
  CHECK(*(v ^ fold_total([](const std::unique_ptr<Foo> &x) { return std::make_unique<std::string>(std::to_string(x->value)); },
                         [](int x) { return std::make_unique<std::string>(std::to_string(x)); })) == "1");
  v = std::make_unique<Foo>(42);
  CHECK(*(v ^ fold_total([](const std::unique_ptr<Foo> &x) { return std::make_unique<std::string>(std::to_string(x->value)); },
                         [](int x) { return std::make_unique<std::string>(std::to_string(x)); })) == "42");
}

TEST_CASE("std::variant_fold_partial_return_unique_ptr") {
  std::variant<std::unique_ptr<Foo>, int> v{1};
  CHECK((v ^ fold_partial([](int x) { return std::make_unique<std::string>(std::to_string(x)); }) ^
         map([](const auto &x) { return *x; })) == //
        "1");
  CHECK((v ^ fold_partial([](const std::unique_ptr<Foo> &x) { return std::make_unique<std::string>(std::to_string(x->value)); })) == //
        std::nullopt);

  v = std::make_unique<Foo>(42);
  CHECK((v ^ fold_partial([](int x) { return std::make_unique<std::string>(std::to_string(x)); })) == std::nullopt);
  CHECK((v ^ fold_partial([](const std::unique_ptr<Foo> &x) { return std::make_unique<std::string>(std::to_string(x->value)); }) ^
         map([](const auto &x) { return *x; })) == "42");
}

TEST_CASE("std::variant_fold_total_unique_ptr") {
  std::variant<std::unique_ptr<Foo>, int> v{1};
  CHECK((v ^ fold_total([](const std::unique_ptr<Foo> &x) { return x->value; }, [](int x) { return x; })) == 1);
  v = std::make_unique<Foo>(42);
  CHECK((v ^ fold_total([](const std::unique_ptr<Foo> &x) { return x->value; }, [](int x) { return x; })) == 42);
}

TEST_CASE("std::variant_fold_total") {
  std::variant<Foo, int, std::string> v{1};
  CHECK((v ^ fold_total([](Foo x) { return x.value; }, [](int x) { return x; }, [](const std::string &s) -> int { return s.length(); })) ==
        1);
  v = "aaa";
  CHECK((v ^ fold_total([](Foo x) { return x.value; }, [](int x) { return x; }, [](const std::string &s) -> int { return s.length(); })) ==
        3);
  v = Foo{42};
  CHECK((v ^ fold_total([](Foo x) { return x.value; }, [](int x) { return x; }, [](const std::string &s) -> int { return s.length(); })) ==
        42);
}

TEST_CASE("std::variant_fold_partial") {
  std::variant<Foo, int, std::string> v{1};
  CHECK((v ^ fold_partial([](Foo x) { return x.value; }, [](int x) { return x; },
                          [](const std::string &s) -> int { return s.length(); })) == 1);
  v = "aaa";
  CHECK((v ^ fold_partial([](Foo x) { return x.value; }, [](int x) { return x; },
                          [](const std::string &s) -> int { return s.length(); })) == 3);
  v = Foo{42};
  CHECK((v ^ fold_partial([](Foo x) { return x.value; }, [](int x) { return x; },
                          [](const std::string &s) -> int { return s.length(); })) == 42);

  v = 1;
  CHECK((v ^ fold_partial([](Foo x) { return x.value; })) == std::nullopt);
  v = "aaa";
  CHECK((v ^ fold_partial([](Foo x) { return x.value; })) == std::nullopt);
  v = Foo{42};
  CHECK((v ^ fold_partial([](Foo x) { return x.value; })) == 42);
}

TEST_CASE("std::variant_foreach_total") {
  std::variant<Foo, int, std::string> v{1};
  v ^ foreach_total([](Foo) { FAIL(); }, [](int x) { CHECK(x == 1); }, [](const std::string &) { FAIL(); });
  v = "aaa";
  v ^ foreach_total([](Foo) { FAIL(); }, [](int) { FAIL(); }, [](const std::string &s) { CHECK(s == "aaa"); });
  v = Foo{42};
  v ^ foreach_total([](Foo x) { CHECK(x == Foo{42}); }, [](int) { FAIL(); }, [](const std::string &) { FAIL(); });
}

TEST_CASE("std::variant_foreach_partial") {
  std::variant<Foo, int, std::string> v{1};
  v ^ foreach_partial([](Foo) { FAIL(); }, [](int x) { CHECK(x == 1); }, [](const std::string &) { FAIL(); });
  v = "aaa";
  v ^ foreach_partial([](Foo) { FAIL(); }, [](int) { FAIL(); }, [](const std::string &s) { CHECK(s == "aaa"); });
  v = Foo{42};
  v ^ foreach_partial([](Foo x) { CHECK(x == Foo{42}); }, [](int) { FAIL(); }, [](const std::string &) { FAIL(); });

  v = 1;
  v ^ foreach_partial([](Foo) { FAIL(); });
  v = "aaa";
  v ^ foreach_partial([](Foo) { FAIL(); });
  v = Foo{42};
  v ^ foreach_partial([](Foo x) { CHECK(x == Foo{42}); });
}
