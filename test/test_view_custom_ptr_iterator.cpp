#include <cassert>
#include <iterator>
#include <vector>

template <typename T> struct ArrayRef {
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = const_pointer;
  using const_iterator = const_pointer;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  constexpr ArrayRef(const T *begin, const T *end) : Data(begin), Length(end - begin) { assert(begin <= end); }
  template <typename A> explicit ArrayRef(const std::vector<T, A> &Vec) : Data(Vec.data()), Length(Vec.size()) {}

  [[nodiscard]] iterator begin() const { return Data; }
  [[nodiscard]] iterator end() const { return Data + Length; }
  [[nodiscard]] bool empty() const { return Length == 0; }

private:
  const T *Data = nullptr;
  size_type Length = 0;
};

#define OP_ | // NOLINT(*-macro-parentheses)
#define TPE_GROUP "test_view+custom_ptr_iterator"
#define TPE_NAME "view+custom_ptr_iterator"
#define TPE_CTOR_IN(T) std::vector<T>
#define TPE_CTOR_OUT(T) std::vector<T>
#define TPE_CTOR_VAR_OUT(T) std::vector<T>
#define TPE_MANY_INIT 1
#define TPE_RUN_TEST

#define TPE_TEST_CHAIN(xs) (ArrayRef(xs))

#include "catch2/catch_test_macros.hpp"

#include <aspartame/all.hpp>

#include "templates/test_template_view.hpp"
#include "test_base_includes.hpp"

TEST_CASE("ArrayRef_begin_end_ctor", "[view+custom_ptr_iterator]") {
  using namespace aspartame;
  const int data[] = {1, 2, 3, 4};
  auto v = ArrayRef<int>(std::begin(data), std::end(data)) | to_vector();
  REQUIRE(v == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("ArrayRef_empty_pair", "[view+custom_ptr_iterator]") {
  using namespace aspartame;
  const int *p = nullptr;
  auto v = ArrayRef<int>(p, p) | to_vector();
  REQUIRE(v == std::vector<int>{});
}
