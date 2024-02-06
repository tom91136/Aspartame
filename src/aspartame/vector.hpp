#pragma once

#include "details/base.hpp"
#include <vector>

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp"
#endif

#define ASPARTAME_VECTOR(T, op, ...)                                                                                                       \
  if constexpr (is_vector<T>) { return vector_##op(__VA_ARGS__); }

namespace aspartame {
template <typename T, typename Op> auto operator|(const std::vector<T> &l, const Op &r) { return r(l); }
} // namespace aspartame

#define ASPARTAME_PREFIX(name) vector_##name
#define ASPARTAME_OUT_TYPE std::vector
#define ASPARTAME_SET_LIKE false

#include "details/container1_template.hpp"
#include "details/sequence1_template.hpp"

#include "details/nop/map_template.hpp"
#include "details/nop/optional_template.hpp"
#include "details/nop/string_template.hpp"

#undef ASPARTAME_PREFIX
#undef ASPARTAME_OUT_TYPE
#undef ASPARTAME_SET_LIKE
