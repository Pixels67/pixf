#ifndef PIXF_SYSTEMREGISTRY_HPP
#define PIXF_SYSTEMREGISTRY_HPP

#include "System.hpp"
#include "TypeId.hpp"
#include "Common.hpp"

namespace Pixf::Core::Entities {
    class PIXF_API SystemRegistry {
    public:
        SystemRegistry() = default;

        SystemRegistry(const SystemRegistry&) = delete;
        SystemRegistry(SystemRegistry&&) noexcept = default;
        SystemRegistry& operator=(const SystemRegistry&) = delete;
        SystemRegistry& operator=(SystemRegistry&&) noexcept = default;

        ~SystemRegistry() = default;

        void Awake(Registry &registry);
        void Start(Registry &registry);

        void Update(Registry &registry, double deltaTime);
        void Render(Registry &registry, double deltaTime);
        void RenderGui(Registry &registry, double deltaTime);

        void OnEvent(Registry &registry, Event::Event &event);

        void Clear();

        template<typename T>
        void Register() {
            static_assert(std::is_base_of<System, T>());

            m_Systems[GetTypeId<T>()] = std::move(std::make_unique<T>());
        }

    private:
        std::unordered_map<TypeId, std::unique_ptr<System>> m_Systems;
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_SYSTEMREGISTRY_HPP
