#ifndef PIXF_MATRIX_HPP
#define PIXF_MATRIX_HPP

#include <array>
#include <cmath>

#include "Math.hpp"
#include "Quaternion.hpp"
#include "Vector.hpp"

namespace Pixf::Core::Math {
    template<typename T>
    class Matrix4 {
    public:
        std::array<T, 4 * 4> m;

        Matrix4() { m = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }

        static Matrix4 Identity() { return Matrix4(); }

        T &At(const unsigned int row, const unsigned int col) { return m[(row * 4) + col]; }

        const T &At(const unsigned int row, const unsigned int col) const { return m[(row * 4) + col]; }

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

        static Matrix4 Translate(const Vector3<T> &translation) {
            Matrix4 mat;

            mat.At(0, 0) = 1;
            mat.At(1, 1) = 1;
            mat.At(2, 2) = 1;
            mat.At(3, 3) = 1;

            mat.At(0, 3) = translation.x;
            mat.At(1, 3) = translation.y;
            mat.At(2, 3) = translation.z;

            return mat;
        }

        static Matrix4 RotateX(const T &angleDegrees) {
            Matrix4 mat;
            T radians = DegreesToRadians(angleDegrees);

            double sinTheta = std::sin(radians);
            double cosTheta = std::cos(radians);

            mat.At(0, 0) = 1;
            mat.At(3, 3) = 1;
            mat.At(1, 1) = cosTheta;
            mat.At(2, 2) = cosTheta;
            mat.At(1, 2) = -sinTheta;
            mat.At(2, 1) = sinTheta;

            return mat;
        }

        static Matrix4 RotateY(const T &angleDegrees) {
            Matrix4 mat;
            T radians = DegreesToRadians(angleDegrees);

            double sinTheta = std::sin(radians);
            double cosTheta = std::cos(radians);

            mat.At(1, 1) = 1;
            mat.At(3, 3) = 1;
            mat.At(0, 0) = cosTheta;
            mat.At(2, 2) = cosTheta;
            mat.At(0, 2) = -sinTheta;
            mat.At(2, 0) = sinTheta;

            return mat;
        }

        static Matrix4 RotateZ(const T &angleDegrees) {
            Matrix4 mat;
            T radians = DegreesToRadians(angleDegrees);

            double sinTheta = std::sin(radians);
            double cosTheta = std::cos(radians);

            mat.At(0, 0) = cosTheta;
            mat.At(1, 1) = cosTheta;
            mat.At(0, 1) = -sinTheta;
            mat.At(1, 0) = sinTheta;
            mat.At(2, 2) = 1;
            mat.At(3, 3) = 1;

            return mat;
        }

        static Matrix4 Rotate(const Vector3<T> &eulerAngles) {
            return RotateY(eulerAngles.y) * RotateX(eulerAngles.x) * RotateZ(eulerAngles.z);
        }

        static Matrix4 Rotate(T angleDegrees, Vector3<T> axis) {
            axis.Normalize();
            return RotateY(axis.y * angleDegrees) * RotateX(axis.x * angleDegrees) * RotateZ(axis.z * angleDegrees);
        }

        static Matrix4 Rotate(const Quaternion &quaternion) {
            Matrix4 mat;
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
            mat.At(0, 1) = 2 * (xy - wz);
            mat.At(0, 2) = 2 * (xz + wy);
            mat.At(1, 0) = 2 * (xy + wz);
            mat.At(1, 1) = 2 * (ww + yy) - 1;
            mat.At(1, 2) = 2 * (xz - wx);
            mat.At(2, 0) = 2 * (xz - wy);
            mat.At(2, 1) = 2 * (yz + wx);
            mat.At(2, 2) = 2 * (ww + zz) - 1;

            return mat;
        }

        static Matrix4 Scale(const Vector3<T> &scale) {
            Matrix4 mat;

            mat.At(0, 0) = scale.x;
            mat.At(1, 1) = scale.y;
            mat.At(2, 2) = scale.z;
            mat.At(3, 3) = 1;

            return mat;
        }

        static Matrix4 Perspective(const T &fovY, const T &aspect, const T &nearZ, const T &farZ) {
            Matrix4 mat;
            mat.m.fill(0);
            double radians = DegreesToRadians(fovY);

            T tanHalfFovY = std::tan(radians / static_cast<T>(2));

            mat.At(0, 0) = 1 / (aspect * tanHalfFovY);
            mat.At(1, 1) = 1 / tanHalfFovY;
            mat.At(2, 2) = farZ / (farZ - nearZ);
            mat.At(2, 3) = -(farZ * nearZ) / (farZ - nearZ);
            mat.At(3, 2) = 1;

            return mat;
        }

        static Matrix4 Orthographic(T left, T right, T bottom, T top, T nearZ, T farZ) {
            Matrix4 mat;
            mat.m.fill(0);

            mat.At(0, 0) = 2 / (right - left);
            mat.At(1, 1) = 2 / (top - bottom);
            mat.At(2, 2) = 1 / (farZ - nearZ);
            mat.At(0, 3) = -(right + left) / (right - left);
            mat.At(1, 3) = -(top + bottom) / (top - bottom);
            mat.At(2, 3) = -nearZ / (farZ - nearZ);
            mat.At(3, 3) = 1;

            return mat;
        }
    };

    using Matrix4f = Matrix4<float>;
    using Matrix4d = Matrix4<double>;
} // namespace Pixf::Core::Math

inline Pixf::Core::Math::Matrix4f Pixf::Core::Math::Quaternion::ToMatrix() const { return Matrix4f::Rotate(*this); }

#endif // PIXF_MATRIX_HPP
