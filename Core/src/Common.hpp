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

#if defined(_WIN32) || defined(_WIN64)
#if defined(PIXF_EXPORTS)
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif
#else
#if defined(PIXF_EXPORTS)
#define PIXF_API
#else
#define PIXF_API
#endif
#endif

namespace Pixf::Core {
    using namespace glm;
    using namespace boost;
} // namespace Pixf::Core

#endif // COMMON_HPP
