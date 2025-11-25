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

#define PIXF_SERIALIZE(Type, ...)                                                                                      \
    template<class Archive>                                                                                            \
    void Serialize(Archive &archive, Type &Type) {                                                                     \
        __VA_ARGS__                                                                                                    \
    }

#define PIXF_FIELD(Name, Field) archive(#Name, Field)

#endif // PIXF_SERIAL_HPP
