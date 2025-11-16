#ifndef PIXF_MATRIX_HPP
#define PIXF_MATRIX_HPP

#include <array>
#include <cmath>

#include "Quaternion.hpp"
#include "Vector.hpp"

namespace Pixf::Core::Math {
    template<typename T>
    class Matrix4 {
    public:
        std::array<T, 4 * 4> m;

        Matrix4() { *this = Identity(); }

        static Matrix4 Identity() {
            Matrix4 mat;
            mat.m = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
            return mat;
        }

        T &At(const unsigned int row, const unsigned int col) { return m[col * 4 + row]; }
        const T &At(const unsigned int row, const unsigned int col) const { return m[col * 4 + row]; }

        const T *Data() const { return m.data(); }

        Matrix4 operator*(const Matrix4 &other) const {
            Matrix4 result;
            for (unsigned int row = 0; row < 4; ++row) {
                for (unsigned int col = 0; col < 4; ++col) {
                    T sum = 0.0F;
                    for (unsigned int k = 0; k < 4; ++k) {
                        sum += At(row, k) * other.At(k, col);
                    }

                    result.At(row, col) = sum;
                }
            }

            return result;
        }

        Matrix4 &operator*=(const Matrix4 &other) {
            *this = *this * other;
            return *this;
        }

        static Matrix4 Perspective(T fovY, T aspect, T near, T far) {
            Matrix4 mat;
            T tanHalfFovY = std::tan(fovY / 2.0F);

            mat.m.fill(0.0F);
            mat.At(0, 0) = 1.0F / (aspect * tanHalfFovY);
            mat.At(1, 1) = 1.0F / tanHalfFovY;
            mat.At(2, 2) = -(far + near) / (far - near);
            mat.At(2, 3) = -1.0F;
            mat.At(3, 2) = -(2.0F * far * near) / (far - near);

            return mat;
        }

        static Matrix4 Orthographic(T left, T right, T bottom, T top, T near, T far) {
            Matrix4 mat = Identity();

            mat.At(0, 0) = 2.0F / (right - left);
            mat.At(1, 1) = 2.0F / (top - bottom);
            mat.At(2, 2) = -2.0F / (far - near);
            mat.At(3, 0) = -(right + left) / (right - left);
            mat.At(3, 1) = -(top + bottom) / (top - bottom);
            mat.At(3, 2) = -(far + near) / (far - near);

            return mat;
        }

        static Matrix4 Translate(Vector3<T> v) {
            Matrix4 mat = Identity();
            mat.At(3, 0) = v.x;
            mat.At(3, 1) = v.y;
            mat.At(3, 2) = v.z;

            return mat;
        }

        Matrix4 &ApplyTranslation(Vector3<T> v) {
            *this = *this * Translate(v);
            return *this;
        }

        static Matrix4 RotateX(T angle) {
            Matrix4 mat;
            mat.Identity();
            T c = std::cos(angle);
            T s = std::sin(angle);
            mat.At(1, 1) = c;
            mat.At(1, 2) = s;
            mat.At(2, 1) = -s;
            mat.At(2, 2) = c;
            return mat;
        }

        static Matrix4 RotateY(T angle) {
            Matrix4 mat;
            mat.Identity();
            T c = std::cos(angle);
            T s = std::sin(angle);
            mat.At(0, 0) = c;
            mat.At(0, 2) = -s;
            mat.At(2, 0) = s;
            mat.At(2, 2) = c;
            return mat;
        }

        static Matrix4 RotateZ(T angle) {
            Matrix4 mat;
            mat.Identity();
            T c = std::cos(angle);
            T s = std::sin(angle);
            mat.At(0, 0) = c;
            mat.At(0, 1) = s;
            mat.At(1, 0) = -s;
            mat.At(1, 1) = c;
            return mat;
        }

        static Matrix4 Rotate(T angle, Vector3<T> axis) {
            Matrix4 mat;
            T c = std::cos(angle);
            T s = std::sin(angle);
            T t = 1.0f - c;

            mat.At(0, 0) = t * axis.x * axis.x + c;
            mat.At(0, 1) = t * axis.x * axis.y + s * axis.z;
            mat.At(0, 2) = t * axis.x * axis.z - s * axis.y;
            mat.At(0, 3) = 0.0F;

            mat.At(1, 0) = t * axis.x * axis.y - s * axis.z;
            mat.At(1, 1) = t * axis.y * axis.y + c;
            mat.At(1, 2) = t * axis.y * axis.z + s * axis.x;
            mat.At(1, 3) = 0.0F;

            mat.At(2, 0) = t * axis.x * axis.z + s * axis.y;
            mat.At(2, 1) = t * axis.y * axis.z - s * axis.x;
            mat.At(2, 2) = t * axis.z * axis.z + c;
            mat.At(2, 3) = 0.0f;

            mat.At(3, 0) = 0.0F;
            mat.At(3, 1) = 0.0F;
            mat.At(3, 2) = 0.0F;
            mat.At(3, 3) = 1.0F;

            return mat;
        }

