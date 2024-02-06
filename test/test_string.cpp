#include "aspartame/string.hpp"
#include <random>

#include "aspartame/fluent.hpp"

#include "catch2/catch_test_macros.hpp"

#define OP_ ^
#define TPE_GROUP "test_string"
#define TPE_NAME "std::string"

using namespace aspartame;
using std::optional;
using std::string;
using std::wstring;

std::wstring operator"" _w(const char *str, std::size_t len) { return {str, str + len}; }

#ifndef DISABLE_MK_STRING
TEST_CASE(TPE_NAME "_mk_string", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("123" ^ mk_string()) == "123");
  CHECK(("abc" ^ mk_string(", ")) == "a, b, c");
  CHECK(("hello" ^ mk_string(", ", "[", "]")) == "[h, e, l, l, o]");
  CHECK(("" ^ mk_string(", ")) == "");
  CHECK(("a" ^ mk_string()) == "a");
  CHECK(("123" ^ mk_string("")) == "123");
  CHECK(("numbers" ^ mk_string("-", "<", ">")) == "<n-u-m-b-e-r-s>");
  CHECK(("" ^ mk_string("", "start-", "-end")) == "start--end");
  CHECK(("1" ^ mk_string(",", "(", ")")) == "(1)");
  CHECK(("single" ^ mk_string()) == "single");
}
#endif

#ifndef DISABLE_APPEND
TEST_CASE(TPE_NAME "_append", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("Hello" ^ append('!')) == "Hello!");
  CHECK(("123" ^ append('4')) == "1234");
  CHECK(("" ^ append('A')) == "A");
  CHECK(("" ^ append('\n')) == "\n");

  CHECK(("Hello"_w ^ append('!')) == "Hello!"_w);
}
#endif

#ifndef DISABLE_CONCAT
TEST_CASE(TPE_NAME "_concat", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("Hello" ^ concat(" World")) == "Hello World");
  CHECK(("123" ^ concat("456")) == "123456");
  CHECK(("" ^ concat("NotEmpty")) == "NotEmpty");
  CHECK(("NotEmpty" ^ concat("")) == "NotEmpty");

  CHECK(("Hello"_w ^ concat(" World"_w)) == "Hello World"_w);
}
#endif

#ifndef DISABLE_MAP
TEST_CASE(TPE_NAME "_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ map([](auto c) { return toupper(c); })) == "ABC");
  CHECK(("XYZ" ^ map([](auto c) { return tolower(c); })) == "xyz");
  CHECK(("123" ^ map([](auto c) { return c + 1; })) == "234");
  CHECK(("" ^ map([](auto c) { return toupper(c); })) == "");

  CHECK(("abc"_w ^ map([](auto c) { return toupper(c); })) == "ABC"_w);
}
#endif

#ifndef DISABLE_COLLECT
TEST_CASE(TPE_NAME "_collect", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc123" ^ collect([](auto c) { return isalpha(c) ? optional<char>{c} : std::nullopt; })) == "abc");
  CHECK(("Hello, World!" ^ collect([](auto c) { return isdigit(c) ? optional<char>{c} : std::nullopt; })) == "");
  CHECK(("a1b2c3" ^ collect([](auto c) { return isdigit(c) ? optional<char>{toupper(c)} : std::nullopt; })) == "123");
  CHECK(("" ^ collect([](auto c) { return optional<char>{c}; })) == "");

  CHECK(("abc123"_w ^ collect([](auto c) { return isalpha(c) ? optional<wchar_t>{c} : std::nullopt; })) == "abc"_w);
}
#endif

#ifndef DISABLE_FILTER
TEST_CASE(TPE_NAME "_filter", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello" ^ filter([](auto c) { return c != 'e'; })) == "hllo");
  CHECK(("12345" ^ filter([](auto c) { return c > '2'; })) == "345");
  CHECK(("abcde" ^ filter([](auto c) { return isalpha(c) && isupper(c); })) == "");
  CHECK(("" ^ filter([](auto c) { return c != 'a'; })) == "");

  CHECK(("hello"_w ^ filter([](auto c) { return c != 'e'; })) == "hllo"_w);
}
#endif

#ifndef DISABLE_BIND
TEST_CASE(TPE_NAME "_bind", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("123" ^ bind([](auto c) { return string(2, c); })) == "112233");
  CHECK(("123" ^ bind([](auto) { return ""; })) == "");
  CHECK(("abc" ^ bind([](auto c) { return string(1, toupper(c)); })) == "ABC");
  CHECK(("" ^ bind([](auto c) { return string(2, c); })) == "");

  CHECK(("123"_w ^ bind([](auto c) { return wstring(2, c); })) == "112233"_w);
}
#endif

