#include "JsonArchive.hpp"

namespace Pixf::Core::Serial {
    bool JsonOutputArchive::IsOutput() { return true; }
    bool JsonOutputArchive::IsInput() { return false; }

    Json::Json JsonOutputArchive::Get() const { return m_Json; }

    void JsonOutputArchive::Recurse(const std::string &key, const std::function<void(JsonOutputArchive &)> &func) {
        JsonOutputArchive inner;
        func(inner);
        m_Json.Add(key, inner.Get());
    }

    bool JsonInputArchive::IsOutput() { return false; }
    bool JsonInputArchive::IsInput() { return true; }

    JsonInputArchive::JsonInputArchive(const Json::Json &json) : m_Json(json) {}

    void JsonInputArchive::Recurse(const std::string &key, const std::function<void(JsonInputArchive &)> &func) const {
        JsonInputArchive inner{m_Json.GetObject(key)};
        func(inner);
    }
} // namespace Pixf::Core::Serial
