#include "common.hpp"
#include "memory/allocator.hpp"

using namespace flock;
using namespace flock::memory;

i32 main() {
    arena_allocator alloc = std::move(arena_allocator::create(32).get());

    byte *str = alloc.allocate(67).get();
    str[0]    = 'H';
    str[1]    = 'e';
    str[2]    = 'l';
    str[3]    = 'l';
    str[4]    = 'o';
    str[5]    = '\n';
}