#ifndef DISABLE_FLATTEN
TEST_CASE(TPE_NAME "_flatten", "[" TPE_NAME "][" TPE_GROUP "]") {
  // XXX no-op due to shape
}
#endif

#ifndef DISABLE_DISTINCT
TEST_CASE(TPE_NAME "_distinct", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("aabcc" ^ distinct()) == "abc");
  CHECK(("" ^ distinct()) == "");
  CHECK(("aaaa" ^ distinct()) == "a");
  CHECK(("abc" ^ distinct()) == "abc");

  CHECK(("aaaa"_w ^ distinct()) == "a"_w);
}
#endif

#ifndef DISABLE_DISTINCT_BY
TEST_CASE(TPE_NAME "_distinct_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("aabcc" ^ distinct_by([](auto c) { return c; })) == "abc");
  CHECK(("" ^ distinct_by([](auto c) { return c; })) == "");
  CHECK(("aaaa" ^ distinct_by([](auto c) { return c; })) == "a");
  CHECK(("abc" ^ distinct_by([](auto c) { return c; })) == "abc");

  CHECK(("aabcc" ^ distinct_by([](auto c) { return std::tolower(c); })) == "abc");
  CHECK(("ABCabc" ^ distinct_by([](auto c) { return std::tolower(c); })) == "ABC");
  CHECK(("12345" ^ distinct_by([](auto c) { return c - '0'; })) == "12345");

  CHECK(("AaAa"_w ^ distinct_by([](auto wc) { return std::towlower(wc); })) == "A"_w);
}
#endif

#ifndef DISABLE_COUNT
TEST_CASE(TPE_NAME "_count", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("aabcc" ^ count([](auto c) { return c == 'a'; })) == 2);
  CHECK(("aabcc" ^ count([](auto c) { return c == 'b'; })) == 1);
  CHECK(("" ^ count([](auto c) { return c == 'a'; })) == 0);
  CHECK(("aaaa" ^ count([](auto c) { return c == 'a'; })) == 4);
  CHECK(("abcde" ^ count([](auto c) { return c > 'c'; })) == 2);
}
#endif

#ifndef DISABLE_EXISTS
TEST_CASE(TPE_NAME "_exists", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("Hello World" ^ exists(::isspace)) == true);
  CHECK(("HelloWorld" ^ exists(::isspace)) == false);
  CHECK(("123" ^ exists(::isdigit)) == true);
  CHECK(("abc" ^ exists(::isdigit)) == false);

  CHECK(("Hello World"_w ^ exists(::isspace)) == true);
}
#endif

#ifndef DISABLE_FORALL
TEST_CASE(TPE_NAME "_forall", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("123" ^ forall(::isdigit)) == true);
  CHECK(("123a" ^ forall(::isdigit)) == false);
  CHECK(("" ^ forall(::isdigit)) == true);
  CHECK(("abc" ^ forall(::isalpha)) == true);

  CHECK(("123"_w ^ forall(::isdigit)) == true);
}
#endif

#ifndef DISABLE_REDUCE
TEST_CASE(TPE_NAME "_reduce", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcd" ^ reduce([](auto a, auto b) { return std::min(a, b); })).value() == 'a');
  CHECK(("1234" ^ reduce([](auto a, auto b) { return std::max(a, b); })).value() == '4');
  CHECK(("" ^ reduce([](auto a, auto b) { return a + b; })).has_value() == false);

  CHECK(("abcd"_w ^ reduce([](auto a, auto b) { return std::min(a, b); })).value() == 'a');
}
#endif

#ifndef DISABLE_TAP_EACH
TEST_CASE(TPE_NAME "_tap_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  {
    string actual;
    CHECK(("hello" ^ tap_each([&actual](auto c) { actual.push_back(c); })) == "hello");
    CHECK(actual == "hello");
  }
  {
    string actual;
    CHECK(("" ^ tap_each([&actual](auto c) { actual.push_back(c); })) == "");
    CHECK(actual == "");
  }

  {
    wstring actual;
    CHECK(("hello"_w ^ tap_each([&actual](auto c) { actual.push_back(c); })) == "hello"_w);
    CHECK(actual == "hello"_w);
  }
}
#endif

#ifndef DISABLE_FOR_EACH
TEST_CASE(TPE_NAME "_for_each", "[" TPE_NAME "][" TPE_GROUP "]") {
  {
    string actual;
    "hello" ^ for_each([&actual](auto c) { actual.push_back(c); });
    CHECK(actual == "hello");
  }
  {
    string actual;
    "" ^ for_each([&actual](auto c) { actual.push_back(c); });
    CHECK(actual == "");
  }

  {
    wstring actual;
    "hello"_w ^ for_each([&actual](auto c) { actual.push_back(c); });
    CHECK(actual == "hello"_w);
  }
}
#endif

