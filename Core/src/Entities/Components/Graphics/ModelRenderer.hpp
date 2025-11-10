#ifndef MODELRENDERER_HPP
#define MODELRENDERER_HPP

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Assets/AssetManager.hpp"
#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct ModelRenderer final : Component, Serialization::Serializable {
        SERIALIZABLE(ModelRenderer)

        Assets::AssetHandle model;

        ModelRenderer() = default;
        explicit ModelRenderer(const Assets::AssetHandle &model) : model(model) {}

        Json::object Serialize() override {
            Json::object json;

            json["uuid"] = uuids::to_string(model.GetUuid());

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            const uuids::uuid uuid = uuids::string_generator()(json.at("uuid").as_string().c_str());
            const std::string path = assetManager.GetAssetPath(uuid).value();
            model = assetManager.ImportModel(path).Unwrap();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // MODELRENDERER_HPP
