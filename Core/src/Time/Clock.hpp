#ifndef CLOCK_HPP
#define CLOCK_HPP
#include "Common.hpp"

namespace Pixf::Core::Time {
    class PIXF_API Clock {
    public:
        Clock() = default;

        Clock(const Clock &other) = default;
        Clock(Clock &&other) = delete;
        Clock &operator=(const Clock &other) = default;
        Clock &operator=(Clock &&other) = delete;

        ~Clock() = default;

        double GetTime() const;
        double GetDeltaTime() const;
        void Tick();

    private:
        double m_Time = 0.0;
        double m_DeltaTime = 0.0;
    };
} // namespace Pixf::Core::Time

#endif // CLOCK_HPP
