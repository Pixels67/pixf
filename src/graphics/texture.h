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


        Texture(const Texture &other);

        Texture &operator=(const Texture &other);

        Texture(Texture &&other);

        Texture &operator=(Texture &&other);


        ~Texture();


        void Bind(unsigned int slot) const;

        static void Unbind(unsigned int slot);

    private:
        std::string m_Path = "";
        unsigned int m_Id     = 0;
        unsigned int m_Width  = 0;
        unsigned int m_Height = 0;
        TextureConfig m_Config;
    };
} // namespace Engine::Graphics
