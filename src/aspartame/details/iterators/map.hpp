#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Function,      //
          typename U = std::decay_t<typename std::invoke_result_t<Function, typename details::value_type_of_t<InputIterator>>>>
class map_iterator : public fwd_iterator<map_iterator<InputIterator, Function, U>, U> {
  InputIterator it, end;
  ca_optional<Function> f;
  std::optional<U> current;
  [[nodiscard]] constexpr bool has_next() const { return f && it != end; }

public:
  constexpr map_iterator() = default;
  constexpr map_iterator(InputIterator begin, InputIterator end, Function function)
      : it(std::move(begin)), end(std::move(end)), f(function) {
    if constexpr (details::assert_non_void<decltype((*f)(*it))>()) {}
  }
  constexpr map_iterator &operator++() {
    ++it;
    return *this;
  }
  [[nodiscard]] constexpr const U &operator*() { return *(current = (*f)(*it)); }
  [[nodiscard]] constexpr bool operator==(const map_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
