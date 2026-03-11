#include "Framebuffer.hpp"

namespace Flock::Graphics {
    std::optional<Framebuffer> Framebuffer::Create() {
        Framebuffer frameBuf;

        FLK_GL_CALL(glGenFramebuffers(1, &frameBuf.m_Id));

        return frameBuf;
    }

    Framebuffer::~Framebuffer() {
        Clear();
    }

    Framebuffer::Framebuffer(Framebuffer &&other) noexcept {
        m_Id       = other.m_Id;
        other.m_Id = 0;
    }

    Framebuffer &Framebuffer::operator=(Framebuffer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id       = other.m_Id;
        other.m_Id = 0;

        return *this;
    }

    void Framebuffer::Clear() const {
        if (m_Id == 0) {
            return;
        }

        FLK_GL_CALL(glDeleteFramebuffers(1, &m_Id));
    }

    bool Framebuffer::Bind() const {
        if (m_Id == 0) {
            return false;
        }

        FLK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_Id));

        FLK_GL_CALL(glDrawBuffer(GL_NONE));
        FLK_GL_CALL(glReadBuffer(GL_NONE));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Unbind();
            return false;
        }

        return true;
    }

    void Framebuffer::Unbind() {
        FLK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    bool Framebuffer::Attach(const Attachment attachment, const Texture &texture) const {
        i32 boundFramebuffer;
        i32 boundTexture;

        FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture));
        FLK_GL_CALL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFramebuffer));

        FLK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_Id));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.GetGlId()));
        FLK_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, ToGlType(attachment), GL_TEXTURE_2D, texture.GetGlId(), 0));

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, boundTexture));
        FLK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, boundFramebuffer));

        return true;
    }

    bool Framebuffer::Attach(const Attachment attachment, const TextureArray &textureArray, u32 index) const {
        if (index >= textureArray.GetLayerCount()) {
            return false;
        }

        i32 boundFramebuffer;
        i32 boundTexture;

        FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &boundTexture));
        FLK_GL_CALL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFramebuffer));

        FLK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_Id));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.GetGlId()));
        FLK_GL_CALL(
            glFramebufferTextureLayer(
                GL_FRAMEBUFFER,
                ToGlType(attachment),
                textureArray.GetGlId(),
                0,
                index
            )
        );

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, boundTexture));
        FLK_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, boundFramebuffer));

        return true;
    }
}
