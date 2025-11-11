#ifndef PIXF_SERIALIZABLE_HPP
#define PIXF_SERIALIZABLE_HPP

#include "Assets/AssetManager.hpp"
#include "Json/Json.hpp"

namespace Pixf::Core::Serialization {
    struct PIXF_API Serializable {
        virtual ~Serializable() = default;

        virtual Json::object Serialize() = 0;
        virtual void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) = 0;
    };

    template<typename T>
    concept SerializableType = std::derived_from<T, Serializable> && requires {
        { T::GetTypeName() } -> std::convertible_to<const char *>;
        { T::GetTypeId() } -> std::same_as<uint64_t>;
    } && requires(T obj, const Json::object &json, Assets::AssetManager &assetManager) {
        { obj.Serialize() } -> std::same_as<Json::object>;
        { obj.Deserialize(json, assetManager) } -> std::same_as<void>;
    };

#define PIXF_SERIALIZABLE(Name)                                                                                             \
    static constexpr const char *GetTypeName() { return #Name; }                                                       \
    static constexpr uint64_t GetTypeId() { return Pixf::Core::Serialization::HashString(GetTypeName()); }

} // namespace Pixf::Core::Serialization

#endif // PIXF_SERIALIZABLE_HPP
