#include <deque>
#include <iterator>
#include <list>
#include <memory>
#include <vector>

#include "catch2/catch_template_test_macros.hpp"
#include "catch2/catch_test_macros.hpp"

#include "aspartame/all.hpp"

#include "fixtures.hpp"
#include "test_base_harness.hpp"
#include "test_base_includes.hpp"

using namespace aspartame;

namespace {
// XXX std::list has no operator[]
template <typename C> auto at_idx(C &c, size_t i) -> decltype(*c.begin()) {
  auto it = c.begin();
  std::advance(it, static_cast<typename std::iterator_traits<decltype(it)>::difference_type>(i));
  return *it;
}

// XXX clang's ConstCastIterator deref type; reproducible without depending on llvm
struct SuperConstIter {
  using iterator_category = std::forward_iterator_tag;
  using value_type = const int *const;
  using difference_type = std::ptrdiff_t;
  using pointer = const int *const *;
  using reference = const int *const &;
  const int *const *p_ = nullptr;
  SuperConstIter() = default;
  explicit SuperConstIter(const int *const *p) : p_(p) {}
  reference operator*() const { return *p_; }
  SuperConstIter &operator++() {
    ++p_;
    return *this;
  }
  SuperConstIter operator++(int) {
    auto c = *this;
    ++p_;
    return c;
  }
  [[maybe_unused]] friend bool operator==(SuperConstIter a, SuperConstIter b) { return a.p_ == b.p_; }
  friend bool operator!=(SuperConstIter a, SuperConstIter b) { return a.p_ != b.p_; }
};

struct SuperConstRange {
  const int *const *first_;
  const int *const *last_;
  using value_type = const int *const;
  SuperConstIter begin() const { return SuperConstIter{first_}; }
  SuperConstIter end() const { return SuperConstIter{last_}; }
  size_t size() const { return static_cast<size_t>(last_ - first_); }
};
} // namespace

namespace aspartame {
template <> struct enable_pipe<::SuperConstRange> : std::true_type {};
template <> struct sequence_traits<::SuperConstRange> {
  using value_type = const int *const;
  template <typename U> using rebind = std::vector<U>;
  [[maybe_unused]] static constexpr bool set_like = false;
};
} // namespace aspartame

TEST_CASE("zip_with_index_super_const_value_type", "[zip_with_index][regression]") {
  int a = 10, b = 20, c = 30;
  const int *const storage[] = {&a, &b, &c};
  SuperConstRange r{std::begin(storage), std::end(storage)};
  auto pairs = r ^ zip_with_index<size_t>();
  REQUIRE(pairs.size() == 3);
  REQUIRE(*pairs[0].first == 10);
  REQUIRE(pairs[0].second == 0u);
  REQUIRE(*pairs[2].first == 30);
  REQUIRE(pairs[2].second == 2u);
}

TEMPLATE_TEST_CASE("map_over_filter_address_via_non_const_auto_ref", "[map][filter][regression]", std::vector<int>, std::deque<int>,
                   std::list<int>) {
  TestType xs{1, 2, 3, 4, 5};
  auto ptrs = xs                                           //
              | filter([](auto &x) { return x % 2 == 0; }) //
              | map([](auto &x) { return &x; })            //
              | to_vector();
  REQUIRE(ptrs.size() == 2);
  REQUIRE(*ptrs[0] == 2);
  REQUIRE(*ptrs[1] == 4);
}

TEMPLATE_TEST_CASE("collect_over_filter_with_non_const_auto_ref", "[collect][filter][regression]", std::vector<int>, std::deque<int>,
                   std::list<int>) {
  TestType xs{1, 2, 3, 4, 5};
  auto ptrs = xs                                                                               //
              | filter([](auto &x) { return x > 0; })                                          //
              | collect([](auto &x) { return x % 2 == 0 ? std::optional{&x} : std::nullopt; }) //
              | to_vector();
  REQUIRE(ptrs.size() == 2);
  REQUIRE(*ptrs[0] == 2);
  REQUIRE(*ptrs[1] == 4);
}

