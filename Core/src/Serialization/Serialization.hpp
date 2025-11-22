#ifndef PIXF_SERIALIZATION_HPP
#define PIXF_SERIALIZATION_HPP

#include <cereal/cereal.hpp>

#define PIXF_SERIALIZE(Type)                                                                                           \
    template<typename Archive>                                                                                         \
    void serialize(Archive &archive, Type &obj) {                                                                      \
        Serialize(archive, obj);                                                                                       \
    }

namespace Pixf::Core::Serialization {
    template<typename Archive, typename T>
    void SerializeNvp(Archive &archive, const std::string &name, T &obj) {
        archive(cereal::make_nvp(name, obj));
    }

    template<typename Archive, typename T>
    void Serialize(Archive &archive, T &obj) {
        archive(obj);
    }
} // namespace Pixf::Core::Serialization

namespace Pixf::Core::Math {
    template<typename Archive, typename T>
    void Serialize(Archive &archive, Vector2<T> &vector) {
        Serialization::SerializeNvp(archive, "x", vector.x);
        Serialization::SerializeNvp(archive, "y", vector.y);
    }

    template<typename Archive, typename T>
    void Serialize(Archive &archive, Vector3<T> &vector) {
        Serialization::SerializeNvp(archive, "x", vector.x);
        Serialization::SerializeNvp(archive, "y", vector.y);
        Serialization::SerializeNvp(archive, "z", vector.z);
    }

    template<typename Archive, typename T>
    void Serialize(Archive &archive, Vector4<T> &vector) {
        Serialization::SerializeNvp(archive, "x", vector.x);
        Serialization::SerializeNvp(archive, "y", vector.y);
        Serialization::SerializeNvp(archive, "z", vector.z);
        Serialization::SerializeNvp(archive, "w", vector.w);
    }

    template<typename Archive, typename T>
    void Serialize(Archive &archive, Color3<T> &color) {
        Serialization::SerializeNvp(archive, "r", color.r);
        Serialization::SerializeNvp(archive, "g", color.g);
        Serialization::SerializeNvp(archive, "b", color.b);
    }

    template<typename Archive, typename T>
    void Serialize(Archive &archive, Color4<T> &color) {
        Serialization::SerializeNvp(archive, "r", color.r);
        Serialization::SerializeNvp(archive, "g", color.g);
        Serialization::SerializeNvp(archive, "b", color.b);
        Serialization::SerializeNvp(archive, "a", color.a);
    }

    template<typename Archive>
    void Serialize(Archive &archive, Quaternion &quat) {
        Serialization::SerializeNvp(archive, "x", quat.x);
        Serialization::SerializeNvp(archive, "y", quat.y);
        Serialization::SerializeNvp(archive, "z", quat.z);
        Serialization::SerializeNvp(archive, "w", quat.w);
    }

    PIXF_SERIALIZE(Vector2f)
    PIXF_SERIALIZE(Vector3f)
    PIXF_SERIALIZE(Vector4f)

    PIXF_SERIALIZE(Vector2d)
    PIXF_SERIALIZE(Vector3d)
    PIXF_SERIALIZE(Vector4d)

    PIXF_SERIALIZE(Vector2i)
    PIXF_SERIALIZE(Vector3i)
    PIXF_SERIALIZE(Vector4i)

    PIXF_SERIALIZE(Vector2u)
    PIXF_SERIALIZE(Vector3u)
    PIXF_SERIALIZE(Vector4u)

    PIXF_SERIALIZE(Color3u8)
    PIXF_SERIALIZE(Color4u8)

    PIXF_SERIALIZE(Color3u16)
    PIXF_SERIALIZE(Color4u16)

    PIXF_SERIALIZE(Quaternion)
} // namespace Pixf::Core::Math

namespace Pixf::Core::Uuid {
    template<typename Archive>
    void Serialize(Archive &archive, Uuid &uuid) {
        std::string str = uuid.ToString();
        Serialization::SerializeNvp(archive, "value", str);
        uuid = Uuid::FromString(str.c_str()).value();
    }

    PIXF_SERIALIZE(Uuid)
} // namespace Pixf::Core::Uuid
#endif // PIXF_SERIALIZATION_HPP
