#include "Clock.hpp"

namespace Pixf::Core::Time {
    double Clock::GetTime() const {
        return m_CurrentTimeSeconds;
    }

    void Clock::StartFrameTimer(const double timeSeconds) {
        m_CurrentTimeSeconds = timeSeconds;
    }

    void Clock::EndFrameTimer(const double timeSeconds) {
        m_DeltaTimeSeconds = timeSeconds - m_CurrentTimeSeconds;
        m_CurrentTimeSeconds = timeSeconds;
    }

    double Clock::GetDeltaTime() const {
        return m_DeltaTimeSeconds;
    }
} // namespace Pixf::Core::Time
