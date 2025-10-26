#ifndef LOCALTIME_HPP
#define LOCALTIME_HPP

#include <string>

namespace Pixf::Core::Time {
    struct LocalTime {
        unsigned int year;
        unsigned int month;
        unsigned int day;
        unsigned int hour;
        unsigned int minute;
        unsigned int second;

        std::string ToString() const;
    };

    LocalTime GetLocalTime();
} // namespace Pixf::Core::Time

#endif // LOCALTIME_HPP
