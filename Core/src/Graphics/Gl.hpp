#ifndef FLK_GL_HPP
#define FLK_GL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Debug/Log.hpp"

namespace Flock::Graphics {
    const char *GetGlErrorString(GLenum err);
}

#ifndef NDEBUG

#define FLK_GL_CALL(x)                                                                                                 \
do {                                                                                                                   \
    while (glGetError() != GL_NO_ERROR) {};                                                                            \
    x;                                                                                                                 \
    GLenum err = glGetError();                                                                                         \
    if (err == GL_NO_ERROR) { break; }                                                                                 \
    Flock::Debug::LogErr("OpenGL error: {} occurred while running {} ({}:{})",                                         \
        Flock::Graphics::GetGlErrorString(err),                                                                    \
        #x,                                                                                                            \
        __FILE__,                                                                                                      \
        __LINE__);                                                                                                     \
} while (0)

#else

#define FLK_GL_CALL(x) x

#endif

#endif //FLK_GL_HPP
