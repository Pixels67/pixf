#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <string>

#include "Error/Result.hpp"

namespace Pixf::Core::Graphics::Gl {
    enum class WrapMode : uint8_t {
        Clamp,
        Repeat,
        MirroredRepeat,
    };

    enum class FilterMode : uint8_t {
        Nearest,
        Linear,
    };

    struct TextureConfig {
        FilterMode filterMode = FilterMode::Linear;
        FilterMode mipmapFilterMode = FilterMode::Linear;
        WrapMode wrapMode = WrapMode::Repeat;
        bool generateMipmaps = true;

        unsigned int GetGlWrap() const;
        unsigned int GetGlMinFilter() const;
        unsigned int GetGlMagFilter() const;

        bool operator==(const TextureConfig &other) const;
        bool operator!=(const TextureConfig &other) const;
    };

    enum class TextureError : uint8_t {
        None = 0,
        FailedToLoad,
    };

    // 2D/3D/4D are an exception to class names being pure PascalCase
    class Texture2D {
    public:
        static Error::Result<Texture2D, TextureError> Load(const std::string &path, TextureConfig config);

        Texture2D(const Texture2D &) = delete;
        Texture2D(Texture2D &&other) noexcept;
        Texture2D &operator=(const Texture2D &other) = delete;
        Texture2D &operator=(Texture2D &&) noexcept;

        ~Texture2D();

        void Bind(unsigned int index) const;
        static void Unbind(unsigned int index);

        TextureConfig GetConfig() const;

    private:
        unsigned int m_Id = 0;
        TextureConfig m_Config;

        explicit Texture2D(unsigned int id, TextureConfig config);

        void Cleanup() const;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // TEXTURE2D_HPP
