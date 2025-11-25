#ifndef PIXF_UUID_HPP
#define PIXF_UUID_HPP

#include <uuid.h>

#include "Common.hpp"
#include "Serial/Serial.hpp"

namespace Pixf::Core::Uuid {
    class PIXF_API Uuid {
    public:
        Uuid() = default;

        static Uuid DnsNamespace();
        static Uuid UrlNamespace();

        static Uuid Random();
        static Uuid FromName(const std::string &name, const Uuid &namespaceUuid);
        static std::optional<Uuid> FromString(const std::string &str);

        std::string ToString() const;
        uint64_t Hash() const;

    private:
        uuids::uuid m_Value;
    };

    PIXF_SERIALIZE(Uuid,
        std::string str = Uuid.ToString();
        PIXF_FIELD(uuid, str);
        Uuid = Uuid::FromString(str).value();
    )
} // namespace Pixf::Core::Uuid

#endif // PIXF_UUID_HPP
