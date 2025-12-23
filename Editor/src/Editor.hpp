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

        void RenderOutliner(Core::Application::Context &context);
        void RenderInspector(Core::Application::Context &context) const;
        void RenderFileBrowser(Core::Application::Context &context);
        void RenderConsole(Core::Application::Context &context);

    private:
        std::optional<Core::Entities::Entity> m_SelectedEntity = std::nullopt;
    };
}

#endif // PIXF_EDITOR_HPP
