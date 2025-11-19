#include "Viewport.hpp"

#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    void Viewport::Apply() const {
        PIXF_GL_CALL(glViewport(origin.x, origin.y, aspect.x, aspect.y));
    }

    double Viewport::GetAspectRatio() const {
        return static_cast<double>(aspect.x) / static_cast<double>(aspect.y);
    }
} // namespace Pixf::Core::Graphics::Gl
