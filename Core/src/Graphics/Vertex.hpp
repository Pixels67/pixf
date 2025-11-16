#ifndef PIXF_VERTEX_HPP
#define PIXF_VERTEX_HPP

#include "Gl/VertexLayout.hpp"
#include "Math/Vector.hpp"

namespace Pixf::Core::Graphics {
    struct PIXF_API Vertex {
        Math::Vector3f position;
        Math::Vector3f normal;
        Math::Vector2f texCoord;

        static Gl::VertexLayout GetLayout();
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_VERTEX_HPP
