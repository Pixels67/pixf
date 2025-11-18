#ifndef PIXF_HANDLE_HPP
#define PIXF_HANDLE_HPP

namespace Pixf::Core::Graphics {
    template<typename T>
    struct Handle {
        uint32_t id : 24{};
        uint32_t version : 8{};
    };

    using Texture2DHandle = Handle<struct Texture2DTag>;
    using ShaderHandle = Handle<struct ShaderTag>;
    using MaterialHandle = Handle<struct MaterialTag>;
    using MeshHandle = Handle<struct MeshTag>;
}

#endif // PIXF_HANDLE_HPP
