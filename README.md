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

## Supported operations

```c++
Container<T> xs = {...};
OutContainer<T> ys = xs ^ Op;
```



