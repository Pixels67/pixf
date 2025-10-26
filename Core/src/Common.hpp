#ifndef COMMON_HPP
#define COMMON_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define PIXF_ASSERT(expr, ...)                                                                                              \
    do {                                                                                                               \
        if (!(expr)) {                                                                                                 \
            const char *msg = __VA_ARGS__;                                                                             \
            std::cerr << "Assertion failed: (" #expr ")" << (msg[0] ? ", Message: " : "") << msg                       \
                      << "\nFile: " << __FILE__ << ", "                                                                \
                      << "Function: " << __FUNCTION__ << ", "                                                          \
                      << "Line: " << __LINE__ << '\n';                                                                 \
            std::terminate();                                                                                          \
        }                                                                                                              \
    } while (0)

namespace Pixf::Core {
    using namespace glm;
} // namespace Pixf::Core

#endif // COMMON_HPP
