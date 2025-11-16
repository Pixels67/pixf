#include "Math.hpp"

namespace Pixf::Core::Math {
    static constexpr double s_Pi = 3.14159265358979;

    double RadiansToDegrees(const double angleRadians) { return angleRadians * 180.0 / s_Pi; }
    double DegreesToRadians(const double angleDegrees) { return angleDegrees * s_Pi / 180.0; }
} // namespace Pixf::Core::Math
