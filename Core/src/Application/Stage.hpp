#ifndef PIXF_STAGE_HPP
#define PIXF_STAGE_HPP

#include "Common.hpp"
#include "Event/Event.hpp"
#include "State.hpp"

namespace Pixf::Core::Application {
    class PIXF_API Stage {
    public:
        virtual ~Stage() = default;

        virtual void OnAttach(State &state) {}
        virtual void OnDetach(State &state) {}

        virtual void Update(State &state, double deltaTime) {}
        virtual void Render(State &state, double deltaTime) {}
        virtual void RenderGui(State &state, double deltaTime) {}

        virtual bool OnEvent(State &state, Event::Event &event) { return false; }
    };
} // namespace Pixf::Core::Application

#endif // PIXF_STAGE_HPP
