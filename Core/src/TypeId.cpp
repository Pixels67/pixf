#include "TypeId.hpp"

namespace Pixf::Core {
    TypeId GetNextTypeId() {
        static TypeId counter = 0;
        return ++counter;
    }
}