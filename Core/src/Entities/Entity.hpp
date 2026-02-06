#ifndef FLK_ENTITIES_HPP
#define FLK_ENTITIES_HPP

#include "Common.hpp"

using EntityId = u32;

struct Entity {
    EntityId id: 24;
    u32      version: 8;
};

#endif //FLK_ENTITIES_HPP
