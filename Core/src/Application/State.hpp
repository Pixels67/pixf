#ifndef PIXF_STATE_HPP
#define PIXF_STATE_HPP

#include "Common.hpp"
#include "Graphics/Gl/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Resources.hpp"
#include "Time/Clock.hpp"

namespace Pixf::Core::Application {
    struct PIXF_API State {
        Graphics::Gl::Window window;
        Graphics::Resources resources{};
        Graphics::Renderer renderer{};
        Time::Clock clock{};
    };
} // namespace Pixf::Core::Application

#endif // PIXF_STATE_HPP
