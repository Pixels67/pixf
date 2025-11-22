#ifndef PIXF_JSONARCHIVE_HPP
#define PIXF_JSONARCHIVE_HPP

#include <cereal/archives/json.hpp>

namespace Pixf::Core::Serialization {
    class JsonOutputArchive final : cereal::JSONOutputArchive {
    public:
        using JSONOutputArchive::JSONOutputArchive;

        template<typename T>
        void Save(const std::string &name, T &value) {
            (*this)(cereal::make_nvp(name.c_str(), value));
        }
    };

    class JsonInputArchive final : cereal::JSONInputArchive {
    public:
        using JSONInputArchive::JSONInputArchive;

        template<typename T>
        void Load(const std::string &name, T &value) {
            (*this)(cereal::make_nvp(name.c_str(), value));
        }
    };
} // namespace Pixf::Core::Serialization

#endif // PIXF_JSONARCHIVE_HPP
