#pragma once

#include "iterator_base.hpp"

#include <memory>
#include <utility>

namespace aspartame::details {

template <typename T> class erased_iterator : public fwd_iterator<erased_iterator<T>, T> {
  struct base {
    virtual ~base() = default;
    [[nodiscard]] virtual std::unique_ptr<base> clone() const = 0;
    [[nodiscard]] virtual const void *token() const = 0;
    virtual void increment() = 0;
    [[nodiscard]] virtual const T &deref() = 0;
    [[nodiscard]] virtual bool equals(const base &other) const = 0;
    [[nodiscard]] virtual bool is_end() const = 0;
  };

  template <typename Iterator> struct erased final : base {
    Iterator it, end;
    erased(Iterator begin, Iterator finish) : it(std::move(begin)), end(std::move(finish)) {}

    [[nodiscard]] std::unique_ptr<base> clone() const override { return std::make_unique<erased>(*this); }
    [[nodiscard]] const void *token() const override {
      static const int key = 0;
      return &key;
    }
    void increment() override { ++it; }
    [[nodiscard]] const T &deref() override { return *it; }
    [[nodiscard]] bool equals(const base &other) const override {
      if (other.token() == token()) return it == static_cast<const erased &>(other).it;
      return this->is_end() && other.is_end();
    }
    [[nodiscard]] bool is_end() const override { return it == end; }
  };

  std::unique_ptr<base> impl;

public:
  erased_iterator() = default;

  template <typename Iterator>
  erased_iterator(Iterator begin, Iterator end) : impl(std::make_unique<erased<Iterator>>(std::move(begin), std::move(end))) {}

  erased_iterator(const erased_iterator &other) : impl(other.impl ? other.impl->clone() : nullptr) {}

  erased_iterator &operator=(const erased_iterator &other) {
    if (this != &other) impl = other.impl ? other.impl->clone() : nullptr;
    return *this;
  }

  erased_iterator(erased_iterator &&) noexcept = default;
  erased_iterator &operator=(erased_iterator &&) noexcept = default;

  [[nodiscard]] constexpr const T &operator*() { return impl->deref(); }
  constexpr erased_iterator &operator++() {
    impl->increment();
    return *this;
  }
  [[nodiscard]] constexpr bool operator==(const erased_iterator &that) const {
    const bool lhs_is_end = !impl || impl->is_end();
    const bool rhs_is_end = !that.impl || that.impl->is_end();
    if (lhs_is_end || rhs_is_end) return lhs_is_end == rhs_is_end;
    return impl->equals(*that.impl);
  }
};

} // namespace aspartame::details
