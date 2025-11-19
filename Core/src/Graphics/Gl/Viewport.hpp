#ifndef PIXF_VIEWPORT_HPP
#define PIXF_VIEWPORT_HPP
#include "Math/Vector.hpp"

namespace Pixf::Core::Graphics::Gl {
    struct PIXF_API Viewport {
        Math::Vector2i origin = Math::Vector2i();
        Math::Vector2i aspect;

        void Apply() const;
        double GetAspectRatio() const;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_VIEWPORT_HPP
