#include "Clock.hpp"

#include "../Graphics/Gl/Gl.hpp"

namespace Pixf::Core::Time {
    double Clock::GetTime() const {
        return m_Time;
    }

    double Clock::GetDeltaTime() const {
        return m_DeltaTime;
    }

    void Clock::Tick() {
        m_DeltaTime = glfwGetTime() - m_Time;
        m_Time = glfwGetTime();
    }
}