#ifndef DISABLE_PARTITION
TEST_CASE(TPE_NAME "_partition", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("135246" ^ partition([](auto c) { return (c - '0') % 2 == 0; })) == std::pair<string, string>{"246", "135"});
  CHECK(("111" ^ partition([](auto c) { return (c - '0') % 2 == 0; })) == std::pair<string, string>{"", "111"});
  CHECK(("222" ^ partition([](auto c) { return (c - '0') % 2 == 0; })) == std::pair<string, string>{"222", ""});
  CHECK(("" ^ partition([](auto c) { return (c - '0') % 2 == 0; })) == std::pair<string, string>{"", ""});

  CHECK(("135246"_w ^ partition([](auto c) { return (c - '0') % 2 == 0; })) == std::pair<wstring, wstring>{"246"_w, "135"_w});
}
#endif

#ifndef DISABLE_GROUP_MAP_REDUCE
TEST_CASE(TPE_NAME "_group_map_reduce", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("aaabbc" ^ group_map_reduce([](auto c) { return c; }, [](auto) { return 1; }, std::plus<>{})) ==
        std::unordered_map<char, int>{{'a', 3}, {'b', 2}, {'c', 1}});
  CHECK(("" ^ group_map_reduce([](auto c) { return c; }, [](auto) { return 1; }, std::plus<>{})) == std::unordered_map<char, int>{});

  CHECK(("aaabbc"_w ^ group_map_reduce([](auto c) { return c; }, [](auto) { return 1; }, std::plus<>{})) ==
        std::unordered_map<wchar_t, int>{{'a', 3}, {'b', 2}, {'c', 1}});
}
#endif

#ifndef DISABLE_GROUP_MAP
TEST_CASE(TPE_NAME "_group_map", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("aaabbc" ^ group_map([](auto c) { return c; }, [](auto) { return 1; })) ==
        std::unordered_map<char, std::vector<int>>{{'a', {1, 1, 1}}, {'b', {1, 1}}, {'c', {1}}});
  CHECK(("" ^ group_map([](auto c) { return c; }, [](auto) { return 1; })) == std::unordered_map<char, std::vector<int>>{});

  CHECK(("aaabbc"_w ^ group_map([](auto c) { return c; }, [](auto) { return 1; })) ==
        std::unordered_map<wchar_t, std::vector<int>>{{'a', {1, 1, 1}}, {'b', {1, 1}}, {'c', {1}}});
}
#endif

#ifndef DISABLE_GROUP_BY
TEST_CASE(TPE_NAME "_group_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("aaabbc" ^ group_by([](auto c) { return c; })) ==
        std::unordered_map<char, std::vector<char>>{{'a', {'a', 'a', 'a'}}, {'b', {'b', 'b'}}, {'c', {'c'}}});
  CHECK(("" ^ group_by([](auto c) { return c; })) == std::unordered_map<char, std::vector<char>>{});

  CHECK(("aaabbc"_w ^ group_by([](auto c) { return c; })) ==
        std::unordered_map<wchar_t, std::vector<wchar_t>>{{'a', {'a', 'a', 'a'}}, {'b', {'b', 'b'}}, {'c', {'c'}}});
}
#endif

#ifndef DISABLE_TO_VECTOR
TEST_CASE(TPE_NAME "_to_vector", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcd" ^ to_vector()) == std::vector<char>{'a', 'b', 'c', 'd'});
  CHECK(("" ^ to_vector()) == std::vector<char>{});

  CHECK(("abcd"_w ^ to_vector()) == std::vector<wchar_t>{'a', 'b', 'c', 'd'});
}
#endif

// sequence...

#ifndef DISABLE_PREPEND
TEST_CASE(TPE_NAME "_prepend", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("xyz" ^ prepend('a')) == "axyz");
  CHECK(("" ^ prepend('a')) == "a");

  CHECK(("xyz"_w ^ prepend('a')) == "axyz"_w);
}
#endif

#ifndef DISABLE_HEAD_MAYBE
TEST_CASE(TPE_NAME "_head_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ head_maybe()) == optional<char>{'a'});
  CHECK(("" ^ head_maybe()) == std::nullopt);

  CHECK(("abc"_w ^ head_maybe()) == optional<wchar_t>{'a'});
}
#endif

