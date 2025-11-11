#ifndef COMMON_HPP
#define COMMON_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Debug/Logger.hpp"

namespace boost {}

#define PIXF_ASSERT(expr, ...)                                                                                         \
    do {                                                                                                               \
        if (!(expr)) {                                                                                                 \
            const char *msg = __VA_ARGS__;                                                                             \
            PIXF_LOG_FATAL("Assertion failed: (" #expr ")", (msg[0] ? ", Message: \"" : ""), msg,                      \
                           "\", File: ", __FILE__, ", ", "Line: ", __LINE__, '\n');                                    \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (0)

namespace Pixf::Core {
    using namespace glm;
    using namespace boost;
} // namespace Pixf::Core

#endif // COMMON_HPP
