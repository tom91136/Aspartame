// #include "src/aspartame/array.hpp"
#include "src/aspartame/deque.hpp"
#include "src/aspartame/list.hpp"
#include "src/aspartame/map.hpp"
#include "src/aspartame/optional.hpp"
#include "src/aspartame/set.hpp"
#include "src/aspartame/string.hpp"
#include "src/aspartame/unordered_map.hpp"
#include "src/aspartame/unordered_set.hpp"
#include "src/aspartame/vector.hpp"
#include "src/aspartame/view.hpp"
#include "src/aspartame/variant.hpp"

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <variant>

#include "src/aspartame/fluent.hpp"

class Foo {
  int *a;
  Foo() = delete;
  Foo(Foo &&) = delete;
};

class Foo2 {
public:
  explicit Foo2(int x) : value(x) { static_assert(!std::is_default_constructible_v<Foo2>); }
  Foo2() = delete;
  int value;
  bool operator==(const Foo2 &rhs) const { return value == rhs.value; }
  bool operator<(const Foo2 &rhs) const { return value < rhs.value; }
};

int main() {
  using namespace aspartame;

  std::variant<std::unique_ptr<Foo2>, int> v{1};
   (v ^ fold_total([](const std::unique_ptr<Foo2> &) { return "x->value"; }, [](int) { return "a"; })) ;
  v = std::make_unique<Foo2>(1);
   (v ^ fold_total([](const std::unique_ptr<Foo2> &) { return "x->value"; }, [](int) { return "a"; })) ;


  auto csv = R"(
    SensorID,Day1,Day2,Day3
    SensorA,100,150,130
    SensorB,90,120,110
    SensorC,200,230,210)";

  auto rows = (csv ^ lines()) //
              | collect([](auto l) { return l ^ is_blank() ? std::nullopt : std::optional{l ^ trim()}; });
  auto header = (rows | head_maybe()) ^ get_or_else("") ^ split(",");
  auto data = (rows | tail()                                                             //
               | map([](auto row) { return row ^ split(","); })                          //
               | group_map_reduce(                                                       //
                     [](auto &x) { return x[0]; },                                       //
                     [&](auto &xs) { return header | zip(xs) | drop(1) | to_vector(); }, //
                     [](auto l, auto r) { return l ^ concat(r); }))                      //
              ^ map_values([](auto vvs) {
                  return vvs | and_then([](auto xs) { return std::map<std::string, std::string>{xs.begin(), xs.end()}; });
                });

  for (auto [row, values] : data) {
    std::cout << row << " = " << (values | mk_string(", ", [](auto k, auto v) { return k + ":" + v; })) << "\n";
  }


  std::variant<int, std::string> foo{"a"};

  auto m = foo ^ fold_total([](int s){
    std::cout << s<<"\n";
    return 42;
  }, [](std::string s){
    std::cout << s<<"\n";
    return 0;
  } );

  auto xxx = foo ^ fold_partial(  [](std::string s){
                     std::cout << s<<"\n";
                     return 0;
                   } );

  std::cout << m <<"\n";

  std::cout << xxx.has_value() <<"\n";

  std::cout << (foo ^ get_maybe<int>()).value() <<"\n";


  foo ^ foreach_partial(  [](std::string s){
               std::cout << s<<"\n";
             } );

  return 0;

  auto U = std::optional{std::tuple<int, int>{42, 43}};

  std::optional a{42};

  std::vector xs{1, 2, 3};

  static_assert(std::is_same_v<decltype(xs.begin().operator*()), int &>);

  static_assert(is_iterable<decltype(xs)>, "a");


  auto vvv = xs | append(1) | concat(xs) | to_vector();
  for (auto &x : vvv) {
    std::cout << "v=" << x << "\n";
  }

  auto [aa, bb] = std::vector{1, 2, 3} | partition([](auto x) { return x < 2; });
  for (auto a_ : aa | tap_each([](auto x) { std::cout << "t=" << x << "\n"; })) {
    std::cout << "aa=" << a_ << "\n";
  }
  for (auto b_ : bb) {
    std::cout << "bb=" << b_ << "\n";
  }

  //  for(auto x : std::vector{1,2,3} |  filter([](auto x){return x< 2;})){
  //
  //  }

  //  static_assert(std::is_same_v<decltype(vvv), decltype(xs)>,"aa" );
  //
  //

  //  std::vector  {1, 2, 3};
  auto u = std::vector{1, 2, 3} ^ filter([](auto x) { return x > 2; });
  for (auto x : u) {
    std::cout << "x=" << x << "\n";
  }

  auto [l, r] = std::vector{1, 2, 3} ^ map([](auto x) { return x * 2; }) ^ partition([](auto x) { return x > 2; });
  for (auto x : l) {
    std::cout << "l=" << x << "\n";
  }
  for (auto x : r) {
    std::cout << "r=" << x << "\n";
  }

  auto p = (iterate(0, [](auto x) { return x + 1; })                             //
            | map([](int n) { return (n % 2 == 0 ? 1.0 : -1.0) / (2 * n + 1); }) //
            | take(10000000)                                                     //
            | reduce(std::plus<>())) ^
           fold([](auto x) { return x * 4; }, []() { return 0; });

  std::cout << "Pi=" << p << "\n";

  //  for(auto i : iterate(0, [](auto x) { return x+1; })
  //               | map([](int n) { return (n % 2 == 0 ? 1.0 : -1.0) / (2 * n + 1); })){
  //
  //    std::cout << "x = " << i << "\n";
  //  }

  auto mm = std::unordered_map<int, int>{{2, 1}, {3, 2}};
  for (auto [k, v] : mm | map([](auto x, auto y) {
                       return std::pair{x + x, y + 1};
                     }) | flat_map([](auto x) {
                       return std::vector{x, x};
                     }) | distinct_by([](auto x, auto ) { return x; })

  ) {
    std::cout << "kv=" << k << " = " << v << "\n";
  }

  auto lll = std::vector{1} ^ last_maybe();

  for (auto x : std::vector{std::string("a"), std::string("b"), std::string("a")} ^ distinct_by([](auto x) { return x.size(); })) {
    std::cout << "d=" << x << "\n";
  }

  //
  //  std::cout << " = " << (xs | append(10) | append(30) | concat(xs) | map([](auto x) { return x * 2; }) | filter([](auto x) { return x >
  //  6; }) | mk_string(",", "[", "]")) << std::endl;
  //
  //  std::optional<std::optional<int>> n = a ^ map([](auto x) { return std::optional{1}; });
  //
  //  a ^ filter([](auto x) { return true; })              //
  //      ^ map([](auto x) { return x; })                  //
  //      ^ map([](auto x) { return x * 2; })              //
  //      ^ bind([](auto x) { return std::optional{x}; })  //
  //      ^ map([](auto x) { return std::optional{"a"}; }) //
  //      ^ flatten()                                      //
  //      ^ zip_with_index()                               //
  //      ^ tap_each([](auto x, auto i) { std::cout << x << "\n"; }) ^ map([](auto x, auto i) {
  //          return std::tuple{x, i, "a"};
  //        }) ^
  //      for_each([](auto x, auto i, auto s) {});
  //
  //  std::cout << "r = " << (std::vector{10, 20, 30} ^ reduce([](auto l, auto r) { return l + r; }) ^ get_or_else(42)) << "\n";
  //  std::cout << "flr = " << (std::vector{10, 20, 30} ^ fold_left(std::string(), [](auto l, auto r) { return l + std::to_string(r); }))
  //            << "\n";
  //
  //  range::inclusive{0, 10}.tabulate<std::vector>([](auto i) { return i; }) ^ for_each([](auto x) {});
  //
  //  std::vector{1, 2} ^ group_by([](auto x) { return std::to_string(x); });
  //  std::vector{1, 2} ^ group_map([](auto x) { return std::to_string(x); }, [](auto x) { return std::pair(x, x); });
  //  std::vector{1, 2} ^
  //      group_map_reduce([](auto x) { return std::to_string(x); }, [](auto x) { return std::pair(x, x); }, [](auto x, auto y) { return x;
  //      });
  //
  //  std::unordered_map<int, int>{{1, 2}, {3, 4}} ^ map([](auto x, auto y) {
  //    return std::pair{x, y * 2};
  //  }) ^ map([](auto x) { return x; }) ^
  //      filter([](auto x, auto y) { return x + y > 5; }) ^
  //      bind([](auto x) {
  //        return std::unordered_map<std::string, decltype(x)>{{std::to_string(x.first + x.second), x}};
  //      })
  //      // ^ last_optional()                      //
  //      // ^ to_vector()
  //      ^ for_each([](auto i, auto x) { std::cout << ": " << i << " " << x.first << "," << x.second << "\n"; });
  //  std::cout << "----\n";
  //
  //  std::list{1, 2} ^ map([](auto x) { return x * 2; })         //
  //                                                              //      ^ bind([](auto x) {
  //                                                              //          return std::vector{x, x};
  //                                                              //        })                                   //
  //                                                              //      ^ filter([](auto x) { return x > 2; })                  //
  //      ^ zip_with_index()                                      //
  //      ^ map([](auto x) { return x; })                         //
  //                                                              // ^ last_optional()                      //
  //                                                              // ^ to_vector()
  //      ^ reverse() ^ sort_by([](auto i, auto x) { return i; }) //
  //      ^ for_each([](size_t i, int x) { std::cout << ": " << i << " " << x << "\n"; });
  //  std::cout << "----\n";

  return 0;
}
