#ifndef PIXF_TEXTURESTORE_HPP
#define PIXF_TEXTURESTORE_HPP

#include "Error/Error.hpp"
#include "Gl/Texture2D.hpp"
#include "Handle.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API TextureStoreError final : public Error::Error::Error {
        using Error::Error;
    };

    class PIXF_API TextureStore {
    public:
        struct Slot {
            Gl::Texture2D texture2D;
            uint8_t version;
            bool active;
        };

        Texture2DHandle Load(const ImageData &image, Gl::Texture2D::Config config);
        void Unload(Texture2DHandle handle);

        Gl::Texture2D &Get(Texture2DHandle handle);

    private:
        std::vector<Slot> m_Textures2D;

        std::pair<uint32_t, Slot &> GetSlot();
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_TEXTURESTORE_HPP
