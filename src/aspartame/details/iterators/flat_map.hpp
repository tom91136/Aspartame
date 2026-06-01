#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename OuterIterator, //
          typename Function,      //
          // XXX see map_iterator: derive InnerRange from `decltype(*it)`, not value_type_of_t
          typename InnerRange = typename std::invoke_result_t<Function, decltype(*std::declval<OuterIterator>())>,
          typename U = typename InnerRange::value_type>
class flat_map_iterator : public fwd_iterator<flat_map_iterator<OuterIterator, Function, InnerRange, U>, U, std::input_iterator_tag> {
  struct State {
    ca_optional<Function> flat_map;
    OuterIterator outer_it, outer_end;
    std::shared_ptr<InnerRange> inner_container;
    typename InnerRange::const_iterator inner_it;
    ASPARTAME_CONSTEXPR_ALLOC State(Function f, OuterIterator begin, OuterIterator end)
        : flat_map(f), outer_it(std::move(begin)), outer_end(std::move(end)), //
          inner_container(std::make_shared<InnerRange>((*flat_map)(*outer_it))), inner_it(inner_container->begin()) {
      advance();
    }
    ASPARTAME_CONSTEXPR_ALLOC void advance() {
      while (outer_it != outer_end && inner_it == inner_container->end()) {
        ++outer_it;
        if (outer_it != outer_end) {
          inner_container = std::make_shared<InnerRange>((*flat_map)(*outer_it));
          inner_it = inner_container->begin();
        }
      }
    }
    constexpr void next() {
      ++inner_it;
      advance();
    }
    [[nodiscard]] constexpr bool has_next() const { return inner_it != inner_container->end(); }
    [[nodiscard]] constexpr const U &deref() { return *inner_it; }
    [[nodiscard]] constexpr bool operator==(const State &that) const { return inner_it == that.inner_it; }
  };
  std::optional<State> state{};
  [[nodiscard]] constexpr bool has_next() const { return state && state->has_next(); }

public:
  constexpr flat_map_iterator() = default;
  constexpr flat_map_iterator(OuterIterator begin, OuterIterator end, Function f)
      : state(begin != end ? std::make_optional<State>(f, std::move(begin), std::move(end)) : std::nullopt) {}
  [[nodiscard]] constexpr const U &operator*() { return state->deref(); }
  constexpr flat_map_iterator &operator++() {
    state->next();
    return *this;
  }
  [[nodiscard]] constexpr bool operator==(const flat_map_iterator &that) const {
    return (this->state == that.state) || (!this->has_next() && !that.has_next());
  }
};

} // namespace aspartame::details
