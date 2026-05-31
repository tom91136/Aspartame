#pragma once

#include <vector>

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Predicate,     //
          typename T = value_type_of_t<InputIterator>>
class chunk_by_iterator : public fwd_iterator<chunk_by_iterator<InputIterator, Predicate, T>, std::vector<T>, std::input_iterator_tag> {
  using OutT = std::vector<T>;
  InputIterator it, end;
  ca_optional<Predicate> p;
  std::optional<OutT> current;
  bool done = true;

  ASPARTAME_CONSTEXPR_ALLOC void build_next() {
    OutT chunk;
    chunk.push_back(*it);
    auto prev_val = *it;
    ++it;
    while (it != end && (*p)(prev_val, *it)) {
      chunk.push_back(*it);
      prev_val = *it;
      ++it;
    }
    current = std::move(chunk);
  }
  [[nodiscard]] constexpr bool has_next() const { return !done; }

public:
  constexpr chunk_by_iterator() = default;
  ASPARTAME_CONSTEXPR_ALLOC chunk_by_iterator(InputIterator begin, InputIterator end_, Predicate pred)
      : it(std::move(begin)), end(std::move(end_)), p(pred) {
    if (it != end) {
      build_next();
      done = false;
    }
  }
  ASPARTAME_CONSTEXPR_ALLOC chunk_by_iterator &operator++() {
    if (it == end) done = true;
    else build_next();
    return *this;
  }
  [[nodiscard]] constexpr const OutT &operator*() { return *current; }
  [[nodiscard]] constexpr bool operator==(const chunk_by_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
