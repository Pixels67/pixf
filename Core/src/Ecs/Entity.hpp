#ifndef FLK_ENTITY_HPP
#define FLK_ENTITY_HPP

#include "Common.hpp"

using EntityId      = u32;
using EntityVersion = u8;

namespace Flock::Ecs {
    struct Entity {
        EntityId      id: 24     = 0;
        EntityVersion version: 8 = 0;
    };
}

#endif //FLK_ENTITY_HPP
