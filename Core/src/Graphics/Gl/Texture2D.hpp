#ifndef PIXF_TEXTURE2D_HPP
#define PIXF_TEXTURE2D_HPP

#include "Graphics/ImageData.hpp"

namespace Pixf::Core::Graphics::Gl {
    class PIXF_API Texture2D {
    public:
        enum class WrapMode : uint8_t {
            Clamp,
            Repeat,
            MirroredRepeat,
        };

        enum class FilterMode : uint8_t {
            Nearest,
            Linear,
        };

        struct Config {
            FilterMode filterMode = FilterMode::Linear;
            FilterMode mipmapFilterMode = FilterMode::Linear;
            WrapMode wrapMode = WrapMode::Repeat;
            bool generateMipmaps = false;

            unsigned int GetGlWrap() const;
            unsigned int GetGlMinFilter() const;
            unsigned int GetGlMagFilter() const;
        };

        static Texture2D Create(const ImageData &image, Config config);

        Texture2D() = default;

        Texture2D(const Texture2D &) = delete;
        Texture2D(Texture2D &&other) noexcept;
        Texture2D &operator=(const Texture2D &other) = delete;
        Texture2D &operator=(Texture2D &&) noexcept;

        ~Texture2D();

        Config GetConfig() const;

        void Bind(unsigned int unit) const;
        static void Unbind(unsigned int unit);

        void Clear() const;

    //private:
        unsigned int m_Id = 0;
        Config m_Config;

        explicit Texture2D(unsigned int id, Config config);
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_TEXTURE2D_HPP
