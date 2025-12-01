#include "Timer.hpp"

namespace Pixf::Core::Time {
    using clock = std::chrono::high_resolution_clock;

    void Timer::Start() {
        m_Start = clock::now();
    }

    void Timer::Stop() {
        const auto end = clock::now();
        m_Duration = end - m_Start;
    }

    double Timer::GetDuration() const {
        return m_Duration.count();
    }
} // namespace Pixf::Core::Time
