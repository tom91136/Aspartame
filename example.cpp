// #include "src/aspartame/array.hpp"
#include "src/aspartame/deque.hpp"
#include "src/aspartame/list.hpp"
#include "src/aspartame/optional.hpp"
#include "src/aspartame/set.hpp"
#include "src/aspartame/string.hpp"
#include "src/aspartame/unordered_map.hpp"
#include "src/aspartame/unordered_set.hpp"
#include "src/aspartame/vector.hpp"

#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>

#include "src/aspartame/fluent.hpp"

int main() {

  using namespace aspartame;

  auto U = std::optional{std::tuple<int, int>{42, 43}};

  std::optional a{42};

  std::optional<std::optional<int>> n = a | map([](auto x) { return std::optional{1}; });

  a | filter([](auto x) { return true; })              //
      | map([](auto x) { return x; })                  //
      | map([](auto x) { return x * 2; })              //
      | bind([](auto x) { return std::optional{x}; })  //
      | map([](auto x) { return std::optional{"a"}; }) //
      | flatten()                                      //
      | zip_with_index()                               //
      | tap_each([](auto x, auto i) { std::cout << x << "\n"; }) | map([](auto x, auto i) {
          return std::tuple{x, i, "a"};
        }) |
      for_each([](auto x, auto i, auto s) {});

  std::cout << "r = " << (std::vector{10, 20, 30} | reduce([](auto l, auto r) { return l + r; }) | get_or_else(42)) << "\n";
  std::cout << "flr = " << (std::vector{10, 20, 30} | fold_left(std::string(), [](auto l, auto r) { return l + std::to_string(r); }))
            << "\n";

  range::inclusive{0, 10}.tabulate<std::vector>([](auto i) { return i; }) | for_each([](auto x) {});

  std::vector{1, 2} | group_by([](auto x) { return std::to_string(x); });
  std::vector{1, 2} | group_map([](auto x) { return std::to_string(x); }, [](auto x) { return std::pair(x, x); });
  std::vector{1, 2} |
      group_map_reduce([](auto x) { return std::to_string(x); }, [](auto x) { return std::pair(x, x); }, [](auto x, auto y) { return x; });

  std::unordered_map<int, int>{{1, 2}, {3, 4}} | map([](auto x, auto y) {
    return std::pair{x, y * 2};
  }) | map([](auto x) { return x; }) |
      filter([](auto x, auto y) { return x + y > 5; }) |
      bind([](auto x) {
        return std::unordered_map<std::string, decltype(x)>{{std::to_string(x.first + x.second), x}};
      })
      // | last_optional()                      //
      // | to_vector()
      | for_each([](auto i, auto x) { std::cout << ": " << i << " " << x.first << "," << x.second << "\n"; });
  std::cout << "----\n";

  std::list{1, 2} | map([](auto x) { return x * 2; })         //
                                                              //      | bind([](auto x) {
                                                              //          return std::vector{x, x};
                                                              //        })                                   //
                                                              //      | filter([](auto x) { return x > 2; })                  //
      | zip_with_index()                                      //
      | map([](auto x) { return x; })                         //
                                                              // | last_optional()                      //
                                                              // | to_vector()
      | reverse() | sort_by([](auto i, auto x) { return i; }) //
      | for_each([](size_t i, int x) { std::cout << ": " << i << " " << x << "\n"; });
  std::cout << "----\n";

  return 0;
}
