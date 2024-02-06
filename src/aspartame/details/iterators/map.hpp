#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Function,      //
          typename U = std::add_const_t<typename std::invoke_result_t<Function, typename InputIterator::value_type>>>
class map_iterator : public fwd_iterator<map_iterator<InputIterator, Function>, U> {
  ca_optional<Function> f;
  InputIterator it, end;
  std::optional<std::remove_const_t<U>> current;
  [[nodiscard]] constexpr bool has_next() const { return f && it != end; }

public:
  constexpr map_iterator() = default;
  constexpr map_iterator(InputIterator current, InputIterator end, Function function)
      : f(function), it(std::move(current)), end(std::move(end)) {}
  constexpr map_iterator &operator++() {
    ++it;
    return *this;
  }
  [[nodiscard]] constexpr U &operator*() { return *(current = (*f)(*it)); }
  [[nodiscard]] constexpr bool operator==(const map_iterator &that) const {
    return (it == that.it) || (!this->has_next() == !that.has_next());
  }
};

} // namespace aspartame::details
