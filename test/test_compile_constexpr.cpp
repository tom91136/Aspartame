// Compile-time witness: static_assert evaluations that fail to build if the listed ops aren't
// actually constexpr-evaluable on the configured stdlib.

#include <array>
#include <optional>

#include "catch2/catch_test_macros.hpp"

#include "aspartame/array.hpp"
#include "aspartame/optional.hpp"

using namespace aspartame;

namespace {

constexpr std::array<int, 5> xs{1, 2, 3, 4, 5};

static_assert((xs ^ head_maybe()) == std::optional<int>{1});
static_assert((xs ^ last_maybe()) == std::optional<int>{5});
static_assert((xs ^ at_maybe(2)) == std::optional<int>{3});
static_assert((xs ^ at_maybe(100)) == std::optional<int>{});

constexpr std::optional<int> some{42};
constexpr std::optional<int> none{};
static_assert((some ^ get_or_else(0)) == 42);
static_assert((none ^ get_or_else(99)) == 99);
static_assert((some ^ exists([](int x) { return x == 42; })));
static_assert(!(none ^ exists([](int) { return true; })));

#if defined(__cpp_lib_constexpr_algorithms) && __cpp_lib_constexpr_algorithms >= 201806L
static_assert((xs ^ count([](int x) { return x > 2; })) == 3u);
static_assert((xs ^ exists([](int x) { return x == 3; })));
static_assert(!(xs ^ exists([](int x) { return x == 99; })));
static_assert((xs ^ forall([](int x) { return x > 0; })));
static_assert((xs ^ fold_left(0, [](int a, int b) { return a + b; })) == 15);
static_assert((xs ^ index_of(3)) == 2);
static_assert((xs ^ contains(4)));
static_assert((xs ^ none_match([](int x) { return x > 100; })));

constexpr auto doubled = xs ^ map([](int x) { return x * 2; });
static_assert(std::is_same_v<decltype(doubled), const std::array<int, 5>>);
static_assert(doubled[0] == 2);
static_assert(doubled[4] == 10);

constexpr auto chained_sum = xs ^ map([](int x) { return x + 1; }) ^ fold_left(0, [](int a, int b) { return a + b; });
static_assert(chained_sum == 20);
#endif

#if defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L && defined(__cpp_lib_constexpr_algorithms) &&             \
    __cpp_lib_constexpr_algorithms >= 201806L
// std::vector can't escape as a constexpr namespace-scope variable (heap allocation isn't
// core-constant); it must be constructed and consumed entirely inside a constexpr function.
  #include "aspartame/vector.hpp"
constexpr size_t vec_filtered_count() {
  std::vector<int> v{1, 2, 3, 4, 5};
  return (v ^ filter([](int x) { return x % 2 == 0; })).size();
}
static_assert(vec_filtered_count() == 2);

constexpr int vec_mapped_sum() {
  std::vector<int> v{1, 2, 3};
  auto ys = v ^ map([](int x) { return x * 10; });
  return ys ^ fold_left(0, [](int a, int b) { return a + b; });
}
static_assert(vec_mapped_sum() == 60);
#endif

TEST_CASE("constexpr_ops_compile", "[constexpr]") { STATIC_REQUIRE((xs ^ head_maybe()) == std::optional<int>{1}); }

} // namespace
