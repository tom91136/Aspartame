#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename T = value_type_of_t<InputIterator>>
class permutations_iterator : public fwd_iterator<permutations_iterator<InputIterator, T>, std::vector<T>, std::input_iterator_tag> {
  using OutT = std::vector<T>;
  std::vector<T> src;
  std::vector<size_t> idx;
  std::optional<OutT> current;
  bool done = true;

  [[nodiscard]] constexpr bool has_next() const { return !done; }

public:
  constexpr permutations_iterator() = default;
  ASPARTAME_CONSTEXPR_ALLOC permutations_iterator(InputIterator begin, InputIterator end) : src(begin, end), idx(src.size()) {
    std::iota(idx.begin(), idx.end(), size_t{0});
    done = false;
  }
  ASPARTAME_CONSTEXPR_ALLOC permutations_iterator &operator++() {
    if (!std::next_permutation(idx.begin(), idx.end())) done = true;
    return *this;
  }
  [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC const OutT &operator*() {
    OutT one;
    one.reserve(src.size());
    for (size_t i : idx)
      one.push_back(src[i]);
    current = std::move(one);
    return *current;
  }
  [[nodiscard]] constexpr bool operator==(const permutations_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
