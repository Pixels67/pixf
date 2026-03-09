#ifndef FLK_MATRIX_HPP
#define FLK_MATRIX_HPP

#include <array>
#include <cmath>

#include "Utils.hpp"
#include "Quaternion.hpp"
#include "Vector.hpp"

namespace Flock {
    template<typename T>
    class FLK_API Matrix4 {
    public:
        std::array<T, 4 * 4> m;

        Matrix4() { m = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }

        Matrix4(std::array<T, 16> array) {
            m = array;
        }

        static Matrix4 Identity() { return Matrix4(); }

        T &At(const u32 row, const u32 col) { return m[(row * 4) + col]; }

        const T &At(const u32 row, const u32 col) const { return m[(row * 4) + col]; }

        const T *Data() const { return m.data(); }

        Matrix4 operator*(const Matrix4 &other) {
            // C[i][j] = Σ(k=0 to n-1) A[i][k] * B[k][j]

            Matrix4 mat;

            for (size_t i = 0; i < 4; i++) {
                for (size_t j = 0; j < 4; j++) {
                    T sum = 0;
                    for (size_t k = 0; k < 4; k++) {
                        sum += At(i, k) * other.At(k, j);
                    }

                    mat.At(i, j) = sum;
                }
            }

            return mat;
        }

        static Matrix4 Translate(const Vector3<T> &t) {
            Matrix4 mat;
            mat.m = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                t.x, t.y, t.z, 1
            };
            return mat;
        }

        static Matrix4 RotateX(const T &angleDegrees) {
            Matrix4 mat;
            T       radians = DegreesToRadians(angleDegrees);

            f64 sinTheta = std::sin(radians);
            f64 cosTheta = std::cos(radians);

            mat.At(0, 0) = 1;
            mat.At(3, 3) = 1;
            mat.At(1, 1) = cosTheta;
            mat.At(2, 2) = cosTheta;
            mat.At(1, 2) = sinTheta;
            mat.At(2, 1) = -sinTheta;

            return mat;
        }

        static Matrix4 RotateY(const T &angleDegrees) {
            Matrix4 mat;
            T       radians = DegreesToRadians(angleDegrees);

            f64 sinTheta = std::sin(radians);
            f64 cosTheta = std::cos(radians);

            mat.At(1, 1) = 1;
            mat.At(3, 3) = 1;
            mat.At(0, 0) = cosTheta;
            mat.At(2, 2) = cosTheta;
            mat.At(0, 2) = sinTheta;
            mat.At(2, 0) = -sinTheta;

            return mat;
        }

        static Matrix4 RotateZ(const T &angleDegrees) {
            Matrix4 mat;
            T       radians = DegreesToRadians(angleDegrees);

            f64 sinTheta = std::sin(radians);
            f64 cosTheta = std::cos(radians);

            mat.At(0, 0) = cosTheta;
            mat.At(1, 1) = cosTheta;
            mat.At(0, 1) = sinTheta;
            mat.At(1, 0) = -sinTheta;
            mat.At(2, 2) = 1;
            mat.At(3, 3) = 1;

            return mat;
        }

        static Matrix4 Rotate(const Vector3<T> &eulerAngles) {
            return RotateY(eulerAngles.y) * RotateX(eulerAngles.x) * RotateZ(eulerAngles.z);
        }

        static Matrix4 Rotate(T angleDegrees, Vector3<T> axis) {
            axis.Normalize();
            return RotateY(axis.y * -angleDegrees) * RotateX(axis.x * -angleDegrees) * RotateZ(axis.z * -angleDegrees);
        }

        static Matrix4 Rotate(const Quaternion &quaternion) {
            Matrix4    mat;
            Quaternion q = quaternion;

            T ww = q.w * q.w;
            T xx = q.x * q.x;
            T yy = q.y * q.y;
            T zz = q.z * q.z;

            T xy = q.x * q.y;
            T xz = q.x * q.z;
            T yz = q.y * q.z;
            T wx = q.w * q.x;
            T wy = q.w * q.y;
            T wz = q.w * q.z;

            mat.At(0, 0) = 2 * (ww + xx) - 1;
            mat.At(1, 0) = 2 * (xy - wz);
            mat.At(2, 0) = 2 * (xz + wy);
            mat.At(0, 1) = 2 * (xy + wz);
            mat.At(1, 1) = 2 * (ww + yy) - 1;
            mat.At(2, 1) = 2 * (xz - wx);
            mat.At(0, 2) = 2 * (xz - wy);
            mat.At(1, 2) = 2 * (yz + wx);
            mat.At(2, 2) = 2 * (ww + zz) - 1;

            return mat;
        }

        static Matrix4 LookAt(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
            Vector3<T> z = (target - eye).Normalized();
            Vector3<T> x = up.Cross(z).Normalized();
            Vector3<T> y = z.Cross(x);

            Matrix4 m;
            m.m.fill(0);

            m.At(0, 0) = x.x;
            m.At(1, 0) = x.y;
            m.At(2, 0) = x.z;
            m.At(0, 1) = y.x;
            m.At(1, 1) = y.y;
            m.At(2, 1) = y.z;
            m.At(0, 2) = z.x;
            m.At(1, 2) = z.y;
            m.At(2, 2) = z.z;

            m.At(3, 0) = -x.Dot(eye);
            m.At(3, 1) = -y.Dot(eye);
            m.At(3, 2) = -z.Dot(eye);
            m.At(3, 3) = 1;

            return m;
        }

        static Matrix4 Scale(const Vector3<T> &scale) {
            Matrix4 mat;

            mat.At(0, 0) = scale.x;
            mat.At(1, 1) = scale.y;
            mat.At(2, 2) = scale.z;
            mat.At(3, 3) = 1;

            return mat;
        }

        static Matrix4 Perspective(T fovY, T aspect, T nearPlane, T farPlane) {
            Matrix4 m;
            m.m.fill(0);
            T y = static_cast<T>(1) / std::tan(DegreesToRadians(fovY) / static_cast<T>(2));
            T x = y / aspect;

            m.At(0, 0) = x;
            m.At(1, 1) = y;
            m.At(2, 2) = (farPlane + nearPlane) / (farPlane - nearPlane);
            m.At(2, 3) = 1;
            m.At(3, 2) = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);

            return m;
        }

        static Matrix4 Orthographic(T left, T right, T bottom, T top, T nearPlane, T farPlane) {
            Matrix4 m;
            m.m.fill(0);
            m.At(0, 0) = 2 / (right - left);
            m.At(1, 1) = 2 / (top - bottom);
            m.At(2, 2) = 2 / (farPlane - nearPlane);
            m.At(3, 0) = -(right + left) / (right - left);
            m.At(3, 1) = -(top + bottom) / (top - bottom);
            m.At(3, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
            m.At(3, 3) = 1;

            return m;
        }
    };

    using Matrix4f = Matrix4<f32>;
    using Matrix4d = Matrix4<f64>;
} // namespace Flock

inline Flock::Matrix4f Flock::Quaternion::ToMatrix() const { return Matrix4f::Rotate(*this); }

#endif // FLK_MATRIX_HPP
