#ifndef FLK_SCHEDULE_HPP
#define FLK_SCHEDULE_HPP

#include "Common.hpp"
#include "Registry.hpp"

namespace Flock::Ecs {
    using System = std::function<void(Registry &)>;

    /**
     * @enum Stage
     * @brief Execution stage.
     */
    enum class Stage {
        Startup,
        Update
    };

    /**
     * @class Schedule
     * @brief Contains the ECS systems.
     */
    class FLK_API Schedule {
        std::unordered_map<Stage, std::vector<System> > m_Systems;

    public:
        /**
         * @brief Executes a stage of the schedule.
         * @param stage The stage to execute.
         * @param registry The registry to run the systems on.
         */
        void Execute(Stage stage, Registry &registry);

        /**
         * @brief Adds a system to a stage.
         * @param stage The stage.
         * @param system The system.
         */
        void AddSystem(Stage stage, const System &system);

        /**
         * @brief Pops the last added system from a stage.
         * @param stage The stage.
         */
        void PopSystem(Stage stage);

        /**
         * @brief Retrieves an ordered vector of all the systems in a stage.
         * @param stage The stage.
         * @return All the systems in the stage.
         */
        std::vector<System> GetSystems(Stage stage);

        /**
         * @brief Clears the schedule.
         */
        void Clear();
    };
}

#endif //FLK_SCHEDULE_HPP
