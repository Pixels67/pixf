#ifndef PIXF_HANDLE_HPP
#define PIXF_HANDLE_HPP

namespace Pixf::Core {
    template<typename T>
    struct Handle {
        uint32_t id : 24 {};
        uint32_t version : 8 {};
    };

    namespace Graphics {
        using Texture2DHandle = Handle<struct Texture2DTag>;
        using ShaderHandle = Handle<struct ShaderTag>;
        using MaterialHandle = Handle<struct MaterialTag>;
        using MeshHandle = Handle<struct MeshTag>;
    } // namespace Graphics

    namespace Audio {
        using ClipHandle = Handle<struct ClipTag>;
    } // namespace Audio
} // namespace Pixf::Core

#endif // PIXF_HANDLE_HPP
