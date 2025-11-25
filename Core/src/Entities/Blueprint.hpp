#ifndef PIXF_BLUEPRINT_HPP
#define PIXF_BLUEPRINT_HPP

#include "World.hpp"
#include "Common.hpp"

namespace Pixf::Core::Entities {
    class PIXF_API Blueprint {
    public:
        explicit Blueprint(const std::function<void(World &)> &initFunction);
        std::function<void(World &)> Get();

    private:
        std::function<void(World &)> m_InitFn;
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_BLUEPRINT_HPP
