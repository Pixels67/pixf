#ifndef FLK_WORLD_HPP
#define FLK_WORLD_HPP

#include "Common.hpp"
#include "Registry.hpp"

namespace Flock::Ecs {
    class FLK_API World {
        Registry                             m_Registry;
        std::unordered_map<TypeId, std::any> m_Resources;

        std::unordered_map<TypeId, std::function<void(Serial::IArchive &, std::any &)> > m_ArchiveFns;

    public:
        static World Default();

        void SetDefaults();

        template<typename T>
        World &InsertResource(T &&resource = {}) {
            m_Resources[GetTypeId<T>()] = std::forward<T>(resource);
            if constexpr (IsReflectable<T>) {
                m_ArchiveFns[GetTypeId<T>()] = [](Serial::IArchive &archive, std::any &any) {
                    T &res = std::any_cast<T &>(any);
                    archive(GetTypeName<T>(), res);
                };
            }

            return *this;
        }

        template<typename T>
        bool HasResource() const {
            return m_Resources.contains(GetTypeId<T>());
        }

        template<typename T>
        T &GetResource() {
            FLK_EXPECT(HasResource<T>(), "Resource does not exist!");
            return std::any_cast<T &>(m_Resources.at(GetTypeId<T>()));
        }

        [[nodiscard]] Registry &GetRegistry();

        void Archive(Serial::IArchive &archive);

        bool Load(const std::filesystem::path &filePath);
        bool Save(const std::filesystem::path &filePath);
    };
}

#endif //FLK_WORLD_HPP
