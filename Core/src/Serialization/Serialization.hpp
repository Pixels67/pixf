#ifndef PIXF_SERIALIZATION_HPP
#define PIXF_SERIALIZATION_HPP

#include <cstdint>
#include <utility>

#include "Json/Json.hpp"

namespace Pixf::Core::Serialization {
    inline Json::object SerializeVec3(const vec3 vector) {
        Json::object json = {{"vec3",
                              {{"x", static_cast<double>(vector.x)},
                               {"y", static_cast<double>(vector.y)},
                               {"z", static_cast<double>(vector.z)}}}};
        return json;
    }

    inline vec3 DeserializeVec3(Json::object json) {
        vec3 vector;

        vector.x = json["vec3"].as_object()["x"].to_number<float>();
        vector.y = json["vec3"].as_object()["y"].to_number<float>();
        vector.z = json["vec3"].as_object()["z"].to_number<float>();

        return vector;
    }

    inline Json::object SerializeVec4(const vec4 vector) {
        Json::object json = {{"vec4",
                              {{"x", static_cast<double>(vector.x)},
                               {"y", static_cast<double>(vector.y)},
                               {"z", static_cast<double>(vector.z)},
                               {"w", static_cast<double>(vector.w)}}}};
        return json;
    }

    inline vec4 DeserializeVec4(Json::object json) {
        vec4 vector;

        vector.x = json["vec4"].as_object()["x"].to_number<float>();
        vector.y = json["vec4"].as_object()["y"].to_number<float>();
        vector.z = json["vec4"].as_object()["z"].to_number<float>();
        vector.w = json["vec4"].as_object()["w"].to_number<float>();

        return vector;
    }

    inline Json::object SerializeEuler(const vec3 euler) {
        Json::object json = {{"quat",
                              {{"x", static_cast<double>(euler.x)},
                               {"y", static_cast<double>(euler.y)},
                               {"z", static_cast<double>(euler.z)}}}};
        return json;
    }

    inline vec3 DeserializeEuler(Json::object json) {
        vec3 euler;

        euler.x = json["quat"].as_object()["x"].to_number<float>();
        euler.y = json["quat"].as_object()["y"].to_number<float>();
        euler.z = json["quat"].as_object()["z"].to_number<float>();

        return euler;
    }

    inline Json::object SerializeQuat(const quat quat) {
        const vec3 euler = degrees(eulerAngles(quat));
        return SerializeEuler(euler);
    }

    inline quat DeserializeQuat(Json::object json) {
        const vec3 euler = DeserializeEuler(std::move(json));
        const auto rot = quat(radians(euler));
        return rot;
    }

    inline Json::object SerializeColorRgb(const vec3 vector) {
        Json::object json = {{"rgb",
                              {{"r", static_cast<double>(vector.r)},
                               {"g", static_cast<double>(vector.g)},
                               {"b", static_cast<double>(vector.b)}}}};
        return json;
    }

    inline vec3 DeserializeColorRgb(Json::object json) {
        vec3 vector;

        vector.r = json["rgb"].as_object()["r"].to_number<float>();
        vector.g = json["rgb"].as_object()["g"].to_number<float>();
        vector.b = json["rgb"].as_object()["b"].to_number<float>();

        return vector;
    }

    inline Json::object SerializeColorRgba(const vec4 vector) {
        Json::object json = {{"rgba",
                              {{"r", static_cast<double>(vector.r)},
                               {"g", static_cast<double>(vector.g)},
                               {"b", static_cast<double>(vector.b)},
                               {"a", static_cast<double>(vector.a)}}}};
        return json;
    }

    inline vec4 DeserializeColorRgba(Json::object json) {
        vec4 vector;

        vector.r = json["rgba"].as_object()["r"].to_number<float>();
        vector.g = json["rgba"].as_object()["g"].to_number<float>();
        vector.b = json["rgba"].as_object()["b"].to_number<float>();
        vector.a = json["rgba"].as_object()["a"].to_number<float>();

        return vector;
    }

} // namespace Pixf::Core::Serialization

#endif // PIXF_SERIALIZATION_HPP