#ifndef DISABLE_LAST_MAYBE
TEST_CASE(TPE_NAME "_last_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ last_maybe()) == optional<char>{'c'});
  CHECK(("" ^ last_maybe()) == std::nullopt);

  CHECK(("abc"_w ^ last_maybe()) == optional<wchar_t>{'c'});
}
#endif

#ifndef DISABLE_INIT
TEST_CASE(TPE_NAME "_init", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ init()) == "ab");
  CHECK(("a" ^ init()) == "");

  CHECK(("abc"_w ^ init()) == "ab"_w);
}
#endif

#ifndef DISABLE_TAIL
TEST_CASE(TPE_NAME "_tail", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ tail()) == "bc");
  CHECK(("a" ^ tail()) == "");

  CHECK(("abc"_w ^ tail()) == "bc"_w);
}
#endif

#ifndef DISABLE_AT_MAYBE
TEST_CASE(TPE_NAME "_at_maybe", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello" ^ at_maybe(0)) == optional<char>{'h'});
  CHECK(("hello" ^ at_maybe(4)) == optional<char>{'o'});
  CHECK(("hello" ^ at_maybe(5)) == std::nullopt);
  CHECK(("" ^ at_maybe(0)) == std::nullopt);

  CHECK(("hello"_w ^ at_maybe(0)) == optional<wchar_t>{'h'});
}
#endif

#ifndef DISABLE_SLICE
TEST_CASE(TPE_NAME "_slice", "[" TPE_NAME "][" TPE_GROUP "]") {
  auto op = [](size_t from, size_t to) { return [=](auto xs) { return xs ^ slice(from, to); }; };
  CHECK(("hello" ^ op(1, 4)) == "ell");
  CHECK(("hello" ^ op(0, 5)) == "hello");
  CHECK(("hello" ^ op(4, 5)) == "o");
  CHECK(("hello" ^ op(1, 1)) == "");
  CHECK(("hello" ^ op(5, 5)) == "");
  CHECK(("hello" ^ op(0, 0)) == "");
  CHECK(("" ^ op(0, 0)) == "");
  CHECK(("hello" ^ op(3, 2)) == "");
}
#endif

#ifndef DISABLE_INDEX_OF_SLICE
TEST_CASE(TPE_NAME "_index_of_slice", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ index_of_slice("ab")) == 0);
  CHECK(("abcde" ^ index_of_slice("cde")) == 2);

  CHECK(("abcde" ^ index_of_slice("fgh")) == -1);

  CHECK(("" ^ index_of_slice("abc")) == -1);
  CHECK(("a" ^ index_of_slice("a")) == 0);
  CHECK(("abcde" ^ index_of_slice("")) == -1);

  CHECK(("abcde"_w ^ index_of_slice(""_w)) == -1);
  CHECK(("abcde"_w ^ index_of_slice("ab"_w)) == 0);
}
#endif

#ifndef DISABLE_CONTAINS_SLICE
TEST_CASE(TPE_NAME "_contains_slice", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ contains_slice("ab")) == true);
  CHECK(("abcde" ^ contains_slice("cde")) == true);

  CHECK(("abcde" ^ contains_slice("fgh")) == false);

  CHECK(("" ^ contains_slice("abc")) == false);
  CHECK(("a" ^ contains_slice("a")) == true);
  CHECK(("abcde" ^ contains_slice("")) == false);

  CHECK(("abcde"_w ^ contains_slice(""_w)) == false);
  CHECK(("abcde"_w ^ contains_slice("ab"_w)) == true);
}
#endif

#ifndef DISABLE_INDEX_OF
TEST_CASE(TPE_NAME "_index_of", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello" ^ index_of('h')) == 0);
  CHECK(("hello" ^ index_of('o')) == 4);
  CHECK(("hello" ^ index_of('x')) == -1);
  CHECK(("" ^ index_of('h')) == -1);

  CHECK(("hello"_w ^ index_of('h')) == 0);
}
#endif

#ifndef DISABLE_CONTAINS
TEST_CASE(TPE_NAME "_contains", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello" ^ contains('h')) == true);
  CHECK(("hello" ^ contains('o')) == true);
  CHECK(("hello" ^ contains('x')) == false);
  CHECK(("" ^ contains('h')) == false);

  CHECK(("hello"_w ^ contains('h')) == true);
}
#endif

#ifndef DISABLE_INDEX_WHERE
TEST_CASE(TPE_NAME "_index_where", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello" ^ index_where([](auto c) { return c == 'h'; })) == 0);
  CHECK(("hello" ^ index_where([](auto c) { return c == 'o'; })) == 4);
  CHECK(("hello" ^ index_where([](auto c) { return c == 'x'; })) == -1);
  CHECK(("" ^ index_where([](auto c) { return c == 'h'; })) == -1);

  CHECK(("hello"_w ^ index_where([](auto c) { return c == 'h'; })) == 0);
}
#endif

