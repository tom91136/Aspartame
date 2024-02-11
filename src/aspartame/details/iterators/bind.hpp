#pragma once

#include "iterator_base.hpp"

namespace aspartame::details {

template <typename OuterIterator, //
          typename Function,      //
          typename InnerRange = typename std::invoke_result_t<Function, typename OuterIterator::value_type>,
          typename U = typename InnerRange::value_type>
class bind_iterator : public fwd_iterator<bind_iterator<OuterIterator, Function, InnerRange, U>, U> {
  struct State {
    ca_optional<Function> bind;
    OuterIterator outer_it, outer_end;
    std::shared_ptr<InnerRange> inner_container;
    typename InnerRange::const_iterator inner_it;
    constexpr State(Function f, OuterIterator begin, OuterIterator end)
        : bind(f), outer_it(std::move(begin)), outer_end(std::move(end)), //
          inner_container(std::make_shared<InnerRange>((*bind)(*outer_it))), inner_it(inner_container->begin()) {
      advance();
    }
    constexpr void advance() {
      while (outer_it != outer_end && inner_it == inner_container->end()) {
        ++outer_it;
        if (outer_it != outer_end) {
          inner_container = std::make_shared<InnerRange>((*bind)(*outer_it));
          inner_it = inner_container->begin();
          inner_container->end() = inner_container->end();
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
  constexpr bind_iterator() = default;
  constexpr bind_iterator(OuterIterator begin, OuterIterator end, Function f)
      : state(begin != end ? std::make_optional<State>(f, std::move(begin), std::move(end)) : std::nullopt) {}
  [[nodiscard]] constexpr const U &operator*() { return state->deref(); }
  constexpr bind_iterator &operator++() {
    state->next();
    return *this;
  }
  [[nodiscard]] constexpr bool operator==(const bind_iterator &that) const {
    return (this->state == that.state) || (!this->has_next() && !that.has_next());
  }
};

} // namespace aspartame::details
