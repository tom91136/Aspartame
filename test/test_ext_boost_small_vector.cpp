#include <aspartame/ext/boost.hpp>

#include "fixtures.hpp"
#include "test_base_includes.hpp"

namespace boost::container {
using ::operator<<;
template <typename T, std::size_t N, typename A, typename O>
std::ostream &operator<<(std::ostream &os, const small_vector<T, N, A, O> &xs) {
  os << "boost::small_vector{";
  for (std::size_t i = 0; i < xs.size(); ++i) {
    if (i > 0) os << ", ";
    os << xs[i];
  }
  return os << "}";
}
} // namespace boost::container

#define OP_ ^
#define TPE_GROUP "test_boost_small_vector"
#define TPE_NAME "boost::small_vector"
#define TPE_CTOR_IN(T) boost::container::small_vector<T, 4>
#define TPE_CTOR_OUT(T) boost::container::small_vector<T, 4>
#define TPE_CTOR_VAR_OUT(T) boost::container::small_vector<T, 4>
#define TPE_INIT_TO_CTOR_IN(x) x
#define TPE_INIT_SKIP(x) false
#define TPE_MANY_INIT 1

#include "templates/test_template_any.hpp"
#include "templates/test_template_container.hpp"
#include "templates/test_template_sequence.hpp"
#define TPE_INPLACE_SEQ 1
#include "templates/test_template_inplace.hpp"
