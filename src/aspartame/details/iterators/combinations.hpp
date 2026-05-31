#pragma once

#include <numeric>
#include <vector>

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename T = value_type_of_t<InputIterator>>
class combinations_iterator : public fwd_iterator<combinations_iterator<InputIterator, T>, std::vector<T>, std::input_iterator_tag> {
  using OutT = std::vector<T>;
  std::vector<T> src;
  size_t k = 0;
  std::vector<size_t> idx;
  std::optional<OutT> current;
  bool done = true;

  [[nodiscard]] constexpr bool has_next() const { return !done; }

public:
  constexpr combinations_iterator() = default;
  ASPARTAME_CONSTEXPR_ALLOC combinations_iterator(InputIterator begin, InputIterator end, size_t k_) : src(begin, end), k(k_), idx(k_) {
    if (k > src.size()) return;
    std::iota(idx.begin(), idx.end(), size_t{0});
    done = false;
  }
  ASPARTAME_CONSTEXPR_ALLOC combinations_iterator &operator++() {
    size_t n = src.size();
    size_t i = k;
    while (i > 0 && idx[i - 1] == n - k + (i - 1))
      --i;
    if (i == 0) {
      done = true;
      return *this;
    }
    ++idx[i - 1];
    for (size_t j = i; j < k; ++j)
      idx[j] = idx[j - 1] + 1;
    return *this;
  }
  [[nodiscard]] ASPARTAME_CONSTEXPR_ALLOC const OutT &operator*() {
    OutT one;
    one.reserve(k);
    for (size_t i : idx)
      one.push_back(src[i]);
    current = std::move(one);
    return *current;
  }
  [[nodiscard]] constexpr bool operator==(const combinations_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
