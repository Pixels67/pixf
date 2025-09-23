#pragma once

#include <string>

namespace Engine::Graphics {
    struct TextureConfig {
        enum class InterpMode : char {
            Nearest,
            Linear,
        };

        enum class WrapMode : char {
            Repeat,
            MirroredRepeat,
            ClampToEdge,
        };

        InterpMode interpMode = InterpMode::Nearest;
        WrapMode wrapMode     = WrapMode::Repeat;
    };

    class Texture {
    public:
        explicit Texture(const std::string &path, TextureConfig config = {
                             TextureConfig::InterpMode::Nearest, TextureConfig::WrapMode::Repeat
                         });


        Texture(const Texture &other) = delete;

        Texture &operator=(const Texture &other) = delete;

        Texture(Texture &&other) = delete;

        Texture &operator=(Texture &&other) = delete;


        ~Texture();


        void Bind(unsigned int slot) const;

        static void Unbind(unsigned int slot);

    private:
        unsigned int m_Id       = 0;
        unsigned int m_Width    = 0;
        unsigned int m_Height   = 0;
        unsigned int m_Channels = 0;
    };
} // namespace Engine::Graphics
