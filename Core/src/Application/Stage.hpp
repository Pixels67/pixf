#ifndef PIXF_STAGE_HPP
#define PIXF_STAGE_HPP

#include "Common.hpp"
#include "Context.hpp"
#include "Event/Event.hpp"

namespace Pixf::Core::Application {
    class PIXF_API Stage {
    public:
        virtual ~Stage() = default;

        virtual void OnAttach(Context &context) {}
        virtual void OnDetach(Context &context) {}

        virtual void Update(Context &context, double deltaTime) {}
        virtual void Render(Context &context, double deltaTime) {}
        virtual void RenderGui(Context &context, double deltaTime) {}

        virtual bool OnEvent(Context &context, Event::Event &event) { return false; }
    };
} // namespace Pixf::Core::Application

#endif // PIXF_STAGE_HPP