#ifndef DISABLE_ZIP_WITH_INDEX
TEST_CASE(TPE_NAME "_zip_with_index", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ zip_with_index()) == std::vector<std::pair<char, size_t>>{std::pair{'a', 0}, std::pair{'b', 1}, std::pair{'c', 2}});
  CHECK(("" ^ zip_with_index()) == std::vector<std::pair<char, size_t>>{});
}
#endif

#ifndef DISABLE_ZIP
TEST_CASE(TPE_NAME "_zip", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ zip("123")) == std::vector{std::pair{'a', '1'}, std::pair{'b', '2'}, std::pair{'c', '3'}});
  CHECK(("ab" ^ zip("123")) == std::vector{std::pair{'a', '1'}, std::pair{'b', '2'}});
  CHECK(("abc" ^ zip("12")) == std::vector{std::pair{'a', '1'}, std::pair{'b', '2'}});
  CHECK(("" ^ zip("")) == std::vector<std::pair<char, char>>{});

  CHECK(("ab"_w ^ zip("123"_w)) == std::vector{std::pair<wchar_t, wchar_t>{'a', '1'}, std::pair<wchar_t, wchar_t>{'b', '2'}});
}
#endif

#ifndef DISABLE_TRANSPOSE
TEST_CASE(TPE_NAME "_transpose", "[" TPE_NAME "][" TPE_GROUP "]") {
  // XXX no-op due to shape
}
#endif

#ifndef DISABLE_REVERSE
TEST_CASE(TPE_NAME "_reverse", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ reverse()) == "cba");
  CHECK(("" ^ reverse()) == "");

  CHECK(("abc"_w ^ reverse()) == "cba"_w);
}
#endif

#ifndef DISABLE_SHUFFLE
TEST_CASE(TPE_NAME "_shuffle", "[" TPE_NAME "][" TPE_GROUP "]") {
  std::mt19937 rng(42); // NOLINT(*-msc51-cpp)
  CHECK(("abc" ^ shuffle(rng)) != "abc");
  CHECK(("" ^ shuffle(rng)) == "");

  CHECK(("abc"_w ^ shuffle(rng)) != "abc"_w);

  // XXX technically not part of spec
  CHECK(("abc" ^ shuffle(std::mt19937(42))) == ("abc" ^ shuffle(std::mt19937(42)))); // NOLINT(*-msc51-cpp)
}
#endif

#ifndef DISABLE_SORT
TEST_CASE(TPE_NAME "_sort", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("cba" ^ sort()) == "abc");
  CHECK(("cbaA" ^ sort()) == "Aabc");
  CHECK(("" ^ sort()) == "");
  CHECK(("cba"_w ^ sort()) == "abc"_w);

  CHECK(("cba" ^ sort([](auto l, auto r) { return l > r; })) == "cba");
  CHECK(("cbaA" ^ sort([](auto l, auto r) { return l > r; })) == "cbaA");
  CHECK(("" ^ sort([](auto l, auto r) { return l > r; })) == "");
  CHECK(("cba"_w ^ sort([](auto l, auto r) { return l > r; })) == "cba"_w);
}
#endif

#ifndef DISABLE_SORT_BY
TEST_CASE(TPE_NAME "_sort_by", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("cbaA" ^ sort_by([](auto x) { return std::tolower(x); })) == "aAbc");
  CHECK(("" ^ sort_by([](auto x) { return std::tolower(x); })) == "");

  CHECK(("cbaA"_w ^ sort_by([](auto x) { return std::tolower(x); })) == "aAbc"_w);
}
#endif

#ifndef DISABLE_SPLIT_AT
TEST_CASE(TPE_NAME "_split_at", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ split_at(1)) == std::pair<string, string>{"a", "bc"});
  CHECK(("abc" ^ split_at(0)) == std::pair<string, string>{"", "abc"});
  CHECK(("abc" ^ split_at(3)) == std::pair<string, string>{"abc", ""});
  CHECK(("" ^ split_at(0)) == std::pair<string, string>{"", ""});

  CHECK(("abc"_w ^ split_at(1)) == std::pair<wstring, wstring>{"a"_w, "bc"_w});
}
#endif

