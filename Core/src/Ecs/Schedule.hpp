#ifndef FLK_SCHEDULE_HPP
#define FLK_SCHEDULE_HPP

#include <functional>
#include <unordered_map>
#include <vector>

#include "Common.hpp"
#include "World.hpp"

namespace Flock::Ecs {
    using System = std::function<void(World &)>;

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
         * @param world The world to run the systems on.
         */
        void Execute(Stage stage, World &world);

        /**
         * @brief Adds a system to a stage.
         * @param stage The stage.
         * @param system The system.
         */
        void AddSystem(Stage stage, const System &system);

        /**
         * Adds multiple systems to a stage; executed in order.
         * @tparam Args The system types.
         * @param stage The stage.
         * @param args The systems.
         */
        template<typename... Args>
        void AddSystems(Stage stage, const Args &... args) {
            (AddSystem(stage, args), ...);
        }

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
        std::vector<System> Systems(Stage stage);

        /**
         * @brief Clears the schedule.
         */
        void Clear();
    };
}

#endif //FLK_SCHEDULE_HPP
