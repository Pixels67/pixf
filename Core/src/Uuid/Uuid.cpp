#include "Uuid.hpp"

#include <uuid.h>

namespace Pixf::Core::Uuid {
    Uuid Uuid::DnsNamespace() { return FromString("6ba7b810-9dad-11d1-80b4-00c04fd430c8").value(); }
    Uuid Uuid::UrlNamespace() { return FromString("6ba7b811-9dad-11d1-80b4-00c04fd430c8").value(); }

    Uuid Uuid::Random() {
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size>{};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::mt19937 generator(seq);
        uuids::uuid_random_generator gen{generator};

        Uuid uuid;
        uuid.m_Value = gen();
        return uuid;
    }

    Uuid Uuid::FromName(const std::string &name, const Uuid &namespaceUuid) {
        uuids::uuid_name_generator gen(namespaceUuid.m_Value);

        Uuid uuid;
        uuid.m_Value = gen(name);
        return uuid;
    }

    std::optional<Uuid> Uuid::FromString(const std::string &str) {
        const auto result = uuids::uuid::from_string(str);

        if (!result) {
            return std::nullopt;
        }

        Uuid uuid;
        uuid.m_Value = result.value();
        return uuid;
    }

    std::string Uuid::ToString() const { return uuids::to_string(m_Value); }

    uint64_t Uuid::Hash() const {
        return std::hash<uuids::uuid>()(m_Value);
    }
} // namespace Pixf::Core::Uuid
