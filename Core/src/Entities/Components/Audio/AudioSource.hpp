#ifndef AUDIOSOURCE_HPP
#define AUDIOSOURCE_HPP

#include "Audio/AudioManager.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Audio {
    struct AudioSource final : Component {
        Core::Audio::AudioClipHandle clip;
        Core::Audio::AudioPlayConfig config;
    };
} // namespace Pixf::Core::Entities::Components::Audio
#endif // AUDIOSOURCE_HPP
