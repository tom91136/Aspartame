#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

enum class append_prepend_iterator_mode : uint8_t { append, prepend };

template <typename InputIterator, //
          typename T = std::add_const_t<typename InputIterator::value_type>>
class append_prepend_iterator : public fwd_iterator<append_prepend_iterator<InputIterator>, T> {
public:
private:
  InputIterator iter_current_, iter_end_;
  append_prepend_iterator_mode mode_ = append_prepend_iterator_mode::append;
  ca_optional<std::remove_const_t<T>> value_;
  bool use_value = false;

  static_assert(std::is_copy_assignable_v<InputIterator>);
  static_assert(std::is_copy_assignable_v<append_prepend_iterator_mode>);
  static_assert(std::is_copy_assignable_v<decltype(value_)>);

public:
  append_prepend_iterator() = default;
  append_prepend_iterator(InputIterator begin, InputIterator end, append_prepend_iterator_mode mode, T value)
      : iter_current_(begin), iter_end_(end), mode_(mode), value_(value), use_value(mode == append_prepend_iterator_mode::prepend) {}

  constexpr append_prepend_iterator &operator++() {
    if (use_value) use_value = false;
    else {
      ++iter_current_;
      if (iter_current_ == iter_end_) use_value = true;
    }
    return *this;
  }

  [[nodiscard]] constexpr T &operator*() const { return use_value ? *value_ : *iter_current_; }
  [[nodiscard]] constexpr bool operator==(const append_prepend_iterator &other) const {
    bool samePos = iter_current_ == other.iter_current_;
    bool sameExtra = use_value == other.at_extra_;
    bool same_mode = mode_ == other.mode_;

    return samePos && sameExtra && same_mode;
  }
};

} // namespace aspartame::details
