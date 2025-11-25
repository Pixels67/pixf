#include "Error.hpp"

#ifndef _WIN32
#include <cxxabi.h>
#else
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
// ReSharper disable once CppWrongIncludesOrder
#include <dbghelp.h>
#endif

namespace Pixf::Core::Error {
    std::string Demangle(const char *name) {
#ifndef _WIN32
        int status = 0;
        std::unique_ptr<char, void (*)(void *)> res{abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};

        return (status == 0) ? res.get() : name;
#else
        char buffer[1024];

        if (UnDecorateSymbolName(name, buffer, sizeof(buffer), UNDNAME_COMPLETE)) {
            return buffer;
        }

        return name; // fallback
#endif
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
