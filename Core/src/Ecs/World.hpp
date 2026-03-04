#ifndef FLK_WORLD_HPP
#define FLK_WORLD_HPP

#include "Common.hpp"
#include "Registry.hpp"

namespace Flock::Ecs {
    class FLK_API World {
        Registry                  m_Registry;
        HashMap<TypeId, std::any> m_Resources;

    public:
        template<typename T>
        World &InsertResource(T &&resource = {}) {
            m_Resources[GetTypeId<T>()] = std::move(resource);
            return *this;
        }

        template<typename T>
        T &GetResource() {
            FLK_EXPECT(m_Resources.contains(GetTypeId<T>()), "Resource does not exist!");
            return std::any_cast<T &>(m_Resources.at(GetTypeId<T>()));
        }

        [[nodiscard]] Registry &GetRegistry();
    };
}

#endif //FLK_WORLD_HPP
