#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "aspartame/all.hpp"

using namespace aspartame;

#ifndef ASPARTAME_TYPE
  #define ASPARTAME_TYPE 0
#endif
#ifndef ASPARTAME_OP
  #define ASPARTAME_OP 0
#endif

#if ASPARTAME_TYPE == 0
using Cont = std::vector<int>;
#elif ASPARTAME_TYPE == 1
using Cont = std::deque<int>;
#elif ASPARTAME_TYPE == 2
using Cont = std::list<int>;
#elif ASPARTAME_TYPE == 3
using Cont = std::set<int>;
#elif ASPARTAME_TYPE == 4
using Cont = std::unordered_set<int>;
#endif

int main() {
#if ASPARTAME_OP == 10
  std::map<int, int> m{{1, 1}};
  m ^= keys();
  (void)m;
#elif ASPARTAME_OP == 11
  struct Unregistered {};
  Unregistered u;
  (void)(u ^ filter([](auto) { return true; }));
#elif ASPARTAME_OP == 12
  std::map<int, int> m{{1, 1}};
  (void)(m ^ get_or_emplace(2, [](int k) { return k; }));
#elif ASPARTAME_OP == 13
  std::map<int, int> m{{1, 1}};
  m ^= map_values([](int v) { return std::to_string(v); });
  (void)m;
#elif ASPARTAME_OP == 99
  std::vector<int> v{3, 1, 2};
  v ^= filter([](int x) { return x > 1; });
  v ^= sort();
  v ^= map([](int x) { return x * 2; });
  (void)v;
#else
  Cont c{1, 2, 3};
  #if ASPARTAME_OP == 0
  c ^= map([](int) { return std::string{}; });
  #elif ASPARTAME_OP == 1
  c ^= fold_left(0, [](int a, int b) { return a + b; });
  #elif ASPARTAME_OP == 2
  c ^= count([](int x) { return x > 0; });
  #elif ASPARTAME_OP == 3
  c ^= exists([](int x) { return x > 0; });
  #elif ASPARTAME_OP == 4
  c ^= mk_string(",");
  #elif ASPARTAME_OP == 5
  c ^= map([](int x) { return static_cast<long>(x); });
  #endif
  (void)c;
#endif
}
