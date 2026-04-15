#ifndef FLK_QUATERNION_HPP
#define FLK_QUATERNION_HPP

#include <cmath>

#include "Common.hpp"
#include "Utils.hpp"
#include "Vector.hpp"

namespace Flock {
    template<typename T>
    struct Matrix4;

    using Matrix4f = Matrix4<f32>;

    struct FLK_API Quaternion {
        f32 x, y, z, w;

        Quaternion() : x(0), y(0), z(0), w(1) {
        }

        Quaternion(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {
        }

        static Quaternion AngleAxis(const f32 angleDegrees, const Vector3f axis) {
            const f32 halfAngle = DegreesToRadians(-angleDegrees) * 0.5F;
            f32       s         = std::sin(halfAngle);
            return {
                axis.Normalized().x * s,
                axis.Normalized().y * s,
                axis.Normalized().z * s,
                std::cos(halfAngle)
            };
        }

        static Quaternion Euler(const Vector3f eulerAngles) {
            return Euler(eulerAngles.x, eulerAngles.y, eulerAngles.z);
        }

        static Quaternion Euler(const f32 x, const f32 y, const f32 z) {
            const f32 cy = std::cos(DegreesToRadians(-y) * 0.5F);
            const f32 sy = std::sin(DegreesToRadians(-y) * 0.5F);
            const f32 cp = std::cos(DegreesToRadians(-x) * 0.5F);
            const f32 sp = std::sin(DegreesToRadians(-x) * 0.5F);
            const f32 cr = std::cos(DegreesToRadians(-z) * 0.5F);
            const f32 sr = std::sin(DegreesToRadians(-z) * 0.5F);

            return Quaternion(cy * sp * cr + sy * cp * sr, // x
                              sy * cp * cr - cy * sp * sr, // y
                              cy * cp * sr - sy * sp * cr, // z
                              cy * cp * cr + sy * sp * sr  // w
            );
        }

        Quaternion operator*(const Quaternion &o) const {
            return {
                w * o.x + x * o.w + y * o.z - z * o.y, // x
                w * o.y - x * o.z + y * o.w + z * o.x, // y
                w * o.z + x * o.y - y * o.x + z * o.w, // z
                w * o.w - x * o.x - y * o.y - z * o.z, // w
            };
        }

        Quaternion &operator*=(const Quaternion &o) {
            *this = *this * o;
            return *this;
        }

        Vector3f operator*(const Vector3f &o) const {
            const Quaternion quat(o.x, o.y, o.z, 0);
            Quaternion result = Conjugate() * quat * *this;

            return {result.x, result.y, result.z};
        }

        [[nodiscard]] f32 SqrMagnitude() const { return x * x + y * y + z * z + w * w; }

        [[nodiscard]] f32 Magnitude() const { return std::sqrt(SqrMagnitude()); }

        Quaternion &Normalize() {
            if (const f32 mag = Magnitude(); mag > 0.0F) {
                const f32 invMag = 1.0F / mag;
                x                *= invMag;
                y                *= invMag;
                z                *= invMag;
                w                *= invMag;
            }

            return *this;
        }

        [[nodiscard]] Quaternion Normalized() const {
            Quaternion q = *this;
            q.Normalize();
            return q;
        }

        [[nodiscard]] Quaternion Conjugate() const {
            return Quaternion(-x, -y, -z, w);
        }

        [[nodiscard]] Quaternion Inverse() const {
            if (const f32 magSq = SqrMagnitude(); magSq > 0.0F) {
                const f32 invMagSq = 1.0F / magSq;
                return Quaternion(-x * invMagSq, -y * invMagSq, -z * invMagSq, w * invMagSq);
            }

            return Quaternion();
        }

        [[nodiscard]] f32 Dot(const Quaternion &q) const {
            return x * q.x + y * q.y + z * q.z + w * q.w;
        }

        static Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, f32 t) {
            Quaternion qz       = q2;
            f32        cosTheta = q1.Dot(q2);

            if (cosTheta < 0.0F) {
                qz       = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
                cosTheta = -cosTheta;
            }

            if (cosTheta > 0.9995F) {
                return Quaternion(q1.x + t * (qz.x - q1.x),
                                  q1.y + t * (qz.y - q1.y),
                                  q1.z + t * (qz.z - q1.z),
                                  q1.w + t * (qz.w - q1.w))
                        .Normalized();
            }

            const f32 angle    = std::acos(cosTheta);
            const f32 sinTheta = std::sin(angle);
            const f32 w1       = std::sin((1.0F - t) * angle) / sinTheta;
            const f32 w2       = std::sin(t * angle) / sinTheta;

            return Quaternion(q1.x * w1 + qz.x * w2,
                              q1.y * w1 + qz.y * w2,
                              q1.z * w1 + qz.z * w2,
                              q1.w * w1 + qz.w * w2);
        }

        [[nodiscard]] Vector3f EulerAngles() const {
            Vector3f  angles;
            const f32 sp = 2.0F * (w * x - y * z);

            if (constexpr f32 epsilon = 0.9999F; std::abs(sp) >= epsilon) {
                angles.x = (sp > 0) ? (Pi() / 2.0F) : (-Pi() / 2.0F);

                angles.y = 2.0F * std::atan2(y, w);
                angles.z = 0.0F;
            } else {
                angles.x = std::asin(sp);

                const f32 sycy = 2.0F * (w * y + x * z);
                const f32 cycy = 1.0F - 2.0F * (x * x + y * y);
                angles.y       = std::atan2(sycy, cycy);

                const f32 szcz = 2.0F * (w * z + x * y);
                const f32 czcz = 1.0F - 2.0F * (x * x + z * z);
                angles.z       = std::atan2(szcz, czcz);
            }

            angles.x = RadiansToDegrees(angles.x);
            angles.y = RadiansToDegrees(angles.y);
            angles.z = RadiansToDegrees(angles.z);

            return angles;
        }

        [[nodiscard]] Matrix4f ToMatrix() const;
    };
}

#endif // FLK_QUATERNION_HPP
