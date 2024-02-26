#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

enum class drop_take_iterator_mode : uint8_t { drop_while_true, take_while_true };

template <drop_take_iterator_mode Mode,
          typename InputIterator, //
          typename Predicate,     //
          typename T = typename details::value_type_of_t<InputIterator>>
class drop_take_iterator : public fwd_iterator<drop_take_iterator<Mode, InputIterator, Predicate, T>, T> {
  InputIterator it, end;
  ca_optional<Predicate> p;
  bool has_more{};
  [[nodiscard]] constexpr bool has_next() const {
    if constexpr (Mode == drop_take_iterator_mode::take_while_true) {
      return p && it != end && has_more;
    } else {
      return p && it != end;
    }
  }

public:
  constexpr drop_take_iterator() = default;
  constexpr drop_take_iterator(InputIterator begin, InputIterator end, Predicate predicate)
      : it(std::move(begin)), end(std::move(end)), p(predicate) {
    if constexpr (details::assert_predicate<decltype((*p)(*it))>()) {}
    if constexpr (Mode == drop_take_iterator_mode::drop_while_true) {
      while (it != this->end && (*p)(*it))
        ++it;
    } else has_more = it != this->end && (*p)(*it);
  }
  constexpr drop_take_iterator &operator++() {
    ++it;
    if constexpr (Mode == drop_take_iterator_mode::take_while_true) {
      if (it != end && !(*p)(*it)) {
        it = end;
        has_more = false;
      }
    }
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return *it; }
  [[nodiscard]] constexpr bool operator==(const drop_take_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
