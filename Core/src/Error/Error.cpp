#include "Error.hpp"

namespace Pixf::Core::Error {
    void HandleTerminate() {
        if (const std::exception_ptr exceptionPtr = std::current_exception()) {
            try {
                std::rethrow_exception(exceptionPtr);
            } catch (const std::exception &e) {
                PIXF_CORE_LOG_FATAL("Exception type: {}, message: {}", typeid(e).name(), e.what());
            } catch (...) {
                PIXF_CORE_LOG_FATAL("Unknown exception");
            }
        }

        std::abort();
    }
} // namespace Pixf::Core::Error
