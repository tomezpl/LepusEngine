#ifndef LEPUS_UTILITY_TYPES_QUATERNION
#define LEPUS_UTILITY_TYPES_QUATERNION

#include "Vector.h"
#include "../../LEngine/ConsoleLogger.h"
#include <cmath>
#include "../Math.h"

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
                float invW = fmax(0.f, 1.f - w() * w());
                float sqrtInvW = sqrt(invW);
                std::string wStr = "invW = " + std::to_string(invW) + ", sqrtInvW = " + std::to_string(sqrtInvW);
                LepusEngine::ConsoleLogger::Global().LogInfo("Quaternion", "Axis", (char*)wStr.c_str());
                if (sqrtInvW == 0.f)
                {
                    // just return Vector3.Up if divide by 0
                    LepusEngine::ConsoleLogger::Global().LogInfo("Quaternion", "Axis", "Up");
                    return Vector3(0.f, 1.f, 0.f);
                }

                Vector3 vec = Vector3(x() / sqrtInvW, y() / sqrtInvW, z() / sqrtInvW);
                return vec * (1.f / vec.Magnitude());
            }

            inline float Angle() const
            {
                return acosf(fmax(-1.f, fmin(1.f, w()))) * 2.f;
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

            inline lepus::types::Vector3 Rotate(const lepus::types::Vector3& v)
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
        };
    }
}

#endif