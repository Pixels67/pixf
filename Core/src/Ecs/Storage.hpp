#ifndef FLK_STORAGE_HPP
#define FLK_STORAGE_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Ecs {
    struct ComponentConfig {
        bool enabled = true;
    };

    FLK_ARCHIVE(ComponentConfig, enabled)

    class IStorage {
    public:
        virtual ~IStorage() = default;

        [[nodiscard]] virtual bool     Has(EntityId id) const = 0;
        [[nodiscard]] virtual bool     IsEnabled(EntityId id) const = 0;
        virtual bool                   SetEnabled(EntityId id, bool enabled) = 0;
        virtual void                   SetAllEnabled(bool enabled) = 0;
        virtual bool                   Remove(EntityId id) = 0;
        virtual void                   Clear() = 0;
        virtual std::vector<EntityId> &Dense() = 0;
    };

    /**
     * @class Storage
     * @brief An ECS component storage.
     * @tparam T The type to store.
     */
    template<typename T>
    class Storage : public IStorage {
        std::vector<usize>           m_Sparse;
        std::vector<EntityId>        m_Dense;
        std::vector<T>               m_Data;
        std::vector<ComponentConfig> m_Configs;

    public:
        /**
         * @brief Inserts component data at a specified entity ID.
         * @param id The entity ID.
         * @param element The component data.
         */
        void Insert(const EntityId id, T element) {
            if (id >= m_Sparse.size()) {
                m_Sparse.resize(id + 1, FLK_INVALID);
            }

            if (m_Sparse[id] != FLK_INVALID) {
                m_Data[m_Sparse[id]]    = std::move(element);
                m_Configs[m_Sparse[id]] = {};
                return;
            }

            m_Sparse[id] = m_Dense.size();
            m_Dense.push_back(id);
            m_Data.push_back(std::move(element));
            m_Configs.push_back({});
        }

        /**
         * @brief Removes component data at a specified entity ID.
         * @param id The entity ID.
         * @return true if successful; false otherwise.
         */
        bool Remove(const EntityId id) override {
            if (id >= m_Sparse.size()) {
                return false;
            }

            if (m_Sparse[id] == FLK_INVALID) {
                return false;
            }

            usize       idx     = m_Sparse[id];
            const usize lastIdx = m_Dense.size() - 1;

            // Remove
            m_Sparse[id] = FLK_INVALID;

            // Swap
            m_Dense[idx]   = m_Dense.back();
            m_Data[idx]    = std::move(m_Data.back());
            m_Configs[idx] = std::move(m_Configs.back());

            // Pop
            m_Dense.pop_back();
            m_Data.pop_back();
            m_Configs.pop_back();

            // Handle swapped id in sparse set
            if (idx != lastIdx) {
                m_Sparse[m_Dense[idx]] = idx;
            }

            return true;
        }

        /**
         * @brief Whether there is component data or not at a specified entity ID.
         * @param id The entity ID.
         * @return true if there is data at id; false otherwise.
         */
        [[nodiscard]] bool Has(const EntityId id) const override {
            return id < m_Sparse.size() && m_Sparse[id] != FLK_INVALID;
        }

        /**
         * @brief Retrieves component data at a specified entity ID.
         * @param id The entity ID.
         * @return The component data if found; nullptr otherwise.
         */
        T *Get(const EntityId id) {
            if (!Has(id)) {
                return nullptr;
            }

            return &m_Data[m_Sparse[id]];
        }

        /**
         * @brief Whether the component is enabled or not.
         * @param id The entity ID.
         * @return true if the component at id is enabled; false otherwise.
         */
        [[nodiscard]] bool IsEnabled(const EntityId id) const override {
            if (!Has(id)) {
                return false;
            }

            return m_Configs[m_Sparse[id]].enabled;
        }

        /**
         * @brief Sets the component enabled state.
         * @param id The entity ID.
         * @param enabled The boolean value to set.
         * @return true successful; false otherwise.
         */
        bool SetEnabled(const EntityId id, const bool enabled) override {
            if (!Has(id)) {
                return false;
            }

            m_Configs[m_Sparse[id]].enabled = enabled;
            return true;
        }

        /**
         * @brief Sets all components enabled state.
         * @param enabled The boolean value to set.
         * @return true successful; false otherwise.
         */
        void SetAllEnabled(const bool enabled) override {
            for (auto &cfg: m_Configs) {
                cfg.enabled = enabled;
            }
        }

        /**
         * @brief Retrieves a reference to all the values inside the storage.
         * @return The storage data.
         */
        std::vector<T> &Data() {
            return m_Data;
        }

        /**
         * @brief Retrieves a reference to all the entity IDs inside the storage.
         * @return The storage entity IDs.
         */
        std::vector<EntityId> &Dense() override {
            return m_Dense;
        }

        /**
         * @brief Clears the storage.
         */
        void Clear() override {
            m_Sparse.clear();
            m_Dense.clear();
            m_Data.clear();
        }

        void Archive(Serial::IArchive &archive) {
            usize count = m_Dense.size();
            archive.BeginArray(NameOf(T{}), count);

            m_Dense.resize(count);
            m_Data.resize(count);
            m_Configs.resize(count);

            for (usize i = 0; i < count; i++) {
                archive.BeginObject();
                archive("id", m_Dense[i]);
                archive("data", m_Data[i]);
                archive("config", m_Configs[i]);
                archive.EndObject();
            }

            archive.EndArray();

            m_Sparse.clear();
            for (usize i = 0; i < m_Dense.size(); i++) {
                const EntityId id = m_Dense[i];
                if (id >= m_Sparse.size()) {
                    m_Sparse.resize(id + 1, FLK_INVALID);
                }

                m_Sparse[id] = i;
            }
        }
    };
}

#endif //FLK_STORAGE_HPP
