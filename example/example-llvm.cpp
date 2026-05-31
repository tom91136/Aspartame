#include <iostream>
#include <string>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include "aspartame/all.hpp"
#include "aspartame/ext/llvm.hpp"

using namespace aspartame;

int main() {
  llvm::SmallVector<int, 8> xs{4, 2, 3, 1, 5};
  const auto doubled = xs                                    //
                       ^ filter([](int x) { return x % 2; }) //
                       ^ map([](int x) { return x * 2; })    //
                       ^ sort();
  std::cout << "doubled-odds=" << (doubled ^ mk_string("[", ", ", "]")) << "\n";

  int backing[]{10, 20, 30, 40, 50};
  llvm::ArrayRef<int> ref{backing, 5};
  const auto sum = ref ^ fold_left(0, [](int a, int b) { return a + b; });
  std::cout << "sum=" << sum << "\n";

  llvm::StringRef hello = "Hello, World";
  const auto vowels = hello //
                      ^ count([](char c) { return std::string("aeiouAEIOU").find(c) != std::string::npos; });
  std::cout << "vowels=" << vowels << "\n";

  llvm::DenseMap<int, std::string> dm;
  dm.insert({1, "one"});
  dm.insert({2, "two"});
  dm.insert({3, "three"});
  const auto sorted_keys = (dm ^ keys() ^ to_vector()) ^ sort();
  std::cout << "keys=" << (sorted_keys ^ mk_string("[", ", ", "]")) << "\n";
  const auto looked_up = dm ^ get_maybe(2) ^ get_or_else(std::string{"<missing>"});
  std::cout << "lookup(2)=" << looked_up << "\n";

  const auto labels = xs                                                       //
                      ^ map([](int x) { return "item-" + std::to_string(x); }) //
                      ^ to_vector();
  std::cout << "labels=" << (labels ^ mk_string("[", ", ", "]")) << "\n";

  return 0;
}
