#include "Clock.hpp"

namespace Pixf::Core::Time {
    using clock = std::chrono::high_resolution_clock;

    void Clock::StartFrameTimer() {
        m_FrameTimer.Start();
    }

    void Clock::EndFrameTimer() {
        m_FrameTimer.Stop();
    }

    double Clock::GetDeltaTime() const {
        return m_FrameTimer.GetDuration();
    }
} // namespace Pixf::Core::Time
