#ifndef PIXF_PROFILE_HPP
#define PIXF_PROFILE_HPP

#include "Common.hpp"
#include "Time/Timer.hpp"

namespace Pixf::Core::Profile {
    class PIXF_API Timer {
    public:
        static void Start(const std::string &name);
        static void Stop(const std::string &name);

    private:
        inline static std::unordered_map<std::string, Time::Timer> s_Timers{};
    };
}

#endif // PIXF_PROFILE_HPP
