#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    const char *GetGlErrorString(const GLenum err) {
        switch (err) {
            case GL_INVALID_ENUM:
                return "GL_INVALID_ENUM";
            case GL_INVALID_VALUE:
                return "GL_INVALID_VALUE";
            case GL_INVALID_OPERATION:
                return "GL_INVALID_OPERATION";
            case GL_OUT_OF_MEMORY:
                return "GL_OUT_OF_MEMORY";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "GL_INVALID_FRAMEBUFFER_OPERATION";
            default:
                return "Unknown GL Error";
        }
    }
} // namespace Pixf::Core::Graphics::Gl