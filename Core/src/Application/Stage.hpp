#ifndef PIXF_STAGE_HPP
#define PIXF_STAGE_HPP

#include "Common.hpp"
#include "Context.hpp"
#include "Event/Event.hpp"

namespace Pixf::Core::Application {
    class PIXF_API Stage {
    public:
        virtual ~Stage() = default;

        virtual void OnAttach(Context &state) {}
        virtual void OnDetach(Context &state) {}

        virtual void Update(Context &state, double deltaTime) {}
        virtual void Render(Context &state, double deltaTime) {}
        virtual void RenderGui(Context &state, double deltaTime) {}

        virtual bool OnEvent(Context &state, Event::Event &event) { return false; }
    };
} // namespace Pixf::Core::Application

#endif // PIXF_STAGE_HPP
