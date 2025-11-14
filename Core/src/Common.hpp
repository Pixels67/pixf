#ifndef COMMON_HPP
#define COMMON_HPP

#include <concepts>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace boost {}

#define PIXF_ASSERT(expr, ...)                                                                                         \
    do {                                                                                                               \
        if (!(expr)) {                                                                                                 \
            const char *msg = __VA_ARGS__;                                                                             \
            PIXF_LOG_FATAL("Assertion failed: (" #expr ")", (msg[0] ? ", Message: \"" : ""), msg,                      \
                           "\", On: ", __FILE__, ':', __LINE__, '\n');                                                 \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (0)

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

namespace Pixf {
    using namespace glm;
    using namespace boost;
} // namespace Pixf

constexpr uint64_t HashString(const char *str) {
    uint64_t hash = 0xcbf29ce484222325;
    while (*str) {
        hash ^= static_cast<uint64_t>(*str++);
        hash *= 0x100000001b3;
    }

    return hash;
}

template<typename T>
concept TypeInformed = requires {
    { T::GetTypeName() } -> std::same_as<const char *>;
    { T::GetTypeId() } -> std::same_as<uint64_t>;
};

#define PIXF_TYPE_INFO(Name)                                                                                           \
    static constexpr const char *GetTypeName() { return #Name; }                                                       \
    static constexpr uint64_t GetTypeId() { return HashString(GetTypeName()); }

#endif // COMMON_HPP