TEMPLATE_TEST_CASE("flat_map_iterator_over_filter_with_non_const_auto_ref", "[flat_map][filter][regression]", std::vector<int>,
                   std::deque<int>, std::list<int>) {
  TestType xs{1, 2, 3};
  auto ys = xs                                                              //
            | filter([](auto &x) { return x > 0; })                         //
            | flat_map([](auto &x) { return std::vector<int>{x, x * 10}; }) //
            | to_vector();
  REQUIRE(ys.size() == 6);
  REQUIRE(ys[0] == 1);
  REQUIRE(ys[1] == 10);
  REQUIRE(ys[5] == 30);
}

TEMPLATE_TEST_CASE("flat_map_moves_from_prvalue_inner_container", "[flat_map][move][regression]", std::vector<int>, std::deque<int>,
                   std::list<int>) {
  TestType seeds{1, 2, 3};
  auto ps = seeds ^ flat_map([](auto x) {
              std::vector<std::unique_ptr<int>> v;
              v.push_back(std::make_unique<int>(x * 10));
              v.push_back(std::make_unique<int>(x * 100));
              return v;
            });
  REQUIRE(ps.size() == 6);
  REQUIRE(*at_idx(ps, 0) == 10);
  REQUIRE(*at_idx(ps, 1) == 100);
  REQUIRE(*at_idx(ps, 5) == 300);
}

TEMPLATE_TEST_CASE("as_ref_yields_mutable_reference_wrapper_via_pipe", "[as_ref][regression]", std::vector<std::unique_ptr<int>>,
                   std::deque<std::unique_ptr<int>>, std::list<std::unique_ptr<int>>) {
  TestType xs;
  xs.push_back(std::make_unique<int>(1));
  xs.push_back(std::make_unique<int>(2));
  xs.push_back(std::make_unique<int>(3));

  auto refs = xs | as_ref() | to_vector();
  static_assert(std::is_same_v<typename decltype(refs)::value_type, std::reference_wrapper<std::unique_ptr<int>>>);
  REQUIRE(refs.size() == 3);
  REQUIRE(*refs[1].get() == 2);

  *refs[1].get() = 99;
  auto xs_it = xs.begin();
  std::advance(xs_it, 1);
  REQUIRE(**xs_it == 99);

  auto moved = std::move(refs[2].get());
  REQUIRE(*moved == 3);
  std::advance(xs_it, 1);
  REQUIRE(*xs_it == nullptr);
}

TEMPLATE_TEST_CASE("as_cref_yields_const_reference_wrapper", "[as_cref][regression]", std::vector<int>, std::deque<int>, std::list<int>) {
  TestType xs{10, 20, 30};
  auto crefs_pipe = xs | as_cref() | to_vector();
  auto crefs_caret = xs ^ as_cref();
  static_assert(std::is_same_v<typename decltype(crefs_pipe)::value_type, std::reference_wrapper<const int>>);
  static_assert(std::is_same_v<typename decltype(crefs_caret)::value_type, std::reference_wrapper<const int>>);
  REQUIRE(crefs_pipe.size() == 3);
  REQUIRE(crefs_pipe[1].get() == 20);
  REQUIRE(at_idx(crefs_caret, 2).get() == 30);
}

// XXX `^` is `const L &` so const propagates; `|` is `Iterable &&` so non-const lvalues stay non-const through the chain
TEMPLATE_TEST_CASE("caret_propagates_const_pipe_preserves_value_category", "[operators][regression]", std::vector<int>, std::deque<int>,
                   std::list<int>) {
  TestType xs{1, 2, 3};
  auto via_pipe = xs | map([](auto &x) { return std::ref(x); }) | to_vector();
  auto via_caret = xs ^ map([](auto &x) { return std::ref(x); });
  static_assert(std::is_same_v<typename decltype(via_pipe)::value_type, std::reference_wrapper<int>>);
  static_assert(std::is_same_v<typename decltype(via_caret)::value_type, std::reference_wrapper<const int>>);
}
