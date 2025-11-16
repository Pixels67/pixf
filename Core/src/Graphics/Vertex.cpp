#include "Vertex.hpp"

namespace Pixf::Core::Graphics {
    Gl::VertexLayout Vertex::GetLayout() {
        return Gl::VertexLayout()
                .Add(3, Gl::AttribType::Float32)
                .Add(3, Gl::AttribType::Float32)
                .Add(2, Gl::AttribType::Float32);
    }
} // namespace Pixf::Core::Graphics
