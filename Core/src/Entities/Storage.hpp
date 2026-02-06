#ifndef FLK_STORAGE_HPP
#define FLK_STORAGE_HPP

#include "Common.hpp"
#include "Entity.hpp"

namespace Flock::Entities {
    /**
     * @class Storage
     * @brief An ECS component storage.
     * @tparam T The type to store.
     */
    template<typename T>
    class Storage {
    public:
        /**
         * @brief Inserts component data at a specified entity ID.
         * @param id The entity ID.
         * @param element The component data.
         */
        void Insert(const EntityId id, T element) {
            if (id >= m_Sparse.size()) {
                m_Sparse.resize(id + 1, ~0u);
            }

            if (m_Sparse[id] != ~0u) {
                m_Data[m_Sparse[id]] = std::move(element);
                return;
            }

            m_Sparse[id] = m_Dense.size();
            m_Dense.push_back(id);
            m_Data.push_back(std::move(element));
        }

        /**
         * @brief Removes component data at a specified entity ID.
         * @param id The entity ID.
         * @return true if successful; false otherwise.
         */
        bool Remove(const EntityId id) {
            if (id >= m_Sparse.size()) {
                return false;
            }

            if (m_Sparse[id] == ~0u) {
                return false;
            }

            usize       idx     = m_Sparse[id];
            const usize lastIdx = m_Dense.size() - 1;

            // Remove
            m_Sparse[id] = ~0u;

            // Swap
            m_Dense[idx] = m_Dense.back();
            m_Data[idx]  = std::move(m_Data.back());

            // Pop
            m_Dense.pop_back();
            m_Data.pop_back();

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
        bool Has(const EntityId id) const {
            return id < m_Sparse.size() && m_Sparse[id] != ~0u;
        }

        /**
         * @brief Retrieves component data at a specified entity ID.
         * @param id The entity ID.
         * @return The component data if found; std::nullopt otherwise.
         */
        OptionalRef<T> Get(const EntityId id) {
            if (!Has(id)) {
                return std::nullopt;
            }

            return m_Data[m_Sparse[id]];
        }

        /**
         * @brief Clears the storage.
         */
        void Clear() {
            m_Sparse.clear();
            m_Dense.clear();
            m_Data.clear();
        }

    private:
        std::vector<usize>    m_Sparse;
        std::vector<EntityId> m_Dense;
        std::vector<T>        m_Data;
    };
}

#endif //FLK_STORAGE_HPP
