#ifndef PIXF_RENDERPASS_HPP
#define PIXF_RENDERPASS_HPP

#include "Math/Color.hpp"
#include "Math/Matrix.hpp"

namespace Pixf::Core::Graphics {
    enum ClearType : uint8_t {
        ClearNone = 0,
        ClearColor = 1 << 0,
        ClearDepth = 1 << 1,
        ClearStencil = 1 << 2,
        ClearAll = 0b111
    };

    struct PIXF_API RenderPass {
        Math::Matrix4f viewMatrix;
        Math::Matrix4f projectionMatrix;
        uint8_t clearFlags = ClearAll;
        Math::Color3u8 clearColor = Math::Color3u8(30, 30, 30);
    };
}

#endif // PIXF_RENDERPASS_HPP
