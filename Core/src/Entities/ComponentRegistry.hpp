#ifndef COMPONENTREGISTRY_HPP
#define COMPONENTREGISTRY_HPP

#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Error/Result.hpp"
#include "Serialization/Serializable.hpp"

namespace Pixf::Core::Entities {
    struct PIXF_API Component {
        virtual ~Component() = default;
    };

    enum class ComponentRegistryError : uint8_t {
        None = 0,
        NotFound,
    };

    class PIXF_API IComponentRegistry : public Serialization::Serializable {
    public:
        ~IComponentRegistry() override = default;

        virtual void Add(size_t index) = 0;
        virtual bool Has(size_t index) const = 0;
        virtual void Remove(size_t index) = 0;
        virtual void Clear() = 0;

        virtual const char *GetTypeName() = 0;
        virtual uint64_t GetTypeId() = 0;

        virtual Serialization::Json::object SerializeElement(size_t index, bool editorMode = false) = 0;
        virtual void DeserializeElement(Serialization::Json::object json, Assets::AssetManager &assetManager,
                                        size_t index, bool editorMode = false) = 0;
    };

    template<TypeInformed T>
    class ComponentRegistry final : public IComponentRegistry {
    public:
        ComponentRegistry() = default;

        ComponentRegistry(const ComponentRegistry &) = delete;
        ComponentRegistry(ComponentRegistry &&) = delete;
        ComponentRegistry &operator=(const ComponentRegistry &) = delete;
        ComponentRegistry &operator=(ComponentRegistry &&) = delete;

        ~ComponentRegistry() override = default;

        void Add(const size_t index) override {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            m_Components.push_back(std::make_shared<T>());
            m_DenseArray.push_back(index);

            if (index >= m_SparseArray.size()) {
                m_SparseArray.resize(index + 1);
            }

            m_SparseArray[index] = m_DenseArray.size() - 1;
        }

        void Add(const size_t index, T component) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (index < m_SparseArray.size() && m_SparseArray[index] != std::nullopt) {
                if (const size_t dense = m_SparseArray[index].value(); dense < m_Components.size()) {
                    m_Components[dense] = std::make_shared<T>(component);
                    return;
                }
            }

            m_Components.push_back(std::make_shared<T>(component));
            m_DenseArray.push_back(index);

            if (index >= m_SparseArray.size()) {
                m_SparseArray.resize(index + 1);
            }

            m_SparseArray[index] = m_DenseArray.size() - 1;
        }

        bool Has(const size_t index) const override {
            return index < m_SparseArray.size() && m_SparseArray[index].has_value();
        }

        Error::Result<std::shared_ptr<T>, ComponentRegistryError> Get(const size_t index) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (!Has(index)) {
                return ComponentRegistryError::NotFound;
            }

            return std::static_pointer_cast<T>(m_Components[m_SparseArray[index].value()]);
        }

        void Remove(const size_t index) override {
            if (!Has(index)) {
                return;
            }

            size_t denseIndex = m_SparseArray.at(index).value();
            const size_t lastDenseIndex = m_DenseArray.size() - 1;
            const size_t lastSparseIndex = m_DenseArray.back();

            m_Components.at(denseIndex).reset();

            if (denseIndex != lastDenseIndex) {
                m_Components.at(denseIndex) = std::move(m_Components.back());
                m_DenseArray.at(denseIndex) = m_DenseArray.back();

                m_SparseArray.at(lastSparseIndex) = denseIndex;
            }

            m_Components.pop_back();
            m_DenseArray.pop_back();

            m_SparseArray.at(index) = std::nullopt;
        }

        void Clear() override {
            m_Components.clear();
            m_DenseArray.clear();
            m_SparseArray.clear();
        }

        std::map<size_t, std::shared_ptr<T>> GetAll() const {
            std::map<size_t, std::shared_ptr<T>> result;
            for (size_t i = 0; i < m_DenseArray.size(); i++) {
                result[m_DenseArray[i]] = std::static_pointer_cast<T>(m_Components.at(i));
            }

            return result;
        }

        Serialization::Json::object Serialize(bool editorMode = false) override {
            Serialization::Json::object json = {};

            if constexpr (Serialization::SerializableType<T>) {
                for (auto &[id, component]: GetAll()) {
                    if (!component)
                        continue;
                    json[std::to_string(id)] = component->Serialize(editorMode);
                }
            }

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         bool editorMode = false) override {
            Clear();

            if constexpr (Serialization::SerializableType<T>) {
                for (auto &[key, value]: json) {
                    size_t id = std::stoull(key);

                    T component{};
                    component.Deserialize(value.as_object(), assetManager, editorMode);
                    Add(id, component);
                }
            }
        }

        Serialization::Json::object SerializeElement(const size_t index, bool editorMode = false) override {
            Serialization::Json::object json = {};

            if constexpr (Serialization::SerializableType<T>) {
                json = Get(index).Unwrap()->Serialize(editorMode);
            }

            return json;
        }

        void DeserializeElement(Serialization::Json::object json, Assets::AssetManager &assetManager,
                                const size_t index, bool editorMode = false) override {
            if constexpr (Serialization::SerializableType<T>) {
                T component{};
                component.Deserialize(json, assetManager, editorMode);
                Add(index, component);
            }
        }

        const char *GetTypeName() override {
            if constexpr (Serialization::SerializableType<T>) {
                return T::GetTypeName();
            }

            return nullptr;
        }

        uint64_t GetTypeId() override { return HashString(GetTypeName()); }

    private:
        std::vector<std::optional<size_t>> m_SparseArray;
        std::vector<size_t> m_DenseArray;
        std::vector<std::shared_ptr<Component>> m_Components;
    };
} // namespace Pixf::Core::Entities

#endif // COMPONENTREGISTRY_HPP
