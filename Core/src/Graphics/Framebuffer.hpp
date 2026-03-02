#ifndef FLK_FRAMEBUFFER_HPP
#define FLK_FRAMEBUFFER_HPP

#include "Common.hpp"
#include "Texture2D.hpp"
#include "TextureArray.hpp"

namespace Flock::Graphics {
    enum class Attachment {
        Color,
        Depth
    };

    inline i32 ToGlType(const Attachment attachment) {
        switch (attachment) {
            case Attachment::Color:
                return GL_COLOR_ATTACHMENT0;
            case Attachment::Depth:
                return GL_DEPTH_ATTACHMENT;
            default:
                return 0;
        }
    }

    class FLK_API Framebuffer {
        u32 m_Id = 0;

    public:
        static std::optional<Framebuffer> Create();

        Framebuffer() = default;
        ~Framebuffer();

        Framebuffer(const Framebuffer &other)     = delete;
        Framebuffer(Framebuffer &&other) noexcept;

        Framebuffer &operator=(const Framebuffer &other)     = delete;
        Framebuffer &operator=(Framebuffer &&other) noexcept;

        void Clear() const;

        bool        Bind() const;
        static void Unbind();

        bool Attach(Attachment attachment, const Texture2D &texture) const;
        bool Attach(Attachment attachment, const TextureArray &textureArray, u32 index) const;
    };
}

#endif //FLK_FRAMEBUFFER_HPP
