#ifndef LOCALTIME_HPP
#define LOCALTIME_HPP

#include <string>

#include "Common.hpp"

namespace Pixf::Core::Time {
    struct PIXF_API LocalTime {
        unsigned int year;
        unsigned int month;
        unsigned int day;
        unsigned int hour;
        unsigned int minute;
        unsigned int second;

        std::string ToString() const;
    };

    PIXF_API LocalTime GetLocalTime();
} // namespace Pixf::Core::Time

#endif // LOCALTIME_HPP
