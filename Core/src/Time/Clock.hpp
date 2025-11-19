#ifndef PIXF_CLOCK_HPP
#define PIXF_CLOCK_HPP

namespace Pixf::Core::Time {
    class Clock {
    public:
        double GetTime() const;

        void StartFrameTimer(double timeSeconds);
        void EndFrameTimer(double timeSeconds);

        double GetDeltaTime() const;

    private:
        double m_CurrentTimeSeconds = 0.0;
        double m_DeltaTimeSeconds = 0.0;
    };
} // namespace Pixf::Core::Time

#endif // PIXF_CLOCK_HPP
