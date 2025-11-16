#ifndef PIXF_ERROR_HPP
#define PIXF_ERROR_HPP

#include "Debug/Logger.hpp"

namespace Pixf::Core::Error {
    class PIXF_API Error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    PIXF_API void HandleTerminate();
} // namespace Pixf::Core::Error

#endif // PIXF_ERROR_HPP
