#ifndef PIXF_TIMER_HPP
#define PIXF_TIMER_HPP

#include "Common.hpp"

namespace Pixf::Core::Time {
    class PIXF_API Timer {
    public:
        void Start();
        void Stop();

        double GetDuration() const;

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start{};
        std::chrono::duration<double> m_Duration;
    };
}

#endif // PIXF_TIMER_HPP