        Matrix4 &ApplyRotation(T angle, Vector3<T> v) {
            *this = *this * Rotate(angle, v.x, v.y, v.z);
            return *this;
        }

        static Matrix4 Rotate(const Quaternion &q) {
            Matrix4 mat;

            float xx = q.x * q.x;
            float yy = q.y * q.y;
            float zz = q.z * q.z;
            float xy = q.x * q.y;
            float xz = q.x * q.z;
            float yz = q.y * q.z;
            float wx = q.w * q.x;
            float wy = q.w * q.y;
            float wz = q.w * q.z;

            mat.At(0, 0) = 1.0F - 2.0F * (yy + zz);
            mat.At(0, 1) = 2.0F * (xy + wz);
            mat.At(0, 2) = 2.0F * (xz - wy);
            mat.At(0, 3) = 0.0F;

            mat.At(1, 0) = 2.0F * (xy - wz);
            mat.At(1, 1) = 1.0F - 2.0F * (xx + zz);
            mat.At(1, 2) = 2.0F * (yz + wx);
            mat.At(1, 3) = 0.0F;

            mat.At(2, 0) = 2.0F * (xz + wy);
            mat.At(2, 1) = 2.0F * (yz - wx);
            mat.At(2, 2) = 1.0F - 2.0F * (xx + yy);
            mat.At(2, 3) = 0.0F;

            mat.At(3, 0) = 0.0F;
            mat.At(3, 1) = 0.0F;
            mat.At(3, 2) = 0.0F;
            mat.At(3, 3) = 1.0F;

            return mat;
        }

        Matrix4 &ApplyRotation(const Quaternion &q) {
            *this = *this * Rotate(q);
            return *this;
        }

        static Matrix4 Scale(Vector3<T> v) {
            Matrix4 mat = Identity();
            mat.At(0, 0) = v.x;
            mat.At(1, 1) = v.y;
            mat.At(2, 2) = v.z;

            return mat;
        }

        static Matrix4 Scale(T s) { return Scale(s, s, s); }

        Matrix4 &ApplyScale(Vector3<T> v) {
            *this = *this * Scale(v);
            return *this;
        }

        Matrix4 &ApplyScale(T s) { return ApplyScale(s, s, s); }

        static Matrix4 LookAt(T eyeX, T eyeY, T eyeZ, T centerX, T centerY, T centerZ, T upX, T upY, T upZ) {
            T fx = centerX - eyeX;
            T fy = centerY - eyeY;
            T fz = centerZ - eyeZ;
            T fLen = std::sqrt(fx * fx + fy * fy + fz * fz);
            fx /= fLen;
            fy /= fLen;
            fz /= fLen;

            T rx = fy * upZ - fz * upY;
            T ry = fz * upX - fx * upZ;
            T rz = fx * upY - fy * upX;
            T rLen = std::sqrt(rx * rx + ry * ry + rz * rz);
            rx /= rLen;
            ry /= rLen;
            rz /= rLen;

            T ux = ry * fz - rz * fy;
            T uy = rz * fx - rx * fz;
            T uz = rx * fy - ry * fx;

            Matrix4 mat;
            mat.At(0, 0) = rx;
            mat.At(1, 0) = ry;
            mat.At(2, 0) = rz;
            mat.At(3, 0) = 0.0F;
            mat.At(0, 1) = ux;
            mat.At(1, 1) = uy;
            mat.At(2, 1) = uz;
            mat.At(3, 1) = 0.0F;
            mat.At(0, 2) = -fx;
            mat.At(1, 2) = -fy;
            mat.At(2, 2) = -fz;
            mat.At(3, 2) = 0.0F;
            mat.At(0, 3) = 0.0F;
            mat.At(1, 3) = 0.0F;
            mat.At(2, 3) = 0.0F;
            mat.At(3, 3) = 1.0F;

            return mat * Translate(-eyeX, -eyeY, -eyeZ);
        }
    };

    using Matrix4f = Matrix4<float>;
    using Matrix4d = Matrix4<double>;
} // namespace Pixf::Core::Math

inline Pixf::Core::Math::Matrix4f Pixf::Core::Math::Quaternion::ToMatrix() const {
    return Matrix4f::Rotate(*this);
}

#endif // PIXF_MATRIX_HPP
