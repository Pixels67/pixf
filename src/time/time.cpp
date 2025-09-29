#include "time.h"

namespace pixf::time {
static double time = 0.0F;
static double delta_time = 0.0F;

void SetTime(const double new_time) {
  delta_time = new_time - time;
  time = new_time;
}

double GetTime() { return time; }

double GetDeltaTime() { return delta_time; }
}  // namespace pixf::time