#ifndef FLOCK_COMMON_HPP
#define FLOCK_COMMON_HPP

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cassert>

namespace flock {
    using f32 = float;
    using f64 = double;

    using i8  = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using usize = size_t;

    using byte = unsigned char;
}


#ifdef _WIN32
#   if defined(FLK_SHARED_BUILD)
#       define FLK_API __declspec(dllexport)
#   else
#       define FLK_API __declspec(dllimport)
#   endif
#else
#   if defined(FLK_SHARED_BUILD)
#      define FLK_API __attribute__((visibility("default")))
#   else
#      define FLK_API
#   endif
#endif

#define FLK_ASSERT(x, msg)         \
    do {                           \
        if (!x) { abort(); } \
    } while (0)

#endif //FLOCK_COMMON_HPP
