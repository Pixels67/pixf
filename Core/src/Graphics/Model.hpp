#ifndef PIXF_GRAPHICS_MODEL_HPP
#define PIXF_GRAPHICS_MODEL_HPP

#include "Handle.hpp"
#include "Common.hpp"

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
