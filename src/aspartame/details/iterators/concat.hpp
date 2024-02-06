#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename Iter1, //
          typename Iter2, //
          typename T = std::add_const_t<typename Iter1::value_type>>
class concat_iterator : public fwd_iterator<concat_iterator<Iter1, Iter2>, T> {
  static_assert(std::is_same_v<      //
                    std::remove_const_t<T>, //
                    std::remove_const_t<typename std::iterator_traits<Iter2>::value_type>>,
                "Iterators must be of the same value type");
  Iter1 iter1_begin_, iter1_end_;
  Iter2 iter2_begin_, iter2_end_;
  bool is_first_iter;

public:
  constexpr concat_iterator() = default;
  constexpr concat_iterator(Iter1 iter1_begin, Iter1 iter1_end, Iter2 iter2_begin, Iter2 iter2_end)
      : iter1_begin_(iter1_begin), iter1_end_(iter1_end), //
        iter2_begin_(iter2_begin), iter2_end_(iter2_end), is_first_iter(true) {}

  constexpr concat_iterator &operator++() {
    if (is_first_iter) {
      if (++iter1_begin_ == iter1_end_) { is_first_iter = false; }
    } else {
      ++iter2_begin_;
    }
    return *this;
  }

  [[nodiscard]] constexpr T &operator*() const { return is_first_iter ? *iter1_begin_ : *iter2_begin_; }
  [[nodiscard]] constexpr bool operator==(const concat_iterator &other) const {
    // Equal if both are at the end or if the current positions are the same
    bool atEnd1 = iter1_begin_ == iter1_end_ && other.iter1_begin_ == other.iter1_end_;
    bool atEnd2 = iter2_begin_ == iter2_end_ && other.iter2_begin_ == other.iter2_end_;
    bool samePos = (iter1_begin_ == other.iter1_begin_ && iter2_begin_ == other.iter2_begin_);
    return (atEnd1 && atEnd2) || samePos;
  }
};

} // namespace aspartame::details
