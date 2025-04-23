#pragma once

#include "iterator_base.hpp"

#include <cstdint>

namespace aspartame::details {

enum class append_prepend_iterator_mode : uint8_t { append, prepend };

template <append_prepend_iterator_mode Mode,
          typename InputIterator, //
          typename T = typename details::value_type_of_t<InputIterator>>
class append_prepend_iterator : public fwd_iterator<append_prepend_iterator<Mode, InputIterator, T>, T> {
public:
private:
  InputIterator it, end;
  ca_optional<T> value{};
  //  append_prepend_iterator_mode mode = append_prepend_iterator_mode::append;
  bool use_value = false;
  [[nodiscard]] constexpr bool has_next() const { return value && (it != end || use_value); }

public:
  append_prepend_iterator() = default;
  append_prepend_iterator(InputIterator begin, InputIterator end, T value)
      : it(std::move(begin)), end(std::move(end)), value(std::move(value)), use_value(it == this->end) {
    if constexpr (Mode == append_prepend_iterator_mode::prepend) { use_value = true; }
  }
  constexpr append_prepend_iterator &operator++() {
    if (use_value) use_value = false;
    else {
      ++it;
      if constexpr (Mode == append_prepend_iterator_mode::append) {
        if (it == end) use_value = true;
      }
    }
    return *this;
  }
  [[nodiscard]] constexpr const T &operator*() { return use_value ? *value : *it; }
  [[nodiscard]] constexpr bool operator==(const append_prepend_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
