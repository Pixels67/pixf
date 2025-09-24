#include "time.h"

namespace Engine::Time {
    static double Time = 0.0f;
    static double DeltaTime = 0.0f;

    void SetTime(const double time) {
        DeltaTime = time - Time;
        Time = time;
    }

    double GetTime() {
        return Time;
    }

    double GetDeltaTime() {
        return DeltaTime;
    }
} // namespace Engine::Time