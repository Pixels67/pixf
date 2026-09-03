#include <cstdio>

#include "collect/vector.hpp"
#include "memory/allocator.hpp"

using namespace flock;
using namespace flock::memory;

i32 main() {
    arena_allocator alloc = std::move(arena_allocator::create(16).get());
    set_allocator(&alloc);

    vector<char> str{16};

    str.push('H');
    str.push('e');
    str.push('l');
    str.push('l');
    str.push('o');
    str.push(',');
    str.push(' ');
    str.push('W');
    str.push('o');
    str.push('r');
    str.push('l');
    str.push('d');
    str.push('!');

    str.insert(5, 'p');
    str.insert(6, 'e');

    str.remove(5);
    str.remove(5);
    str.remove(5);

    str.pop();

    str[6] = 'w';
    str[0] = 'h';

    for (usize i = 0; i < str.len(); i++) {
        printf("%c", str[i]);
    }

    printf("\n");

    reset_allocator();
}
