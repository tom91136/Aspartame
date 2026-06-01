#pragma once
#ifndef ASPARTAME_SHOW_STREAM
#define ASPARTAME_SHOW_STREAM
#endif
#include "aspartame/all.hpp"

// XXX Workaround for Catch2 + clang + libstdc++ 14 + c++23: optional's <=> rewrite
// probes std::tuple_size<Catch::Decomposer> with a hard error instead of SFINAE.
// See https://github.com/catchorg/Catch2/issues/2923 and llvm/llvm-project#113087
#include <cstddef>
#include <tuple>
namespace Catch { struct Decomposer; }
namespace std {
template <> struct tuple_size<Catch::Decomposer> : integral_constant<size_t, 1> {};
} // namespace std
