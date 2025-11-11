#ifndef PIXF_BLUEPRINT_HPP
#define PIXF_BLUEPRINT_HPP

#include <functional>

#include "EntityManager.hpp"
#include "SystemsManager.hpp"
#include "Common.hpp"

namespace Pixf::Core::Entities {
    class PIXF_API Blueprint {
    public:
        Blueprint() = default;

        Blueprint(const Blueprint &) = delete;
        Blueprint(Blueprint &&) = delete;
        Blueprint &operator=(const Blueprint &) = delete;
        Blueprint &operator=(Blueprint &&) = delete;

        ~Blueprint() = default;

        void Configure(const std::function<void(EntityManager &, SystemsManager &)> &function);
        std::function<void(EntityManager &, SystemsManager &)> Get() const;

    private:
        std::function<void(EntityManager &, SystemsManager &)> m_Init;
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_BLUEPRINT_HPP
