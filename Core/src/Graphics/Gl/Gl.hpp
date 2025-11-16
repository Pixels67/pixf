#ifndef PIXF_GL_HPP
#define PIXF_GL_HPP

#include <glad/glad.h>
// ReSharper disable once CppWrongIncludesOrder
#include <GLFW/glfw3.h>

#include "Debug/Logger.hpp"

namespace Pixf::Core::Graphics::Gl {
    PIXF_API const char *GetGlErrorString(GLenum err);
} // namespace Pixf::Core::Graphics::Gl

#ifndef NDEBUG

#define PIXF_GL_CALL(x)                                                                                                \
    do {                                                                                                               \
        while (glGetError() != GL_NO_ERROR) {                                                                          \
        };                                                                                                             \
        x;                                                                                                             \
        GLenum err = glGetError();                                                                                     \
        if (err == GL_NO_ERROR)                                                                                        \
            break;                                                                                                     \
        PIXF_CORE_LOG_ERROR("OpenGL error: {} in {} ({}:{})", Pixf::Core::Graphics::Gl::GetGlErrorString(err), #x,     \
                            __FILE__, __LINE__);                                                                       \
    } while (0)

#else

#define PIXF_GL_CALL(x) x

#endif

#endif // PIXF_GL_HPP