#ifndef DISABLE_TAKE
TEST_CASE(TPE_NAME "_take", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ take(2)) == "ab");
  CHECK(("abc" ^ take(0)) == "");
  CHECK(("abc" ^ take(5)) == "abc");
  CHECK(("" ^ take(2)) == "");

  CHECK(("abc"_w ^ take(5)) == "abc"_w);
  CHECK(("abc"_w ^ take(2)) == "ab"_w);
  CHECK(("abc"_w ^ take(0)) == ""_w);
}
#endif

#ifndef DISABLE_DROP
TEST_CASE(TPE_NAME "_drop", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ drop(0)) == "abcde");
  CHECK(("abcde" ^ drop(2)) == "cde");
  CHECK(("abcde" ^ drop(5)) == "");
  CHECK(("abcde" ^ drop(6)) == "");
  CHECK(("" ^ drop(0)) == "");

  CHECK(("abcde"_w ^ drop(0)) == "abcde"_w);
  CHECK(("abcde"_w ^ drop(2)) == "cde"_w);
  CHECK(("abcde"_w ^ drop(6)) == ""_w);
}
#endif

#ifndef DISABLE_TAKE_RIGHT
TEST_CASE(TPE_NAME "_take_right", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ take_right(2)) == "de");
  CHECK(("abcde" ^ take_right(0)) == "");
  CHECK(("abcde" ^ take_right(6)) == "abcde");
  CHECK(("" ^ take_right(1)) == "");

  CHECK(("abcde"_w ^ take_right(2)) == "de"_w);
  CHECK(("abcde"_w ^ take_right(0)) == ""_w);
}
#endif

#ifndef DISABLE_DROP_RIGHT
TEST_CASE(TPE_NAME "_drop_right", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ drop_right(2)) == "abc");
  CHECK(("abcde" ^ drop_right(0)) == "abcde");
  CHECK(("abcde" ^ drop_right(6)) == "");
  CHECK(("" ^ drop_right(1)) == "");

  CHECK(("abcde"_w ^ drop_right(2)) == "abc"_w);
  CHECK(("abcde"_w ^ drop_right(6)) == ""_w);
}
#endif

#ifndef DISABLE_TAKE_WHILE
TEST_CASE(TPE_NAME "_take_while", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ take_while([](auto c) { return c < 'd'; })) == "abc");
  CHECK(("abcde" ^ take_while([](auto) { return false; })) == "");
  CHECK(("" ^ take_while([](auto) { return true; })) == "");

  CHECK(("abcde"_w ^ take_while([](auto c) { return c < 'd'; })) == "abc"_w);
}
#endif

#ifndef DISABLE_DROP_WHILE
TEST_CASE(TPE_NAME "_drop_while", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ drop_while([](auto c) { return c < 'd'; })) == "de");
  CHECK(("abcde" ^ drop_while([](auto) { return true; })) == "");
  CHECK(("" ^ drop_while([](auto) { return false; })) == "");

  CHECK(("abcde"_w ^ drop_while([](auto c) { return c < 'd'; })) == "de"_w);
}
#endif

#ifndef DISABLE_FOLD_LEFT
TEST_CASE(TPE_NAME "_fold_left", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ fold_left(string(""), [](auto acc, auto c) { return acc + c; })) == "abc");
  CHECK(("" ^ fold_left(string("init"), [](auto acc, auto c) { return acc + c; })) == "init");
  CHECK(("hello" ^ fold_left(std::vector<char>{}, [](auto acc, auto x) {
           acc.push_back(x);
           return acc;
         })) == std::vector<char>{'h', 'e', 'l', 'l', 'o'});

  CHECK(("abc"_w ^ fold_left(""_w, [](auto acc, auto c) { return acc + c; })) == "abc"_w);
}
#endif

#ifndef DISABLE_FOLD_RIGHT
TEST_CASE(TPE_NAME "_fold_right", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abc" ^ fold_right(string(""), [](auto c, auto acc) { return acc + c; })) == "cba");
  CHECK(("" ^ fold_right(string("init"), [](auto c, auto acc) { return acc + c; })) == "init");
  CHECK(("hello" ^ fold_right(std::vector<char>{}, [](auto x, auto acc) {
           acc.insert(acc.begin(), x);
           return acc;
         })) == std::vector<char>{'h', 'e', 'l', 'l', 'o'});

  CHECK(("abc"_w ^ fold_right(""_w, [](auto c, auto acc) { return acc + c; })) == "cba"_w);
}
#endif

