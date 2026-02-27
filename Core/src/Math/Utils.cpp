#include "Utils.hpp"

namespace Flock {
    static constexpr f64 s_Pi = 3.14159265358979;

    f64 RadiansToDegrees(const f64 angleRadians) { return angleRadians * 180.0 / s_Pi; }
    f64 DegreesToRadians(const f64 angleDegrees) { return angleDegrees * s_Pi / 180.0; }

    f64 GetPi() {
        return s_Pi;
    }
} // namespace Flock
