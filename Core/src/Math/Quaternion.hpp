#ifndef PIXF_QUATERNION_HPP
#define PIXF_QUATERNION_HPP

#include <cmath>

#include "Common.hpp"
#include "Math.hpp"
#include "Vector.hpp"

namespace Pixf::Core::Math {
    template<typename T>
    class Matrix4;

    using Matrix4f = Matrix4<float>;

    class PIXF_API Quaternion {
    public:
        float x, y, z, w;

        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        static Quaternion AngleAxis(const float angleDegrees, const Vector3f axis) {
            const float halfAngle = DegreesToRadians(angleDegrees) * 0.5F;
            float s = std::sin(halfAngle);
            return Quaternion(axis.Normalized().x * s,
                              axis.Normalized().y * s,
                              axis.Normalized().z * s,
                              std::cos(halfAngle));
        }

        static Quaternion Euler(const Vector3f eulerAngles) {
            return Euler(eulerAngles.x, eulerAngles.y, eulerAngles.z);
        }

        static Quaternion Euler(const float pitch, const float yaw, const float roll) {
            const float cy = std::cos(yaw * 0.5F);
            const float sy = std::sin(yaw * 0.5F);
            const float cp = std::cos(pitch * 0.5F);
            const float sp = std::sin(pitch * 0.5F);
            const float cr = std::cos(roll * 0.5F);
            const float sr = std::sin(roll * 0.5F);

            return Quaternion(sr * cp * cy - cr * sp * sy, // x
                              cr * sp * cy + sr * cp * sy, // y
                              cr * cp * sy - sr * sp * cy, // z
                              cr * cp * cy + sr * sp * sy // w
            );
        }

        Quaternion operator*(const Quaternion &o) const {
            return Quaternion(w * o.x + x * o.w + y * o.z - z * o.y, // x
                              w * o.y - x * o.z + y * o.w + z * o.x, // y
                              w * o.z + x * o.y - y * o.x + z * o.w, // z
                              w * o.w - x * o.x - y * o.y - z * o.z); // w
        }

        Quaternion &operator*=(const Quaternion &o) {
            *this = *this * o;
            return *this;
        }

        Vector3f operator*(const Vector3f &o) const {
            Quaternion quat(o.x, o.y, o.z, 0);
            Quaternion result = Normalized() * quat * Conjugate().Normalize();

            return {result.x, result.y, result.z};
        }

        float SqrMagnitude() const { return x * x + y * y + z * z + w * w; }

        float Magnitude() const { return std::sqrt(SqrMagnitude()); }

        Quaternion &Normalize() {
            if (const float mag = Magnitude(); mag > 0.0F) {
                const float invMag = 1.0F / mag;
                x *= invMag;
                y *= invMag;
                z *= invMag;
                w *= invMag;
            }

            return *this;
        }

        Quaternion Normalized() const {
            Quaternion q = *this;
            q.Normalize();
            return q;
        }

        Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }

        Quaternion Inverse() const {
            if (const float magSq = SqrMagnitude(); magSq > 0.0F) {
                const float invMagSq = 1.0F / magSq;
                return Quaternion(-x * invMagSq, -y * invMagSq, -z * invMagSq, w * invMagSq);
            }

            return Quaternion();
        }

        float Dot(const Quaternion &q) const { return x * q.x + y * q.y + z * q.z + w * q.w; }

        static Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, float t) {
            Quaternion qz = q2;
            float cosTheta = q1.Dot(q2);

            if (cosTheta < 0.0F) {
                qz = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
                cosTheta = -cosTheta;
            }

            if (cosTheta > 0.9995F) {
                return Quaternion(q1.x + t * (qz.x - q1.x),
                                  q1.y + t * (qz.y - q1.y),
                                  q1.z + t * (qz.z - q1.z),
                                  q1.w + t * (qz.w - q1.w))
                        .Normalized();
            }

            const float angle = std::acos(cosTheta);
            const float sinTheta = std::sin(angle);
            const float w1 = std::sin((1.0F - t) * angle) / sinTheta;
            const float w2 = std::sin(t * angle) / sinTheta;

            return Quaternion(q1.x * w1 + qz.x * w2,
                              q1.y * w1 + qz.y * w2,
                              q1.z * w1 + qz.z * w2,
                              q1.w * w1 + qz.w * w2);
        }

        Vector3f EulerAngles() const {
            Vector3f angles;
            const float sp = 2.0F * (w * y - z * x);

            if (constexpr float epsilon = 0.9999F; std::abs(sp) >= epsilon) {
                angles.y = (sp > 0) ? (GetPi() / 2.0F) : (-GetPi() / 2.0F);

                angles.x = -2.0F * std::atan2(x, w); // or 2.0F * atan2(x, w)
                angles.z = 0.0F;

            } else {
                angles.y = std::asin(sp);

                const float srcp = 2.0F * (w * z + x * y);
                const float crcp = 1.0F - 2.0F * (y * y + z * z);
                angles.z = std::atan2(srcp, crcp);
                const float sycp = 2.0F * (w * x + y * z);
                const float cycp = 1.0F - 2.0F * (x * x + y * y);
                angles.x = std::atan2(sycp, cycp);
            }

            return angles;
        }

        Matrix4f ToMatrix() const;
    };

    PIXF_SERIALIZE(Quaternion,
        PIXF_FIELD(x, Quaternion.x);
        PIXF_FIELD(y, Quaternion.y);
        PIXF_FIELD(z, Quaternion.z);
        PIXF_FIELD(w, Quaternion.w);
    )
} // namespace Pixf::Core::Math

#endif // PIXF_QUATERNION_HPP
