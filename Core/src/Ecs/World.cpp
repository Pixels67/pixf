#include "World.hpp"

namespace Flock::Ecs {
    Registry &World::GetRegistry() {
        return m_Registry;
    }

    void World::Archive(Serial::IArchive &archive) {
        archive.BeginObject("registry");
        m_Registry.Archive(archive);
        archive.EndObject();

        archive.BeginObject("resources");
        for (auto &[typeId, fn]: m_ArchiveFns) {
            if (!m_Resources.contains(typeId)) {
                continue;
            }

            fn(archive, m_Resources.at(typeId));
        }

        archive.EndObject();
    }
}
