#ifndef FLK_VERTEX_HPP
#define FLK_VERTEX_HPP

#include "Common.hpp"
#include "VertexLayout.hpp"
#include "Math/Vector.hpp"

namespace Flock::Graphics {
    struct FLK_API Vertex {
        Vector3f position;
        Vector3f normal;
        Vector2f texCoords;

        static VertexLayout GetLayout() {
            return VertexLayout{}
                    .Add(3, AttribType::F32)
                    .Add(3, AttribType::F32)
                    .Add(2, AttribType::F32);
        }
    };
}

#endif //FLK_VERTEX_HPP
