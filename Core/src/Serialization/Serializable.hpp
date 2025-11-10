#ifndef PIXF_SERIALIZABLE_HPP
#define PIXF_SERIALIZABLE_HPP

#include "Assets/AssetManager.hpp"
#include "Json/Json.hpp"

namespace Pixf::Core::Serialization {
    struct Serializable {
        virtual ~Serializable() = default;

        virtual Json::object Serialize() = 0;
        virtual void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) = 0;
    };
} // namespace Pixf::Core::Serialization

#endif // PIXF_SERIALIZABLE_HPP
