#ifndef FLK_COMMON_HPP
#define FLK_COMMON_HPP

#include "Debug/Log.hpp"

#ifdef _WIN32

#if defined(FLK_DYLIB_BUILD)
#define FLK_API __declspec(dllexport)
#else
#define FLK_API __declspec(dllimport)
#endif

#else

#if defined(FLK_DYLIB_BUILD)
#define FLK_API __attribute__((visibility("default")))
#else
#define FLK_API
#endif

#endif

#define FLK_ASSERT(expr)                                                                                               \
do {                                                                                                                   \
    if (!(expr)) {                                                                                                     \
        Flock::Debug::LogFtl("Assertion failed: ({}) on: {}:{}", #expr, __FILE__, __LINE__);                           \
        std::abort();                                                                                                  \
    }                                                                                                                  \
} while (false)

#define FLK_EXPECT(expr, msg)                                                                                          \
do {                                                                                                                   \
    if (!(expr)) {                                                                                                     \
        Flock::Debug::LogFtl("Assertion failed: ({}) with message: \"{}\" on: {}:{}", #expr, msg, __FILE__, __LINE__); \
        std::abort();                                                                                                  \
    }                                                                                                                  \
} while (false)

#endif //FLK_COMMON_HPP
