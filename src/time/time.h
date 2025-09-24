#pragma once

namespace Engine::Time {
    void SetTime(double time);

    [[nodiscard]] double GetTime();
    [[nodiscard]] double GetDeltaTime();
} // namespace Engine::Time
