#include "Timer.hpp"

#include "Debug/Logger.hpp"
#include "Time/Clock.hpp"

namespace Pixf::Core::Profile {
    void Timer::Start(const std::string &name) { s_Timers[name].Start(); }
    void Timer::Stop(const std::string &name) {
        s_Timers[name].Stop();
        PIXF_CORE_LOG_INFO("Timer {} took {}ms.", name, s_Timers[name].GetDuration() * 1000.0);
    }
} // namespace Pixf::Core::Profile
