#ifndef PIXF_TEXTURESTORE_HPP
#define PIXF_TEXTURESTORE_HPP

#include "Error/Error.hpp"
#include "Gl/Texture2D.hpp"
#include "Handle.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API TextureStoreError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API TextureStore {
    public:
        struct PIXF_API Slot {
            Gl::Texture2D texture2D;
            uint8_t version;
            bool active;
        };

        TextureStore() = default;

        TextureStore(const TextureStore &) = delete;
        TextureStore(TextureStore &&) noexcept = default;
        TextureStore &operator=(const TextureStore &) = delete;
        TextureStore &operator=(TextureStore &&) noexcept = default;

        ~TextureStore() = default;

        Texture2DHandle Create(const ImageData &image, Gl::Texture2D::Config config);
        void Destroy(Texture2DHandle handle);

        Gl::Texture2D &Get(Texture2DHandle handle);

    private:
        std::vector<Slot> m_Textures2D;

        std::pair<uint32_t, Slot &> GetSlot();
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_TEXTURESTORE_HPP
