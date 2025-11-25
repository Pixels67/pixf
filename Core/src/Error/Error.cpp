#include "Error.hpp"

#include <cxxabi.h>

namespace Pixf::Core::Error {
    std::string Demangle(const char* name) {
        int status = 0;
        std::unique_ptr<char, void(*)(void*)> res {
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free
        };

        return (status == 0) ? res.get() : name;
    }

    void HandleTerminate() {
        if (const std::exception_ptr exceptionPtr = std::current_exception()) {
            try {
                std::rethrow_exception(exceptionPtr);
            } catch (const std::exception &e) {
                PIXF_CORE_LOG_FATAL("Exception type: {}, message: {}", Demangle(typeid(e).name()), e.what());
            } catch (...) {
                PIXF_CORE_LOG_FATAL("Unknown exception");
            }
        }

        std::abort();
    }
} // namespace Pixf::Core::Error
