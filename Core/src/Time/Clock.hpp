#ifndef PIXF_CLOCK_HPP
#define PIXF_CLOCK_HPP
#include "Timer.hpp"

namespace Pixf::Core::Time {
    class Clock {
    public:
        void StartFrameTimer();
        void EndFrameTimer();

        double GetDeltaTime() const;

    private:
        double m_CurrentTimeSeconds = 0.0;
        Timer m_FrameTimer{};
    };
} // namespace Pixf::Core::Time

#endif // PIXF_CLOCK_HPP
