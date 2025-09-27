#include "time.h"

namespace engine::time {
static double time = 0.0f;
static double delta_time = 0.0f;

void SetTime(const double new_time) {
  delta_time = new_time - time;
  time = new_time;
}

double GetTime() { return time; }

double GetDeltaTime() { return delta_time; }
}  // namespace engine::time