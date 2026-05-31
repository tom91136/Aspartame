#include "test_base_includes.hpp"

#define OP_ ^
#define TPE_GROUP "test_optional"
#define TPE_NAME "std::optional"
#define TPE_CTOR_IN(T) std::optional<T>
#define TPE_CTOR_OUT(T) std::optional<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) (empty(x) ? std::nullopt : std::optional{*x.begin()})
#define TPE_INIT_SKIP(x) ((x).size() > 1)

#define DISABLE_INDEX_OF_MIN
#define DISABLE_INDEX_OF_MAX
#define DISABLE_INDEX_OF_MIN_BY
#define DISABLE_INDEX_OF_MAX_BY
#define DISABLE_INTERSECT
#define DISABLE_DIFF
#define DISABLE_SCAN_LEFT
#define DISABLE_SCAN_RIGHT
#define DISABLE_UNZIP
#define DISABLE_CROSS
#define DISABLE_COMBINATIONS
#define DISABLE_PERMUTATIONS
#define DISABLE_CHUNK_BY
#define DISABLE_JOIN_WITH
#define DISABLE_SYMMETRIC_DIFFERENCE
#define DISABLE_COMBINE_ALL
#define DISABLE_INTERSPERSE
#define DISABLE_TRAVERSE
#define DISABLE_SEQUENCE

#include "catch2/catch_test_macros.hpp"

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_optional.hpp"
#include "templates/test_template_sequence.hpp"

TEST_CASE("optional_attempt_returns_value_on_success", "[std::optional][attempt]") {
  using namespace aspartame;
  auto r = attempt([] { return 42; });
  REQUIRE(r.has_value());
  CHECK(*r == 42);
}

TEST_CASE("optional_attempt_returns_nullopt_on_throw", "[std::optional][attempt]") {
  using namespace aspartame;
  auto r = attempt([]() -> int { throw std::runtime_error("boom"); });
  CHECK_FALSE(r.has_value());
}