#ifndef DISABLE_SLIDING
TEST_CASE(TPE_NAME "_sliding", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ sliding(1, 1)) == std::vector<string>{"a", "b", "c", "d", "e"});
  CHECK(("abcde" ^ sliding(2, 1)) == std::vector<string>{"ab", "bc", "cd", "de"});
  CHECK(("abcde" ^ sliding(2, 2)) == std::vector<string>{"ab", "cd", "e"});
  CHECK(("abcde" ^ sliding(2, 3)) == std::vector<string>{"ab", "de"});
  CHECK(("abcde" ^ sliding(2, 4)) == std::vector<string>{"ab", "e"});
  CHECK(("abcde" ^ sliding(2, 5)) == std::vector<string>{"ab"});
  CHECK(("" ^ sliding(2, 5)) == std::vector<string>{});

  CHECK(("abcde" ^ sliding(1, 3)) == std::vector<string>{"a", "d"});
  CHECK(("abcde" ^ sliding(2, 2)) == std::vector<string>{"ab", "cd", "e"});
  CHECK(("abcde" ^ sliding(1, 10)) == std::vector<string>{"a"});
  CHECK(("abcde" ^ sliding(10, 1)) == std::vector<string>{"abcde"});
  CHECK(("abc" ^ sliding(4, 1)) == std::vector<string>{"abc"});
}
#endif

#ifndef DISABLE_GROUPED
TEST_CASE(TPE_NAME "_grouped", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ grouped(1)) == std::vector<string>{"a", "b", "c", "d", "e"});
  CHECK(("abcde" ^ grouped(2)) == std::vector<string>{"ab", "cd", "e"});
  CHECK(("abcde" ^ grouped(3)) == std::vector<string>{"abc", "de"});
  CHECK(("" ^ grouped(2)) == std::vector<string>{});
}
#endif

// =============

#ifndef DISABLE_TRIM_LEADING
TEST_CASE(TPE_NAME "_trim_leading", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("    abcde" ^ trim_leading()) == "abcde");
  CHECK(("abcde     " ^ trim_leading()) == "abcde     ");
  CHECK(("  abcde  " ^ trim_leading()) == "abcde  ");
  CHECK(("" ^ trim_leading()) == "");
  CHECK(("     " ^ trim_leading()) == "");

  CHECK(("    abcde"_w ^ trim_leading()) == "abcde"_w);
}
#endif

#ifndef DISABLE_TRIM_TRAILING
TEST_CASE(TPE_NAME "_trim_trailing", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("    abcde" ^ trim_trailing()) == "    abcde");
  CHECK(("abcde     " ^ trim_trailing()) == "abcde");
  CHECK(("  abcde  " ^ trim_trailing()) == "  abcde");
  CHECK(("" ^ trim_trailing()) == "");
  CHECK(("     " ^ trim_trailing()) == "");

  CHECK(("    abcde"_w ^ trim_trailing()) == "    abcde"_w);
}
#endif

#ifndef DISABLE_TRIM
TEST_CASE(TPE_NAME "_trim", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("    abcde   " ^ trim()) == "abcde");
  CHECK(("abcde" ^ trim()) == "abcde");
  CHECK(("   " ^ trim()) == "");
  CHECK(("" ^ trim()) == "");
  CHECK(("  abcde  " ^ trim()) == "abcde");

  CHECK(("    abcde   "_w ^ trim()) == "abcde"_w);
}
#endif

#ifndef DISABLE_IS_BLANK
TEST_CASE(TPE_NAME "_is_blank", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("" ^ is_blank()) == true);
  CHECK(("     " ^ is_blank()) == true);
  CHECK(("abcde" ^ is_blank()) == false);
  CHECK(("  abcde  " ^ is_blank()) == false);

  CHECK((""_w ^ is_blank()) == true);
}
#endif

#ifndef DISABLE_INDENT
TEST_CASE(TPE_NAME "_indent", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ indent(3)) == "   abcde");
  CHECK(("abcde" ^ indent(0)) == "abcde");
  CHECK(("" ^ indent(3)) == "");
  CHECK(("\nabc\n" ^ indent(2)) == "  \n  abc\n");
  CHECK(("abc" ^ indent(0)) == "abc");
  CHECK(("abc\nxyz" ^ indent(2)) == "  abc\n  xyz");
  CHECK(("\nabc\nxyz\n" ^ indent(3)) == "   \n   abc\n   xyz\n");
  CHECK(("abc\nxyz" ^ indent(-3)) == "abc\nxyz");
  CHECK(("   abc\n   xyz" ^ indent(-3)) == "abc\nxyz");
  CHECK(("   abc\n   xyz" ^ indent(-10)) == "abc\nxyz");

  CHECK(("\nabc\nxyz\n"_w ^ indent(3)) == "   \n   abc\n   xyz\n"_w);
}
#endif

