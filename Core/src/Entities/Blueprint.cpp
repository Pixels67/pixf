#include "Blueprint.hpp"

namespace Pixf::Core::Entities {
    void Blueprint::Set(const std::function<void(EntityManager &, SystemsManager &)> &function) {
        m_Init = function;
    }

    std::function<void(EntityManager &, SystemsManager &)> Blueprint::Get() const {
        return m_Init;
    }
} // namespace Pixf::Core::Entities
