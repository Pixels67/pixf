#ifndef PIXF_EDITOR_HPP
#define PIXF_EDITOR_HPP

#include "Application/Application.hpp"

namespace Pixf::Editor {
    class EditorRender final : public Core::Application::Stage {
    public:
        void Render(Core::Application::Context &context, double deltaTime) override;
    };

    class EditorGui final : public Core::Application::Stage {
    public:
        void RenderGui(Core::Application::Context &context, double deltaTime) override;

        void RenderOutline(Core::Application::Context &context);
    };
}

#endif // PIXF_EDITOR_HPP
