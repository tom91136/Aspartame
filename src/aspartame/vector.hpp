#pragma once

#include "details/base.hpp"
#include <vector>

#ifdef ASPARTAME_FINALISED
  #error "This header must be included before aspartame/fluent.hpp; \
consider adding a new line between this include and others to prevent reording by formatters."
#endif

namespace aspartame {
template <typename T, typename Op> auto operator^(const std::vector<T> &l, const Op &r) { return r(l); }
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
