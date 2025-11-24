#include "JsonArchive.hpp"

namespace Pixf::Core::Serial {
    Json::Json JsonOutputArchive::Get() const { return m_Json; }
    void JsonOutputArchive::AddObject(const std::string &key, std::function<void(JsonOutputArchive &)> &&func) {
        JsonOutputArchive archive;
        func(archive);
        m_Json.AddObject(key, archive.Get());
    }

    JsonInputArchive::JsonInputArchive(const Json::Json &json) : m_Json(json) {}

    void JsonInputArchive::AddObject(const std::string &key, std::function<void(JsonInputArchive &)> &&func) const {
        JsonInputArchive archive(m_Json.GetObject(key));
        func(archive);
    }
} // namespace Pixf::Core::Serial
