#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Math/Math.hpp"

namespace Pixf::Core::Graphics {
    struct Vertex {
        Math::Vector3f position;
        Math::Vector3f normal;
        Math::Vector2f texCoords;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif //VERTEX_HPP
