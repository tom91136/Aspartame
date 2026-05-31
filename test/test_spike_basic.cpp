#include <catch2/catch_test_macros.hpp>

#include "aspartame/all.hpp"

using namespace aspartame;

TEST_CASE("unregistered types are not pipeable", "[spike][negative]") {
  struct NotRegistered {
    int begin() const { return 0; }
    int end() const { return 0; }
  };
  static_assert(!enable_pipe<NotRegistered>::value, "unregistered types must not be pipeable");
}
