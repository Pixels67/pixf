#ifndef MODELRENDERER_HPP
#define MODELRENDERER_HPP

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Assets/AssetManager.hpp"
#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PIXF_API ModelRenderer final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(ModelRenderer)

        Assets::AssetHandle model;

        ModelRenderer() = default;
        explicit ModelRenderer(const Assets::AssetHandle &model) : model(model) {}

        Serialization::Json::object Serialize(bool editorMode = false) override {
            Serialization::Json::object json;

            json["uuid"] = uuids::to_string(model.GetUuid());

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         bool editorMode = false) override {
            try {
                const uuids::uuid uuid = uuids::string_generator()(json.at("uuid").as_string().c_str());
                const auto result = assetManager.GetAssetPath(uuid);
                if (result.IsError()) {
                    PIXF_LOG_ERROR(std::string("Failed to deserialize ModelRenderer: Asset UUID ") +
                                   json.at("uuid").as_string().c_str() + " not registered");
                    return;
                }

                const std::string path = result.Unwrap();
                model = assetManager.ImportModel(path).Unwrap();
            } catch (const std::runtime_error &e) {
                PIXF_LOG_ERROR("Failed to deserialize ModelRenderer: ",
                               std::string("Deserialization error ") + e.what());
            }
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // MODELRENDERER_HPP
