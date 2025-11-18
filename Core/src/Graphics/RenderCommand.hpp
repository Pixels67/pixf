#ifndef PIXF_RENDERCOMMAND_HPP
#define PIXF_RENDERCOMMAND_HPP

#include "Handle.hpp"
#include "Math/Matrix.hpp"
#include "Common.hpp"

namespace Pixf::Core::Graphics {
    struct PIXF_API RenderCommand {
        MeshHandle mesh;
        MaterialHandle material;
        Math::Matrix4f modelMatrix;
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_RENDERCOMMAND_HPP
