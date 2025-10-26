#include "LocalTime.hpp"

#include <chrono>
#include <string>

namespace Pixf::Core::Time {
    std::string LocalTime::ToString() const {
        return std::to_string(year) + '-' + std::to_string(month) + '-' + std::to_string(day) + ' ' +
               std::to_string(hour) + ':' + std::to_string(minute) + ':' + std::to_string(second);
    }

    LocalTime GetLocalTime() {
        const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        const std::tm *localTime = std::localtime(&now);

        LocalTime result{};

        result.year = localTime->tm_year + 1900;
        result.month = localTime->tm_mon + 1;
        result.day = localTime->tm_mday;

        result.hour = localTime->tm_hour;
        result.minute = localTime->tm_min;
        result.second = localTime->tm_sec;

        return result;
    }
} // namespace Pixf::Core::Time
