#ifndef PIXF_SERIALIZABLE_HPP
#define PIXF_SERIALIZABLE_HPP

#include "Common.hpp"
#include "Serialization/Json/Json.hpp"

namespace Pixf::Core::Assets {
    class AssetManager;
}

namespace Pixf::Core::Serialization {
    struct PIXF_API Serializable {
        virtual ~Serializable() = default;

        virtual Json::object Serialize(bool editorMode) = 0;
        virtual void Deserialize(const Json::object &json, Assets::AssetManager &assetManager, bool editorMode) = 0;
    };

    template<typename T>
    concept SerializableType = std::derived_from<T, Serializable> && requires {
        { T::GetTypeName() } -> std::convertible_to<const char *>;
        { T::GetTypeId() } -> std::same_as<uint64_t>;
    } && requires(T obj, const Json::object &json, Assets::AssetManager &assetManager) {
        { obj.Serialize() } -> std::same_as<Json::object>;
        { obj.Deserialize(json, assetManager) } -> std::same_as<void>;
    };

} // namespace Pixf::Core::Serialization

#endif // PIXF_SERIALIZABLE_HPP
