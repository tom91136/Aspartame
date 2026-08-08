#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, //
          typename Function,      //
          typename T = typename details::value_type_of_t<InputIterator>, typename Reference = decltype(*std::declval<InputIterator &>())>
class tap_each_iterator
    : public fwd_iterator<tap_each_iterator<InputIterator, Function, T, Reference>, T, std::input_iterator_tag, Reference> {
  InputIterator it, end;
  ca_optional<Function> f;
  [[nodiscard]] constexpr bool has_next() const { return f && it != end; }

public:
  constexpr tap_each_iterator() = default;
  constexpr tap_each_iterator(InputIterator begin, InputIterator end, Function function)
      : it(std::move(begin)), end(std::move(end)), f(function) {}

  constexpr tap_each_iterator &operator++() {
    ++it;
    return *this;
  }
  [[nodiscard]] constexpr Reference operator*() {
    Reference value = *it;
    (*f)(value);
    return value;
  }
  [[nodiscard]] constexpr bool operator==(const tap_each_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
