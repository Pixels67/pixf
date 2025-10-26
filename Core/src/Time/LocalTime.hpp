//
// Created by  Hasan Rasheed on 26/10/2025.
//

#ifndef LOCALTIME_HPP
#define LOCALTIME_HPP

namespace Pixf {
namespace Core {
namespace Time {

class LocalTime {
public:
    LocalTime() = default;
    
    LocalTime(const LocalTime &) = delete;
    LocalTime(LocalTime &&) = delete;
    LocalTime &operator=(const LocalTime &) = delete;
    LocalTime &operator=(LocalTime &&) = delete;
    
    ~LocalTime() = default;
    
private:
    
};

} // Time
} // Core
} // Pixf

#endif //LOCALTIME_HPP
