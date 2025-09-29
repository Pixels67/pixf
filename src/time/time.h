#pragma once

namespace pixf::time {
void SetTime(double new_time);

[[nodiscard]] double GetTime();
[[nodiscard]] double GetDeltaTime();
}  // namespace pixf::time
