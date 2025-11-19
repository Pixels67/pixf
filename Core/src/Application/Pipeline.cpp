#include "Pipeline.hpp"

namespace Pixf::Core::Application {
    void Pipeline::Update(State &state, const double deltaTime) {
        for (auto &[typeId, stage] : m_Stages) {
            stage->Update(state, deltaTime);
        }
    }

    void Pipeline::Render(State &state, const double deltaTime) {
        for (auto &[typeId, stage] : m_Stages) {
            stage->Render(state, deltaTime);
        }
    }

    void Pipeline::RenderGui(State &state, const double deltaTime) {
        for (auto &[typeId, stage] : m_Stages) {
            stage->RenderGui(state, deltaTime);
        }
    }

    void Pipeline::OnEvent(State &state, Event::Event &event) {
        for (auto &[typeId, stage] : m_Stages) {
            if (stage->OnEvent(state, event)) {
                break;
            }
        }
    }
} // namespace Pixf::Core::Application
