#ifndef FLK_GUIRENDERER_HPP
#define FLK_GUIRENDERER_HPP

#include "Common.hpp"
#include "Font.hpp"
#include "RectTransform.hpp"
#include "Text.hpp"
#include "Math/Color.hpp"

struct NVGcontext;

namespace Flock::Gui {
    class FLK_API GuiRenderer {
        NVGcontext *m_Ctx = nullptr;

    public:
        static GuiRenderer Create();

        GuiRenderer() = default;
        ~GuiRenderer();

        GuiRenderer(const GuiRenderer &other) = delete;
        GuiRenderer(GuiRenderer &&other) noexcept;

        GuiRenderer &operator=(const GuiRenderer &other) = delete;
        GuiRenderer &operator=(GuiRenderer &&other) noexcept;

        void Clear() const;

        bool BeginFrame(Vector2u screenSize, u32 pixelRatio = 1) const;
        bool EndFrame() const;

        bool RenderText(
            const std::string &text,
            u32                fontSize,
            const Font &       font,
            RectTransform      transform           = {},
            Color4u8           color               = Color4u8::White(),
            AlignmentH         horizontalAlignment = Left,
            AlignmentV         verticalAlignment   = Top
        ) const;
    };
}

#endif //FLK_GUIRENDERER_HPP
