#ifndef GL_HPP
#define GL_HPP

#include <iostream>

#include <glad/glad.h>
// ReSharper disable once CppWrongIncludesOrder
#include <GLFW/glfw3.h>

#ifndef NDEBUG

#define PIXF_GL_CALL(x)                                                                                                     \
    do {                                                                                                               \
        while (glGetError() != GL_NO_ERROR) {                                                                          \
        };                                                                                                             \
        x;                                                                                                             \
        GLenum err = glGetError();                                                                                     \
        if (err != GL_NO_ERROR) {                                                                                      \
            std::cerr << "OpenGL error 0x" << std::hex << err << std::dec << " in " << #x << " (" << __FILE__ << ":"   \
                      << __LINE__ << ")" << '\n';                                                                      \
        }                                                                                                              \
    } while (0)

#else

#define GL_CALL(x) x

#endif

#endif // GL_HPP
