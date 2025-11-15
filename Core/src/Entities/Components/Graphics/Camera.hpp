#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Entities/Components/RigidTransform.hpp"
#include "Window.hpp"
#include "Math/Math.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    enum class CameraType : uint8_t { Orthographic, Perspective };

    inline std::string ToString(const CameraType type) {
        switch (type) {
            case CameraType::Orthographic:
                return "Orthographic";
            case CameraType::Perspective:
                return "Perspective";
            default:
                return "Unknown";
        }
    }

    inline CameraType FromString(const std::string &str) {
        if (str == "Orthographic") {
            return CameraType::Orthographic;
        }

        if (str == "Perspective") {
            return CameraType::Perspective;
        }

        throw new std::invalid_argument("Invalid Camera type");
    }

    struct PIXF_API Camera final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(Camera)

        RigidTransform transform;

        float near = 0.1F;
        float far = 100.0F;
        float aspect = g_DefaultWindowSize.x / g_DefaultWindowSize.y;

        CameraType type;

        float size = 5.0F;
        float fov = 60.0F;

        Math::Matrix4f GetViewMatrix() const;
        Math::Matrix4f GetProjectionMatrix() const;

        Camera() = default;

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            Serialization::Json::object json;

            json["transform"] = transform.Serialize(editorMode);

            json["near"] = near;
            json["far"] = far;
            json["aspect"] = aspect;
            json["type"] = ToString(type);
            json["size"] = size;
            json["fov"] = fov;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode = false) override {
            transform.Deserialize(json.at("transform").as_object(), assetManager, editorMode);

            near = json.at("near").to_number<float>();
            far = json.at("far").to_number<float>();
            aspect = json.at("aspect").to_number<float>();
            type = FromString(json.at("type").as_string().c_str());
            size = json.at("size").to_number<float>();
            fov = json.at("fov").to_number<float>();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // CAMERA_HPP