#ifndef DISABLE_TO_UPPER
TEST_CASE(TPE_NAME "_to_upper", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("abcde" ^ to_upper()) == "ABCDE");
  CHECK(("ABCDE" ^ to_upper()) == "ABCDE");
  CHECK(("AbCdE" ^ to_upper()) == "ABCDE");
  CHECK(("12345" ^ to_upper()) == "12345");
  CHECK(("" ^ to_upper()) == "");

  CHECK(("AbCdE"_w ^ to_upper()) == "ABCDE"_w);
}
#endif

#ifndef DISABLE_TO_LOWER
TEST_CASE(TPE_NAME "_to_lower", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("ABCDE" ^ to_lower()) == "abcde");
  CHECK(("abcde" ^ to_lower()) == "abcde");
  CHECK(("AbCdE" ^ to_lower()) == "abcde");
  CHECK(("12345" ^ to_lower()) == "12345");
  CHECK(("" ^ to_lower()) == "");

  CHECK(("AbCdE"_w ^ to_lower()) == "abcde"_w);
}
#endif

#ifndef DISABLE_REPLACE_ALL
TEST_CASE(TPE_NAME "_replace_all", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello world" ^ replace_all("world", "there")) == "hello there");
  CHECK(("hello world world" ^ replace_all("world", "there")) == "hello there there");
  CHECK(("hello" ^ replace_all("world", "there")) == "hello");
  CHECK(("" ^ replace_all("world", "there")) == "");
  CHECK(("world" ^ replace_all("world", "")) == "");
  CHECK(("world world" ^ replace_all("", "there")) == "world world");

  CHECK(("hello world"_w ^ replace_all("world"_w, "there"_w)) == "hello there"_w);
}
#endif

#ifndef DISABLE_CONTAINS_IGNORE_CASE
TEST_CASE(TPE_NAME "_contains_ignore_case", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("Hello World" ^ contains_ignore_case("world")) == true);
  CHECK(("Hello World" ^ contains_ignore_case("WORLD")) == true);
  CHECK(("Hello World" ^ contains_ignore_case("hello")) == true);
  CHECK(("Hello World" ^ contains_ignore_case("there")) == false);
  CHECK(("" ^ contains_ignore_case("there")) == false);
  CHECK(("Hello World" ^ contains_ignore_case("")) == true);

  CHECK(("Hello World"_w ^ contains_ignore_case("world"_w)) == true);
}
#endif

#ifndef DISABLE_EQUALS_IGNORE_CASE
TEST_CASE(TPE_NAME "_equals_ignore_case", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("Hello World" ^ equals_ignore_case("hello world")) == true);
  CHECK(("HELLO WORLD" ^ equals_ignore_case("hello world")) == true);
  CHECK(("Hello" ^ equals_ignore_case("Hello")) == true);
  CHECK(("Hello World" ^ equals_ignore_case("Hello There")) == false);
  CHECK(("" ^ equals_ignore_case("")) == true);
  CHECK(("Hello World" ^ equals_ignore_case("")) == false);

  CHECK(("Hello World"_w ^ equals_ignore_case("hello world"_w)) == true);
}
#endif

#ifndef DISABLE_SPLIT
TEST_CASE(TPE_NAME "_split", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello,world,here" ^ split(',')) == std::vector<string>{"hello", "world", "here"});
  CHECK(("hello world here" ^ split(' ')) == std::vector<string>{"hello", "world", "here"});
  CHECK(("hello" ^ split('o')) == std::vector<string>{"hell", ""});
  CHECK(("hello" ^ split("lo")) == std::vector<string>{"hel", ""});
  CHECK(("" ^ split(',')) == std::vector<string>{""});
  CHECK(("hello" ^ split("world")) == std::vector<string>{"hello"});

  CHECK(("hello"_w ^ split(wchar_t('o'))) == std::vector<wstring>{"hell"_w, ""_w});
  CHECK(("hello"_w ^ split("lo"_w)) == std::vector<wstring>{"hel"_w, ""_w});
}
#endif

#ifndef DISABLE_LINES
TEST_CASE(TPE_NAME "_lines", "[" TPE_NAME "][" TPE_GROUP "]") {
  CHECK(("hello\nworld" ^ lines()) == std::vector<string>{"hello", "world"});
  CHECK(("hello\nworld\n" ^ lines()) == std::vector<string>{"hello", "world"});
  CHECK(("" ^ lines()) == std::vector<string>{});
  CHECK(("\n" ^ lines()) == std::vector<string>{""});
  CHECK(("\n\n\n" ^ lines()) == std::vector<string>{"", "", ""});

  CHECK(("hello\nworld\n"_w ^ lines()) == std::vector<wstring>{"hello"_w, "world"_w});
}
#endif
