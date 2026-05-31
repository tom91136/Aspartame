#include "catch2/catch_test_macros.hpp"

#include "fixtures.hpp"
#include "test_base_includes.hpp"

#if __has_include(<version>)
  #include <version>
#endif

#define OP_ ^
#define TPE_GROUP "test_vector"
#define TPE_NAME "std::vector"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_sequence.hpp"

#define TPE_INPLACE_SEQ 1
#include "templates/test_template_inplace.hpp"

#if defined(__cpp_lib_expected)
TEST_CASE("vector_partition_map_splits_oks_and_errs", "[std::vector][partition_map]") {
  using namespace aspartame;
  std::vector<int> xs{1, 2, 3, 4, 5};
  auto [errs, oks] = xs ^ partition_map([](int x) -> std::expected<int, std::string> {
                       if (x % 2 == 0) return x * 10;
                       return std::unexpected(std::to_string(x));
                     });
  CHECK(oks == std::vector<int>{20, 40});
  CHECK(errs == std::vector<std::string>{"1", "3", "5"});
}
#endif

TEST_CASE("vector_combine_all_nested_flattens", "[std::vector][combine_all]") {
  using namespace aspartame;
  std::vector<std::vector<int>> xs{{1, 2}, {3, 4}, {5}};
  CHECK((xs ^ combine_all()) == std::vector<int>{1, 2, 3, 4, 5});
}
