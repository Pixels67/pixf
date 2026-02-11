#ifndef FLK_ENTITY_HPP
#define FLK_ENTITY_HPP

#include "Common.hpp"

using EntityId      = u32;
using EntityVersion = u8;

struct Entity {
    EntityId      id: 24;
    EntityVersion version: 8;
};

#endif //FLK_ENTITY_HPP
