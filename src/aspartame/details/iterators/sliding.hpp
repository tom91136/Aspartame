#pragma once

#include <deque>

#include "../base.hpp"
#include "iterator_base.hpp"

namespace aspartame::details {

template <typename InputIterator, template <typename...> typename C, typename WindowStorage, typename W = C<InputIterator, WindowStorage>>
class indexable_sliding_iterator
    : public fwd_iterator<indexable_sliding_iterator<InputIterator, C, WindowStorage, W>, W, std::input_iterator_tag> {
  InputIterator it, end;
  size_t size{}, step{};
  bool initial = true;
  WindowStorage storage;
  W window;
  [[nodiscard]] constexpr bool has_next() const { return it != end && (initial || step != 1 || static_cast<size_t>(end - it) >= size); }

public:
  constexpr indexable_sliding_iterator() = default;
  constexpr indexable_sliding_iterator(InputIterator begin, InputIterator end_, size_t size_, size_t step_, WindowStorage storage_)
      : it(std::move(begin)), end(std::move(end_)), size(size_), step(step_), storage(std::move(storage_)) {}
  constexpr indexable_sliding_iterator &operator++() {
    const auto remaining = static_cast<size_t>(end - it);
    if (initial && remaining < size) {
      initial = false;
      it = end;
      return *this;
    }
    initial = false;
    it += static_cast<typename std::iterator_traits<InputIterator>::difference_type>(std::min(step, remaining));
    return *this;
  }
  [[nodiscard]] constexpr const W &operator*() {
    const auto count = std::min(size, static_cast<size_t>(end - it));
    window = W{storage, it, it + static_cast<typename std::iterator_traits<InputIterator>::difference_type>(count)};
    return window;
  }
  [[nodiscard]] constexpr bool operator==(const indexable_sliding_iterator &that) const { return (!has_next() == !that.has_next()); }
};

template <typename InputIterator,                                        //
          template <typename...> typename C,                             //
          typename T = typename details::value_type_of_t<InputIterator>, //
          typename W = C<typename std::deque<T>::const_iterator>>
class sliding_iterator : public fwd_iterator<sliding_iterator<InputIterator, C, T, W>, W, std::input_iterator_tag> {
  InputIterator it, end;
  size_t size{}, step{};

  std::deque<T> window_buffer{};
  W window;
  [[nodiscard]] constexpr bool has_next() const { return !window_buffer.empty(); }

  ASPARTAME_CONSTEXPR_ALLOC void fill_window() {
    while (window_buffer.size() < size && it != end) {
      window_buffer.push_back(*it);
      ++it;
    }
  }

public:
  constexpr sliding_iterator() = default;
  ASPARTAME_CONSTEXPR_ALLOC sliding_iterator(InputIterator begin, InputIterator end_, size_t size_, size_t step_)
      : it(std::move(begin)), end(std::move(end_)), size(size_), step(step_) {
    fill_window();
  }
  ASPARTAME_CONSTEXPR_ALLOC sliding_iterator &operator++() {
    if (it == end) {
      window_buffer.clear();
      return *this;
    }
    size_t steps_to_advance = std::min(step, window_buffer.size());
    for (size_t i = 0; i < steps_to_advance; ++i)
      window_buffer.pop_front();
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
