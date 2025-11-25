#include "Pipeline.hpp"

namespace Pixf::Core::Application {
    void Pipeline::Update(Context &state, const double deltaTime) {
        for (auto &[typeId, stage] : m_Stages) {
            stage->Update(state, deltaTime);
        }
    }

    void Pipeline::Render(Context &state, const double deltaTime) {
        for (auto &[typeId, stage] : m_Stages) {
            stage->Render(state, deltaTime);
        }
    }

    void Pipeline::RenderGui(Context &state, const double deltaTime) {
        for (auto &[typeId, stage] : m_Stages) {
            stage->RenderGui(state, deltaTime);
        }
    }

    void Pipeline::OnEvent(Context &state, Event::Event &event) {
        for (auto &[typeId, stage] : m_Stages) {
            if (stage->OnEvent(state, event)) {
                break;
            }
        }
    }
} // namespace Pixf::Core::Application
