//
// Created by  Hasan Rasheed on 26/10/2025.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

namespace Pixf {
namespace Core {
namespace Debug {

class Logger {
public:
    Logger() = default;
    
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger &operator=(Logger &&) = delete;
    
    ~Logger() = default;
    
private:
    
};

} // Debug
} // Core
} // Pixf

#endif //LOGGER_HPP
