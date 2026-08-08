# Aspartame

> Aspartame is an artificial non-saccharide sweetener 200 times sweeter than sucrose and is commonly
> used as a sugar substitute in foods and beverages.

Aspartame is a C++17 header-only library that provides a C++20 range-like experience over a wider
range of containers and with a richer set of operations.
The operations are designed specifically to be chainable (i.e., fluent) with operations inspired by
high-productivity languages like Scala and Haskell.


### Feature highlights

* C++ >= 17
* Rich container support:
    * `std::optional` as a 1-element container: support for all container methods
    * `std::string` as a char container: support for all container methods + extra string methods
    * `std::vector`, `std::array`, `std::deque`, `std::list`
    * `std::set`, `std::unordered_set`
    * `std::map`, `std::unordered_map`
    * `std::variant`, `std::expected` (C++23) and `tl::expected`
    * Optional ext bridges for Boost, abseil, parallel-hashmap, tsl, LLVM ADT, and Qt containers
* Two modes: immediate (strict, allocating) and views (lazy via iterators)
* Lightweight: doesn't pull in all supported container headers at once
* Automatic application of `std::pair`/`std::tuple` values in lambdas
* Readable error messages via limited SFINAE and early dispatch (`if constexpr`)
* CI matrix covers GCC 9-15, Clang 11-21, MSVC (Windows), Apple Clang (macOS), NVHPC, on x86_64 and aarch64

## Quick start

C++ algorithms are designed for composition by index, not by data flow:

```c++
std::vector<int> xs = {4, 1, 3, 2, 0};
xs.erase(std::remove_if(xs.begin(), xs.end(), [](auto x) { return x > 5; }), xs.end());
std::sort(xs.begin(), xs.end());
xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
```

The same pipeline in Aspartame:

```c++
auto xs = std::vector<int>{4, 1, 3, 2, 0} ^ filter([](auto x) { return x > 5; }) ^ sort() ^ distinct();
```

`^` is the eager pipe; `|` is the lazy view pipe.

### Mutating through the chain (`^` vs `|`)

`^` takes the source by const-ref (every downstream lambda sees `const T&`); `|` takes it by forwarding-ref so a non-const lvalue source stays non-const through the chain. Use `|` whenever the chain needs to mutate or move from the source.

```c++
std::vector<std::unique_ptr<Device>> devices = enumerate();
auto matches = devices                                            //
               | map([](auto &d) { return std::ref(d); })         // reference_wrapper<unique_ptr<Device>>
               | filter([&](auto rw) { return pred(*rw.get()); }) //
               | to_vector();
currentDevice = std::move(matches.front().get());
```

`as_ref()` / `as_cref()` are shorthand for `map(std::ref)` / `map(std::cref)`; `as_ref()` only works under `|` for the same reason.

Reference-preserving lazy adapters (`filter`, `take`, `drop`, `slice`, `distinct`, `tap_each`, and `zip`) now expose the
underlying elements directly. This means a pipeline can mutate an lvalue source without an `as_ref()` detour:

```c++
devices | filter(is_active) | for_each([](Device &device) { device.refresh(); });
```

`find_ref(predicate)` and `find_cref(predicate)` return an optional `reference_wrapper` when copying the found value is
undesirable. Lazy `zip` yields a pair of references, including for move-only values. Lazy `concat`, `zip`, and `cross`
borrow an lvalue right-hand container and own an rvalue right-hand container, so `xs | concat(make_values())` is safe to
store. They also accept an optional right-hand value directly as a zero-or-one collection.

### Non-owning strings and parsing

`std::string_view` supports the string and sequence pipe syntax. Shape-preserving operations such as `trim`, `slice`,
`take`, `drop`, `split`, `split_once`, `rsplit_once`, `lines`, and `words` return views into the original character storage.
The source storage must therefore outlive every returned view. Operations that transform characters, such as `to_upper`,
return an owning `std::string`.

```c++
std::string_view line = "  name=value  ";
auto pair = line ^ trim() ^ split_once('=');
// optional<pair<string_view, string_view>>{"name", "value"}
```

`split_once` and `rsplit_once` are also available for owning strings. `glob_matches(pattern)` and
`glob_matches_ignore_case(pattern)` implement full-string `*` and `?` wildcard matching without regular expressions.
Owning results from a custom `basic_string_view<Char, Traits>` retain its character traits; a view has no allocator to
propagate, so the result uses the corresponding `basic_string` default allocator.

### Sources and focused terminals

`unfold(initial, successor)` (also named `iterate_maybe`) creates a lazy source containing the initial value and repeatedly
applies a successor returning `std::optional<T>` until it returns empty. `istream_lines(stream)` exposes a single-pass lazy
line source. `istream_split(stream, delimiter)` reads other character-delimited streams, while
`istream_lines_with_position(stream)` also yields each post-read stream position for parsers that need to seek backwards.

`collect_to<Container>(f)` combines optional-producing mapping and construction of a chosen output container.
`append_to(output)` appends a pipeline directly to an existing container; `move_to(output)` explicitly consumes its mutable
source. The source and destination must not alias; direct self-aliasing is diagnosed, while aliasing through a view remains
the caller's responsibility. `distinct_by_if(predicate, key)` only applies uniqueness tracking to selected elements.

Lazy `sliding` over a random-access source produces indexable windows with `empty()`, `size()`, and `operator[]` without
copying their elements. Windows retain an owned temporary source and otherwise observe their lvalue source. Other iterator
categories use a buffered indexable window whose contents remain valid only until the window iterator advances.

## Supported operations

```c++
Container<T> xs = {...};
OutContainer<T> ys = xs ^ Op;
```
