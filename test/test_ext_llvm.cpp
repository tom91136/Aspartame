#include <type_traits>

#include <aspartame/ext/llvm.hpp>

#include "test_base_includes.hpp"

static_assert(aspartame::enable_pipe<llvm::SmallVectorImpl<int>>::value);
static_assert(aspartame::enable_pipe<llvm::SmallVector<int, 4>>::value);
static_assert(aspartame::enable_pipe<llvm::DenseSet<int>>::value);
static_assert(aspartame::enable_pipe<llvm::SetVector<int>>::value);
static_assert(aspartame::enable_pipe<llvm::DenseMap<int, int>>::value);
static_assert(aspartame::enable_pipe<llvm::MapVector<int, int>>::value);
static_assert(aspartame::enable_pipe<llvm::ArrayRef<int>>::value);
static_assert(aspartame::enable_pipe<llvm::StringRef>::value);
static_assert(aspartame::enable_pipe<llvm::SmallString<32>>::value);
static_assert(aspartame::enable_pipe<llvm::iterator_range<const int *>>::value);
static_assert(aspartame::enable_pipe<llvm::SmallDenseMap<int, int>>::value);
