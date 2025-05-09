#pragma once

#include "details/base.hpp"
#include "fluent.hpp"

#include <vector>

namespace aspartame {
template <typename T, typename Op>
#ifdef ASPARTAME_USE_CONCEPTS
  requires std::invocable<Op, const std::vector<T> &, tag>
#endif
auto operator^(const std::vector<T> &l, const Op &r) {
  return r(l, tag{});
}
} // namespace aspartame

#define ASPARTAME_IN_TYPE2(K, V) std::vector<std::pair<K, V>>
#define ASPARTAME_IN_TYPE1(C) std::vector<C>
#define ASPARTAME_OUT_TYPE std::vector
#define ASPARTAME_SET_LIKE false

#include "details/container1_template.hpp"
#include "details/sequence1_template.hpp"

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_SET_LIKE
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_IN_TYPE1
#undef ASPARTAME_IN_TYPE2
