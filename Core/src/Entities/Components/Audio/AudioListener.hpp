#ifndef AUDIOLISTENER_HPP
#define AUDIOLISTENER_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Audio {
    struct AudioListener final : Component {
        PIXF_COMPONENT(AudioListener);

        vec3 position;
        vec3 direction;
    };
} // namespace Pixf::Core::Entities::Components::Audio

#endif // AUDIOLISTENER_HPP
