#pragma once

namespace engine::time {
void SetTime(double new_time);

[[nodiscard]] double GetTime();
[[nodiscard]] double GetDeltaTime();
}  // namespace engine::time
