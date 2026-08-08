#include <string_view>
#include <type_traits>

#include "catch2/catch_test_macros.hpp"

#include "test_base_includes.hpp"

using namespace aspartame;

namespace {
struct custom_char_traits : std::char_traits<char> {};
using custom_string = std::basic_string<char, custom_char_traits>;
using custom_string_view = std::basic_string_view<char, custom_char_traits>;
} // namespace

TEST_CASE("string_view_shape_ops", "[std::string_view][string]") {
  const std::string storage = "  alpha:beta:gamma  ";
  const std::string_view input = storage;
  const auto clean = input ^ trim();

  STATIC_REQUIRE(std::is_same_v<decltype(clean), const std::string_view>);
  CHECK(clean.data() == storage.data() + 2);
  CHECK((clean ^ split(':')) == std::vector<std::string_view>{"alpha", "beta", "gamma"});
  CHECK((clean ^ split("")) == std::vector<std::string_view>{clean});
  CHECK((clean ^ take(5)) == "alpha");
  CHECK((clean ^ drop(6)) == "beta:gamma");
  CHECK((clean ^ take_right(5)) == "gamma");
  CHECK((clean ^ drop_right(6)) == "alpha:beta");
  CHECK((clean ^ init()) == "alpha:beta:gamm");
  CHECK((clean ^ tail()) == "lpha:beta:gamma");
  CHECK((clean ^ slice(6, 10)) == "beta");
  CHECK((clean ^ take_while([](char c) { return c != ':'; })) == "alpha");
  CHECK((clean ^ drop_while([](char c) { return c != ':'; })) == ":beta:gamma");
  CHECK((clean ^ split_at(5)) == std::pair{std::string_view{"alpha"}, std::string_view{":beta:gamma"}});
  CHECK((clean ^ span([](char c) { return c != ':'; })) == std::pair{std::string_view{"alpha"}, std::string_view{":beta:gamma"}});
}

TEST_CASE("string_view_split_once", "[std::string_view][string]") {
  const std::string_view input = "a::b::c";
  CHECK((input ^ split_once("::")) == std::optional{std::pair{std::string_view{"a"}, std::string_view{"b::c"}}});
  CHECK((input ^ rsplit_once("::")) == std::optional{std::pair{std::string_view{"a::b"}, std::string_view{"c"}}});
  CHECK_FALSE(input ^ split_once(';'));
  CHECK_FALSE(input ^ split_once(""));
  CHECK((std::string_view{"::a"} ^ split_once("::")) == std::optional{std::pair{std::string_view{}, std::string_view{"a"}}});
  CHECK((std::string_view{"a::"} ^ rsplit_once("::")) == std::optional{std::pair{std::string_view{"a"}, std::string_view{}}});
}

TEST_CASE("wstring_view_operations", "[std::wstring_view][string]") {
  const std::wstring storage = L"  alpha::beta  ";
  const std::wstring_view input = storage;
  const auto clean = input ^ trim();
  STATIC_REQUIRE(std::is_same_v<decltype(clean), const std::wstring_view>);
  CHECK(clean.data() == storage.data() + 2);
  CHECK((clean ^ split_once(L"::")) == std::optional{std::pair{std::wstring_view{L"alpha"}, std::wstring_view{L"beta"}}});
  CHECK((clean ^ rsplit_once(L"::")) == std::optional{std::pair{std::wstring_view{L"alpha"}, std::wstring_view{L"beta"}}});
  CHECK(clean ^ glob_matches(L"a*::b?t?"));
  CHECK(clean ^ glob_matches_ignore_case(L"A*::B?T?"));
}

TEST_CASE("string_view_words_and_lines", "[std::string_view][string]") {
  CHECK((std::string_view{" one\t two  three "} ^ words()) == std::vector<std::string_view>{"one", "two", "three"});
  CHECK((std::string_view{"one\ntwo\n"} ^ lines()) == std::vector<std::string_view>{"one", "two"});
}

TEST_CASE("string_view_glob_matches", "[std::string_view][string]") {
  CHECK(std::string_view{"alpha.cpp"} ^ glob_matches("a?pha.*"));
  CHECK(std::string_view{"anything"} ^ glob_matches("*"));
  CHECK_FALSE(std::string_view{"alpha.cpp"} ^ glob_matches("a?pha.hpp"));
  CHECK(std::string_view{"Alpha.CPP"} ^ glob_matches_ignore_case("a?pha.*"));
  CHECK_FALSE(std::string_view{"Alpha.CPP"} ^ glob_matches_ignore_case("a?pha.hpp"));
  CHECK(std::string_view{} ^ glob_matches(""));
  CHECK(std::string_view{"abc"} ^ glob_matches("**a**b**c**"));
  CHECK_FALSE(std::string_view{} ^ glob_matches("?"));
}

TEST_CASE("string_view_owning_results_preserve_traits", "[std::string_view][string][traits]") {
  const custom_string input{" ab "};
  const custom_string_view view{input};
  const auto identity = [](char c) { return c; };
  static_assert(std::is_same_v<decltype(view ^ map(identity)), custom_string>);
  static_assert(std::is_same_v<decltype(view ^ replace_all("a", "x")), custom_string>);
  static_assert(std::is_same_v<decltype(view ^ indent(2)), custom_string>);
  static_assert(std::is_same_v<decltype(view ^ trim()), custom_string_view>);
  CHECK((view ^ map(identity)) == input);
  CHECK((view ^ replace_all("a", "x")) == custom_string{" xb "});
  CHECK((custom_string_view{"a\nb"} ^ indent(2)) == custom_string{"  a\n  b"});
}

TEST_CASE("string_view_owning_transforms", "[std::string_view][string]") {
  CHECK(std::string_view{"   "} ^ is_blank());
  CHECK(std::string_view{"Hello"} ^ starts_with("Hel"));
  CHECK(std::string_view{"Hello"} ^ ends_with("llo"));
  CHECK((std::string_view{"Hello"} ^ strip_prefix("He")) == "llo");
  CHECK((std::string_view{"Hello"} ^ strip_suffix("lo")) == "Hel");
  CHECK((std::string_view{"Hello"} ^ to_upper()) == "HELLO");
  CHECK((std::string_view{"Hello"} ^ to_lower()) == "hello");
  CHECK((std::string_view{"hello"} ^ capitalize()) == "Hello");
  CHECK((std::string_view{"Hello"} ^ uncapitalize()) == "hello");
  CHECK(std::string_view{"Hello World"} ^ contains_ignore_case("WORLD"));
  CHECK(std::string_view{"Hello"} ^ equals_ignore_case("hELLo"));
  CHECK((std::string_view{"a-b-a"} ^ replace_all("a", "x")) == "x-b-x");
  CHECK((std::string_view{"a-b-a"} ^ replace_first("a", "x")) == "x-b-a");
  CHECK((std::string_view{"a\nb"} ^ indent(2)) == "  a\n  b");
  CHECK((std::string_view{"ab"} ^ repeated(2)) == "abab");
  CHECK((std::string_view{"ab"} ^ pad_left(4, '0')) == "00ab");
  CHECK((std::string_view{"ab"} ^ pad_right(4, '0')) == "ab00");
}
