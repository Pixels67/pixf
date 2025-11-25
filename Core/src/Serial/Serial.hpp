#ifndef PIXF_SERIAL_HPP
#define PIXF_SERIAL_HPP

namespace Pixf::Core::Json {
    class Json;
}

namespace Pixf::Core::Serial {
    template<typename Archive, typename T>
    concept Serializable = requires(Archive &ar, T &value) {
        { Serialize(ar, value) } -> std::same_as<void>;
    };
} // namespace Pixf::Core::Serial

#endif // PIXF_SERIAL_HPP
