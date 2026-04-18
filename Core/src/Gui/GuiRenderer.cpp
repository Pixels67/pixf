#include "GuiRenderer.hpp"

#include <functional>

#include "Nvg.hpp"
#include "Graphics/Texture.hpp"
#include "Debug/Log.hpp"
#include "Gui/Font.hpp"
#include "Gui/RectTransform.hpp"
#include "Gui/Text.hpp"
#include "Memory/Buffer.hpp"
#include "nanovg.h"

namespace Flock::Gui {
    GuiRenderer GuiRenderer::Create() {
        GuiRenderer renderer;
        renderer.m_Ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

        return renderer;
    }

    GuiRenderer::~GuiRenderer() {
        Clear();
    }

    GuiRenderer::GuiRenderer(GuiRenderer &&other) noexcept {
        m_Ctx       = other.m_Ctx;
        other.m_Ctx = nullptr;
    }

    GuiRenderer &GuiRenderer::operator=(GuiRenderer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Ctx       = other.m_Ctx;
        other.m_Ctx = nullptr;

        return *this;
    }

    void GuiRenderer::Clear() const {
        if (!m_Ctx) {
            return;
        }

        nvgDeleteGL3(m_Ctx);
    }

    bool GuiRenderer::BeginFrame(const Vector2u screenSize, const u32 pixelRatio) const {
        if (!m_Ctx) {
            return false;
        }

        nvgBeginFrame(m_Ctx, static_cast<f32>(screenSize.x), static_cast<f32>(screenSize.y), static_cast<f32>(pixelRatio));
        return true;
    }

    bool GuiRenderer::EndFrame() const {
        if (!m_Ctx) {
            return false;
        }

        nvgEndFrame(m_Ctx);
        return true;
    }

    bool GuiRenderer::RenderText(
        const std::string & text,
        const u32           fontSize,
        const Font &        font,
        const RectTransform transform,
        const Color4u8      color,
        const AlignmentH    horizontalAlignment,
        const AlignmentV    verticalAlignment
    ) const {
        if (!m_Ctx) {
            return false;
        }

        if (nvgFindFont(m_Ctx, font.name.c_str()) == -1) {
            const i32 fontId = nvgCreateFontMem(m_Ctx, font.name.c_str(), static_cast<ubyte *>(font.buffer.Get()), font.buffer.Size(), 0);
            if (fontId == -1) {
                Debug::LogErr("GuiRenderer::RenderText: Invalid font data");
                return false;
            }
        }

        nvgFontFace(m_Ctx, font.name.c_str());
        nvgFillColor(m_Ctx, nvgRGBA(color.r, color.g, color.b, color.a));
        nvgFontSize(m_Ctx, fontSize);

        f32 ascender, descender, lineH;
        nvgTextMetrics(m_Ctx, &ascender, &descender, &lineH);

        i32 align = 0;

        auto [x, y]          = transform.rect.origin;
        auto [width, height] = transform.rect.aspect;

        switch (horizontalAlignment) {
            case Center: align |= NVG_ALIGN_CENTER;
                break;
            case Right: align |= NVG_ALIGN_RIGHT;
                break;
            default: align |= NVG_ALIGN_LEFT;
                break;
        }

        f32 bounds[4];
        nvgTextBoxBounds(m_Ctx, x, y, width, text.c_str(), nullptr, bounds);
        const f32 textHeight = bounds[3] - bounds[1];

        switch (verticalAlignment) {
            case Middle:
                y += (height - textHeight) * 0.5f + ascender;
                break;
            case Bottom:
                y += height - textHeight + ascender;
                break;
            default:
                y += ascender;
                break;
        }

        nvgTextAlign(m_Ctx, align);
        nvgTextBox(m_Ctx, x, y, width, text.c_str(), nullptr);

        return true;
    }

    bool GuiRenderer::RenderButton(
        const RectTransform      transform,
        const Color4u8           color,
        const Color4u8           tint,
        const Graphics::Texture *texture
    ) const {
        if (!m_Ctx) {
            return false;
        }

        if (texture) {
            RenderImage(transform, *texture);
        } else {
            RenderRect(transform, color);
        }

        RenderRect(transform, tint);

        return true;
    }

    bool GuiRenderer::RenderRect(RectTransform transform, const Color4u8 color) const {
        if (!m_Ctx) {
            return false;
        }

        auto [x, y] = transform.rect.origin;
        auto [w, h] = transform.rect.aspect;

        nvgBeginPath(m_Ctx);
        nvgRect(m_Ctx, x, y, w, h);
        nvgFillColor(m_Ctx, nvgRGBA(color.r, color.g, color.b, color.a));
        nvgFill(m_Ctx);

        return true;
    }

    bool GuiRenderer::RenderImage(RectTransform transform, const Graphics::Texture &texture) const {
        if (!m_Ctx) {
            return false;
        }

        auto [x, y] = transform.rect.origin;
        auto [w, h] = transform.rect.aspect;

        auto [tw, th] = texture.Size();

        const i32      img = nvglCreateImageFromHandleGL3(m_Ctx, texture.GlId(), tw, th, 0);
        const NVGpaint p   = nvgImagePattern(m_Ctx, x, y + h, w, -h, 0, img, 1.0F);

        nvgBeginPath(m_Ctx);
        nvgRect(m_Ctx, x, y, w, h);
        nvgFillPaint(m_Ctx, p);
        nvgFill(m_Ctx);

        return true;
    }
}
