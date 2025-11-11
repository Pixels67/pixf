#ifndef PIXF_SERIALIZATION_HPP
#define PIXF_SERIALIZATION_HPP

#include <cstdint>

#include "Json/Json.hpp"

namespace Pixf::Core::Serialization {
    constexpr uint64_t HashString(const char *str) {
        uint64_t hash = 0xcbf29ce484222325;
        while (*str) {
            hash ^= static_cast<uint64_t>(*str++);
            hash *= 0x100000001b3;
        }

        return hash;
    }

    inline Json::object SerializeVec3(const vec3 vector) {
        Json::object json = {{"x", static_cast<double>(vector.x)},
                                    {"y", static_cast<double>(vector.y)},
                                    {"z", static_cast<double>(vector.z)}};
        return json;
    }

    inline vec3 DeserializeVec3(Json::object json) {
        vec3 vector;

        vector.x = json["x"].to_number<float>();
        vector.y = json["y"].to_number<float>();
        vector.z = json["z"].to_number<float>();

        return vector;
    }

    inline Json::object SerializeVec4(const vec4 vector) {
        Json::object json = {{"x", static_cast<double>(vector.x)},
                                    {"y", static_cast<double>(vector.y)},
                                    {"z", static_cast<double>(vector.z)},
                                    {"w", static_cast<double>(vector.w)}};
        return json;
    }

    inline vec4 DeserializeVec4(Json::object json) {
        vec4 vector;

        vector.x = json["x"].to_number<float>();
        vector.y = json["y"].to_number<float>();
        vector.z = json["z"].to_number<float>();
        vector.w = json["w"].to_number<float>();

        return vector;
    }

    inline Json::object SerializeQuat(const quat quat) {
        Json::object json = {{"x", static_cast<double>(quat.x)},
                                    {"y", static_cast<double>(quat.y)},
                                    {"z", static_cast<double>(quat.z)},
                                    {"w", static_cast<double>(quat.w)}};
        return json;
    }

    inline quat DeserializeQuat(Json::object json) {
        quat quat;

        quat.x = json["x"].to_number<float>();
        quat.y = json["y"].to_number<float>();
        quat.z = json["z"].to_number<float>();
        quat.w = json["w"].to_number<float>();

        quat = normalize(quat);

        return quat;
    }

    inline Json::object SerializeColorRgb(const vec3 vector) {
        Json::object json = {{"r", static_cast<double>(vector.r)},
                                    {"g", static_cast<double>(vector.g)},
                                    {"b", static_cast<double>(vector.b)}};
        return json;
    }

    inline vec3 DeserializeColorRgb(Json::object json) {
        vec3 vector;

        vector.r = json["r"].to_number<float>();
        vector.g = json["g"].to_number<float>();
        vector.b = json["b"].to_number<float>();

        return vector;
    }

    inline Json::object SerializeColorRgba(const vec4 vector) {
        Json::object json = {{"r", static_cast<double>(vector.r)},
                                    {"g", static_cast<double>(vector.g)},
                                    {"b", static_cast<double>(vector.b)},
                                    {"a", static_cast<double>(vector.a)}};
        return json;
    }

    inline vec4 DeserializeColorRgba(Json::object json) {
        vec4 vector;

        vector.r = json["r"].to_number<float>();
        vector.g = json["g"].to_number<float>();
        vector.b = json["b"].to_number<float>();
        vector.a = json["a"].to_number<float>();

        return vector;
    }

} // namespace Pixf::Core::Serialization

#endif // PIXF_SERIALIZATION_HPP
