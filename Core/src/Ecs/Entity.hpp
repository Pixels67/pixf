#ifndef FLK_ENTITY_HPP
#define FLK_ENTITY_HPP

#include "Common.hpp"
#include "Serial/Archive.hpp"

using EntityId      = u32;
using EntityVersion = u8;

namespace Flock::Ecs {
    struct Entity {
        EntityId      id: 24     = FLK_INVALID;
        EntityVersion version: 8 = 0;
    };

    inline const char *NameOf(Entity) { return "Entity"; }

    inline bool Archive(Serial::IArchive &ar, Entity &val) {
        u32 id      = val.id;
        u32 version = val.version;

        if (!ar("id", id)) return false;
        if (!ar("version", version)) return false;

        val.id      = id;
        val.version = version;

        return true;
    }
}

#endif //FLK_ENTITY_HPP
