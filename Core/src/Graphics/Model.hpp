#ifndef PIXF_GRAPHICS_MODEL_HPP
#define PIXF_GRAPHICS_MODEL_HPP

#include "Common.hpp"
#include "Handle.hpp"

namespace Pixf::Core::Graphics {
    struct PIXF_API ModelElement {
        MeshHandle mesh;
        MaterialHandle material;
    };

    struct PIXF_API Model {
        std::vector<ModelElement> elements;
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_GRAPHICS_MODEL_HPP
