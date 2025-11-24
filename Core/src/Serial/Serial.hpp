#ifndef PIXF_SERIAL_HPP
#define PIXF_SERIAL_HPP

namespace Pixf::Core::Json {
    class Json;
}

namespace Pixf::Core::Serial {
    template<typename Archive, typename T>
    concept Serializable = requires(Archive archive, T &type) {
        { T::Serialize(archive, type) } -> std::same_as<void>;
    };
} // namespace Pixf::Core::Serial

#endif // PIXF_SERIAL_HPP
