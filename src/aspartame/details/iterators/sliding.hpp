#pragma once

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator,                          //
          template <typename...> typename C,               //
          typename T = typename InputIterator::value_type, //
          typename W = C<typename std::deque<T>::const_iterator>>
class sliding_iterator : public fwd_iterator<sliding_iterator<InputIterator, C, T, W>, W> {
  InputIterator it, end;
  size_t idx{}, size{}, step{};

  std::deque<T> window_buffer{};
  W window;
  [[nodiscard]] constexpr bool has_next() const { return !window_buffer.empty(); }

  constexpr void fill_window() {
    while (window_buffer.size() < size && it != end) {
      window_buffer.push_back(*it);
      ++it;
    }
  }

public:
  constexpr sliding_iterator() = default;
  constexpr sliding_iterator(InputIterator begin, InputIterator end, size_t size, size_t step)
      : it(std::move(begin)), end(std::move(end)), idx(0), size(size), step(step) {
    fill_window();
  }
  constexpr sliding_iterator &operator++() {
    if (it == end) {
      window_buffer.clear();
      return *this;
    }
    // Move the start of the window by step_size elements

    size_t steps_to_advance = std::min(step, window_buffer.size());
    for (size_t i = 0; i < steps_to_advance; ++i) {
      if (!window_buffer.empty()) {
        window_buffer.pop_front(); // Remove elements from the front according to step_size
      }
    }
    // handle step > size which skips ahead
    for (size_t i = size; i < step && it != end; ++i, ++it) {}
    fill_window();
    return *this;
  }
  [[nodiscard]] constexpr const W &operator*() {
    window = W{window_buffer.begin(), window_buffer.begin() + std::min(window_buffer.size(), size)};
    return window;
  }

  [[nodiscard]] constexpr bool operator==(const sliding_iterator &that) const { return (!this->has_next() == !that.has_next()); }
};

} // namespace aspartame::details
