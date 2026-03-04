#include "Schedule.hpp"

namespace Flock::Ecs {
    void Schedule::Execute(const Stage stage, World &world) {
        for (auto &system: m_Systems[stage]) {
            system(world);
        }
    }

    void Schedule::AddSystem(const Stage stage, const System &system) {
        m_Systems[stage].push_back(system);
    }

    void Schedule::PopSystem(const Stage stage) {
        m_Systems[stage].pop_back();
    }

    std::vector<System> Schedule::GetSystems(const Stage stage) {
        return m_Systems[stage];
    }

    void Schedule::Clear() {
        m_Systems.clear();
    }
}
