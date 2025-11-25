#ifndef PIXF_STATE_HPP
#define PIXF_STATE_HPP

#include "Common.hpp"
#include "Entities/WorldManager.hpp"
#include "Files/Assets/AssetManager.hpp"
#include "Graphics/Gl/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Resources.hpp"
#include "Time/Clock.hpp"

namespace Pixf::Core::Application {
    struct PIXF_API Context {
        Graphics::Gl::Window window;
        Graphics::Resources resources{};
        Graphics::Renderer renderer{};
        Files::Assets::AssetManager assetManager{"./Assets/"};
        Time::Clock clock{};
        Entities::WorldManager worldManager{};
    };
} // namespace Pixf::Core::Application

#endif // PIXF_STATE_HPP
