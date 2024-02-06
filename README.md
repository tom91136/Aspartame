# Aspartame

> Aspartame is an artificial non-saccharide sweetener 200 times sweeter than sucrose and is commonly
> used as a sugar substitute in foods and beverages.

Aspartame is a C++17 header-only library that provides a C++20 range-like experience over a wider
range of containers and with a richer set of operations.
The operations are designed specifically to be chainable (i.e., fluent) with operations inspired by
high-productivity languages like Scala and Haskell.


### Feature highlights

* C++ \>= 17
* Rich container support:
    * `std::optional` as a 1-element container: support for all container methods
    * `std::string` as a char container: support for all container methods + extra string methods
    * `std::vector`, `std::array`, `std::deque`
    * `std::set`, `std::unordered_set`
    * `std::map`, `std::unordered_map`
* Two modes: immediate (strict and allocating) and views (lazy via iterators) 
* Lightweight: Doesn't pull in all supported container headers at once
* Automatic application of `std::pair`/`std::tuple` values in lambdas
* Readable error messages with limited use of SFINAE and early dispatch (`if constexpr`) to signal
  compile errors
* TODO Tested with GCC X, Clang X, VS X
* Correct IntelliSense and type information in CLion, Clangd-based IDES, and VS2019

```c++



```

## Quick start

C++ algorithms in many cases are designed for composition:

```c++
std::vector<int> xs = {4, 1, 3, 2, 0};
xs.erase(std::remove_if(xs.begin(), xs.end(), [](auto x) { return val > 5; }), xs.end());
std::sort(xs.begin(), xs.end());
std::unique(xs.begin(), xs.end());
```

While this is excellent for performance, productivity suffers a bit; here's the same with Aspartame:

```c++
auto xs = std::vector<int>{4, 1, 3, 2, 0} | filter([](auto x) { return val > 5; }) | unique();
```

## Supported operations

Syntax:

```c++
Container<T> xs = {...};
OutCountainer<T> ys = xs | Op;
```



