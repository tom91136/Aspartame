#include <aspartame/ext/llvm.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace llvm {
using ::operator<<;
template <typename T, unsigned N> std::ostream &operator<<(std::ostream &os, const SmallVector<T, N> &xs) {
  os << "llvm::SmallVector{";
  for (std::size_t i = 0; i < xs.size(); ++i) {
    if (i > 0) os << ", ";
    os << xs[i];
  }
  return os << "}";
}
} // namespace llvm

#define OP_ ^
#define TPE_GROUP "test_llvm_small_vector"
#define TPE_NAME "llvm::SmallVector"
#define TPE_CTOR_IN(T) llvm::SmallVector<T, 4>
#define TPE_CTOR_OUT(T) llvm::SmallVector<T, 4>
#define TPE_CTOR_VAR_OUT(T) llvm::SmallVector<T, 4>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_sequence.hpp"
#define TPE_INPLACE_SEQ 1
#include "templates/test_template_inplace.hpp"
