#ifndef LEPUS_UTILITY_TYPES_QUATERNION
#define LEPUS_UTILITY_TYPES_QUATERNION

#include "Vector.h"

namespace lepus
{
    namespace types
    {
        class Quaternion : public Vector4
        {
            public:
            Quaternion()
            {
                const float const identity[] = { 0.f, 0.f, 0.f, 1.f };
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

            Quaternion(float axisX, float axisY, float axisZ, float angle)
            {
                float const q[] = { axisX * sinf(angle / 2.f), axisY * sinf(angle / 2.f), axisZ * sinf(angle / 2.f), cos(angle / 2.f) };
                init((float*)q);
            }

            Quaternion(const Vector3& axis, float angle) : Quaternion(axis.x(), axis.y(), axis.z(), angle) {}

            inline Vector3 Axis()
            {
                float sqrtInvW = sqrtf(1.f - w() * w());
                if (sqrtInvW == 0.f)
                {
                    // just return Vector3.Up if divide by 0
                    return Vector3(0.f, 1.f, 0.f);
                }

                Vector3 vec = Vector3(x() / sqrtInvW, y() / sqrtInvW, z() / sqrtInvW);
                return vec * (1.f / vec.Magnitude());
            }

            inline float Angle()
            {
                return acosf(w()) * 2.f;
            }

            inline Quaternion operator*(const Quaternion& b)
            {
                // TODO: use scalar/vector notation for multiplying quaternions
                Quaternion result = Quaternion();

                lepus::types::Vector3 va = lepus::types::Vector3((float*)GetData());
                lepus::types::Vector3 vb = lepus::types::Vector3((float*)b.GetData());
                result.w((w() * b.w()) - lepus::types::Vector3::Dot(va, vb));

                lepus::types::Vector3 vc = lepus::types::Vector3::Cross(vb, va) + (vb * w()) + (va * b.w());
                result.x(vc.x());
                result.y(vc.y());
                result.z(vc.z());

                return result;
            }
        };
    }
}

#endif