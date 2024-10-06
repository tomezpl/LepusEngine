#ifndef LEPUS_UTILITY_TYPES_QUATERNION
#define LEPUS_UTILITY_TYPES_QUATERNION

#include "Vector.h"
#include <lepus/engine/ConsoleLogger.h>
#include <cmath>
#include "../Math.h"

namespace lepus
{
    namespace types
    {
        class Quaternion : public Vector<4>
        {
            public:
            Quaternion()
            {
                const float identity[] = { 0.f, 0.f, 0.f, 1.f };
                init((float*)identity);
            }

            Quaternion(const Quaternion& quat)
            {
                init(quat);
            }

            Quaternion(float* quatData)
            {
                init(quatData);
            }

            /// @brief Constructs a Quaternion from Axis-Angle representation.
            /// @remarks This is not for initialising XYZW directly - use the array initialiser instead.
            /// @param axisX X component of the axis
            /// @param axisY Y component of the axis
            /// @param axisZ Z component of the axis
            /// @param angle Rotation angle around the axis
            Quaternion(float axisX, float axisY, float axisZ, float angle)
            {
                // Negating the angle here so that the Quaternion represents a clockwise rotation along an axis as observed looking towards the origin/object.
                angle *= -1.f;
                float const q[] = { axisX * sinf(angle / 2.f), axisY * sinf(angle / 2.f), axisZ * sinf(angle / 2.f), cos(angle / 2.f) };
                init((float*)q);
            }

            Quaternion(const Vector3& axis, float angle) : Quaternion(axis.x(), axis.y(), axis.z(), angle) {}

            /// @brief Get the axis of rotation for this Quaternion.
            /// @return A Vector3 representing the axis of rotation calculated from the Quaternion's imaginary part.
            inline Vector3 Axis() const
            {
                float sqrtInvW = sqrt(fmax(0.f, 1.f - w() * w()));
                if (sqrtInvW == 0.f)
                {
                    // just return Vector3.Up if divide by 0
                    return Vector3(0.f, 1.f, 0.f);
                }

                Vector3 vec = Vector3(x() / sqrtInvW, y() / sqrtInvW, z() / sqrtInvW);
                return vec * (1.f / vec.Magnitude());
            }

            /// @brief Get the angle of rotation for this Quaternion.
            /// @return A scalar describing the angle of rotation calculated from the Quaternion's real part (arccos(w) * 2).
            inline float Angle() const
            {
                float wrappedW = w();
                // if (wrappedW > 1.f)
                // {
                //     wrappedW = -1.f + (wrappedW - 1.f);
                // }
                // else if (wrappedW < -1.f)
                // {
                //     wrappedW = 1.f + (wrappedW + 1.f);
                // }
                return acosf(fmin(1.f, fmax(-1.f, wrappedW))) * 2.f;
            }

            inline float x() const { return m_Components[0]; }
            inline float y() const { return m_Components[1]; }
            inline float z() const { return m_Components[2]; }
            inline float w() const
            {
                float w = m_Components[3];
                return w;
            }
            inline float x(float newX) { return m_Components[0] = newX; }
            inline float y(float newY) { return m_Components[1] = newY; }
            inline float z(float newZ) { return m_Components[2] = newZ; }
            inline float w(float newW)
            {
                // Wrap the angle component as going outside of [-1, 1] would cause the Quaternion to rotate in the opposite direction
                // if (newW > 1.f)
                // {
                //     newW = -1.f + (newW - 1.f);
                // }
                // else if (newW < -1.f)
                // {
                //     newW = 1.f + (newW + 1.f);
                // }

                return m_Components[3] = newW;
            }

            inline Quaternion operator*(const Quaternion& b) const
            {
                Quaternion result = Quaternion();

                // lepus::engine::ConsoleLogger::Global().LogInfo("Quaternion", "operator*", std::to_string(w()).c_str(), "const Quaternion& b");

                // result.w(fmin(1.f, fmax(-1.f, w())));

                lepus::types::Vector3 va = lepus::types::Vector3((float*)GetData());
                lepus::types::Vector3 vb = lepus::types::Vector3((float*)b.GetData());
                result.w((w() * b.w()) - lepus::types::Vector3::Dot(va, vb));

                lepus::types::Vector3 vc = lepus::types::Vector3::Cross(va, vb) + (vb * w()) + (va * b.w());
                result.x(vc.x());
                result.y(vc.y());
                result.z(vc.z());

                // lepus::engine::ConsoleLogger::Global().LogInfo("Quaternion", "operator*", result.Axis().ToString().c_str(), "const Quaternion& b");

                return result;
            }

            /// @brief Rotates a Vector3 v by the axis and angle described by this Quaternion.
            /// @param v Vector3 representing a point in 3D space to rotate.
            /// @return A Vector3 containing the point v rotated around the origin.
            inline lepus::types::Vector3 Rotate(const lepus::types::Vector3& v) const
            {
                Quaternion p = Quaternion();
                p.w(0.f);
                p.x(v.x());
                p.y(v.y());
                p.z(v.z());
                Quaternion conjugate = Quaternion(*this);
                conjugate.x(-conjugate.x());
                conjugate.y(-conjugate.y());
                conjugate.z(-conjugate.z());
                return lepus::types::Vector3((float*)(*this * p * conjugate).GetData());
            }

            /// @brief Create a text representation of the Quaternion. Useful for debugging.
            /// @return A string representing the Quaternion formatted as: "X = x, Y = y, Z = z, W = w" 
            std::string ToString() const
            {
                return std::string("X = ").append(std::to_string(x())).append(", Y = ").append(std::to_string(y())).append(", Z = ").append(std::to_string(z())).append(", W = ").append(std::to_string(w()));
            }
        };
    }
}

#endif