#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "../Common.hpp"

namespace Pixf::Core::Graphics {
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texCoords;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif //VERTEX_HPP
