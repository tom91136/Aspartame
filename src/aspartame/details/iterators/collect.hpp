#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, typename Function,
          // XXX see map_iterator: derive U from `decltype(*it)`, not value_type_of_t
          typename U = typename std::invoke_result_t<Function, decltype(*std::declval<InputIterator>())>::value_type>
class collect_iterator : public fwd_iterator<collect_iterator<InputIterator, Function, U>, U, std::input_iterator_tag> {
  InputIterator it, end;
  ca_optional<Function> f;
  std::optional<U> current;
  [[nodiscard]] constexpr bool has_next() const { return f && it != end; }
  constexpr void advance() {
    while (it != end) {
      if (auto x = (*f)(*it); x) {
        current = *x;
        return;
      }
      ++it;
    }
    current.reset();
  }

public:
  constexpr collect_iterator() = default;
  constexpr collect_iterator(InputIterator begin, InputIterator end, Function function)
      : it(std::move(begin)), end(std::move(end)), f(function) {
    advance();
  }
  constexpr collect_iterator &operator++() {
    ++it;
    advance();
    return *this;
  }
  [[nodiscard]] constexpr const U &operator*() { return *current; }
  [[nodiscard]] constexpr bool operator==(const collect_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
