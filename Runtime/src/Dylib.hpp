#ifndef DYLIB_HPP
#define DYLIB_HPP

#ifdef _WIN32
#include <windows.h>
#define DYLIB_HANDLE HMODULE
#define LOAD_DYLIB(path) LoadLibraryA(path)
#define GET_FUNC(handle, name) GetProcAddress(handle, name)
#define FREE_DYLIB(handle) FreeLibrary(handle)
#define DYLIB_EXT ".dll"
#else
#include <dlfcn.h>
#define DYLIB_HANDLE void *
#define LOAD_DYLIB(path) dlopen(path, RTLD_LAZY)
#define GET_FUNC(handle, name) dlsym(handle, name)
#define FREE_DYLIB(handle) dlclose(handle)
#ifdef __APPLE__
#define DYLIB_EXT ".dylib"
#else
#define DYLIB_EXT ".so"
#endif
#endif

#include <string>

#include "Debug/Logger.hpp"
#include "Error/Result.hpp"

namespace Pixf::Runtime {
    class Dylib {
    public:
        Dylib() : m_Handle(nullptr) {}

        Dylib(const Dylib &) = delete;
        Dylib(Dylib &&) = default;
        Dylib &operator=(const Dylib &) = delete;
        Dylib &operator=(Dylib &&) = default;

        ~Dylib() {
            if (m_Handle != nullptr) {
                Unload();
            }
        }

        static Dylib Load(const std::string &path) {
            Dylib dylib;

            dylib.m_DylibPath = path + DYLIB_EXT;
            DYLIB_HANDLE handle = LOAD_DYLIB(dylib.m_DylibPath.c_str());

            if (handle == nullptr) {
                PIXF_LOG_ERROR("Failed to load library: ", path);
                return dylib;
            }

            dylib.m_Handle = handle;
            return dylib;
        }

        template<typename T>
        T GetFunction(const std::string &funcName) {
            if (!m_Handle) {
                PIXF_LOG_ERROR("Failed to retrieve function: Library not loaded");
                return nullptr;
            }

            T func = reinterpret_cast<T>(GET_FUNC(m_Handle, funcName.c_str()));

            if (!func) {
                PIXF_LOG_ERROR("Failed to get function: ", funcName);
            }

            return func;
        }

        void Unload() {
            if (m_Handle != nullptr) {
                FREE_DYLIB(m_Handle);
                m_Handle = nullptr;
            }
        }

        bool IsLoaded() const { return m_Handle != nullptr; }

    private:
        DYLIB_HANDLE m_Handle;
        std::string m_DylibPath;
    };
} // namespace Pixf::Runtime
#endif // DYLIB_HPP
