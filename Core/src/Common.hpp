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

#ifdef _WIN32

using ssize_t = unsigned int;

#endif

using usize = size_t;
using isize = ssize_t;
using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using u64   = uint64_t;
using i8    = int8_t;
using i16   = int16_t;
using i32   = int32_t;
using i64   = int64_t;
using f32   = float;
using f64   = double;

template<typename T>
using OptionalRef = std::optional<std::reference_wrapper<T> >;

#endif //FLK_COMMON_HPP
