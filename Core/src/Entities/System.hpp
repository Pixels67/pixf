#ifndef PIXF_SYSTEM_HPP
#define PIXF_SYSTEM_HPP

#include "Event/Event.hpp"
#include "Registry.hpp"
#include "Common.hpp"

namespace Pixf::Core::Entities {
    struct PIXF_API System {
        virtual ~System() = default;

        virtual void Awake(Registry &registry);
        virtual void Start(Registry &registry);

        virtual void Update(Registry &registry, double deltaTime);
        virtual void Render(Registry &registry, double deltaTime);
        virtual void RenderGui(Registry &registry, double deltaTime);

        virtual void OnEvent(Registry &registry, Event::Event &event);
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_SYSTEM_HPP
