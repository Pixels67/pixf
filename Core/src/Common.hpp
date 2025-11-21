#ifndef PIXF_COMMON_HPP
#define PIXF_COMMON_HPP

#ifdef _WIN32
#if defined(PIXF_DYLIB_BUILD)
#define PIXF_API __declspec(dllexport)
#else
#define PIXF_API __declspec(dllimport)
#endif
#else
#if defined(PIXF_DYLIB_BUILD)
#define PIXF_API __attribute__((visibility("default")))
#else
#define PIXF_API
#endif
#endif

#define PIXF_ASSERT(expr, msg)                                                                                         \
    do { /* NOLINT(cppcoreguidelines-avoid-do-while) */                                                                \
        if (!(expr)) {                                                                                                 \
            PIXF_CORE_LOG_FATAL("Assertion failed: ({}), message: \"{}\", On: {}:{}", #expr, msg, __FILE__, __LINE__); \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (0)

#endif // PIXF_COMMON_HPP
