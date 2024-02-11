#pragma once

#include <memory>
#include <optional>

namespace aspartame::details {

template <typename T> class ca_optional {
  std::optional<T> storage;

public:
  ca_optional() = default;
  explicit ca_optional(T f) : storage(std::move(f)) { static_assert(std::is_copy_assignable_v<ca_optional>); }
  ca_optional(const ca_optional &other) {
    if (other.storage) storage.emplace(*other);
  }
  ca_optional &operator=(const ca_optional &other) {
    if (this != &other) {
      storage.reset();
      if (other.storage) storage.emplace(*other);
    }
    return *this;
  }
  constexpr ca_optional &operator=(T &&t) noexcept {
    storage.operator=(std::forward<T &&>(t));
    return *this;
  }
  constexpr T &operator*() & noexcept { return storage.operator*(); }
  constexpr const T &operator*() const & noexcept { return storage.operator*(); }
  constexpr const T *operator->() const noexcept { return storage.operator->(); }
  constexpr explicit operator bool() const noexcept { return storage.operator bool(); }
};

template <typename Derived, typename T> struct fwd_iterator {
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = const T *;
  using reference = const T &;
  using iterator_category = std::forward_iterator_tag;
  constexpr fwd_iterator() {
    static_assert(std::is_same_v<                                    //
                      decltype(std::declval<Derived>().operator*()), //
                      reference>,
                  "Derived class must implement 'T operator*()'");
    static_assert(std::is_same_v<                            //
                      decltype(++std::declval<Derived &>()), //
                      Derived &>,
                  "Derived class must implement 'Derived& operator++()'");
    static_assert(std::is_same_v<                                                                   //
                      decltype(std::declval<const Derived &>() == std::declval<const Derived &>()), //
                      bool>,
                  "Derived class must implement 'bool operator==(const Derived&) const'");

    static_assert(std::is_default_constructible_v<Derived>, "LegacyForwardIterator: Derived must be default constructible");

    static_assert(std::is_copy_constructible_v<Derived>, "LegacyIterator: Derived must be copy constructible");
    static_assert(std::is_copy_assignable_v<Derived>, "LegacyIterator: Derived must be copy assignable");
    static_assert(std::is_destructible_v<Derived>, "LegacyIterator: Derived must destructible");
  }
  constexpr Derived operator++(int) { // NOLINT(*-dcl21-cpp)
    Derived d = *static_cast<Derived *>(this);
    static_cast<Derived *>(this)->operator++();
    return d;
  }

  [[nodiscard]] constexpr bool operator!=(const Derived &that) const { return !static_cast<const Derived *>(this)->operator==(that); }
};
} // namespace aspartame::details