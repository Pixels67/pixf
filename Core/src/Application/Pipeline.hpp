#ifndef PIXF_PIPELINE_HPP
#define PIXF_PIPELINE_HPP

#include "Common.hpp"
#include "Stage.hpp"
#include "TypeId.hpp"

namespace Pixf::Core::Application {
    class PIXF_API Pipeline {
    public:
        Pipeline() = default;

        Pipeline(const Pipeline &) = delete;
        Pipeline(Pipeline &&) noexcept = default;
        Pipeline &operator=(const Pipeline &) = delete;
        Pipeline &operator=(Pipeline &&) noexcept = default;

        ~Pipeline() = default;

        void Update(Context &context, double deltaTime);
        void Render(Context &context, double deltaTime);
        void RenderGui(Context &context, double deltaTime);

        void OnEvent(Context &context, Event::Event &event);

        template<typename T>
        void Attach(Context &context) {
            static_assert(std::is_base_of<Stage, T>());

            TypeId typeId = GetTypeId<T>();
            m_Stages.push_back(std::make_pair(typeId, std::make_unique<T>()));
            m_Stages.back().second->OnAttach(context);
        }

        template<typename T>
        void Attach(Context &context, T stage) {
            static_assert(std::is_base_of<Stage, T>());

            TypeId typeId = GetTypeId<T>();
            m_Stages.push_back(std::make_pair(typeId, std::make_unique<T>(stage)));
            m_Stages.back().second->OnAttach(context);
        }

        template<typename T>
        void Detach(Context &context) {
            const TypeId typeId = GetTypeId<T>();

            for (size_t i = 0; i < m_Stages.size(); i++) {
                if (m_Stages[i].first != typeId) {
                    continue;
                }

                m_Stages[i].second->OnDetach(context);
                m_Stages.erase(m_Stages.begin() + i);
                break;
            }
        }

    private:
        std::vector<std::pair<TypeId, std::unique_ptr<Stage>>> m_Stages;
    };
} // namespace Pixf::Core::Application

#endif // PIXF_PIPELINE_HPP
