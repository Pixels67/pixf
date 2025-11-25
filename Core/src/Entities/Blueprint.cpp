#include "Blueprint.hpp"

namespace Pixf::Core::Entities {
    Blueprint::Blueprint(const std::function<void(World &)> &initFunction) : m_InitFn(initFunction) {}

    std::function<void(World &)> Blueprint::Get() {
        return m_InitFn;
    }
} // namespace Pixf::Core::Entities
