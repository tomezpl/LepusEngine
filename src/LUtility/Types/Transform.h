#ifndef LUTILITY_MATH_TRANSFORM
#define LUTILITY_MATH_TRANSFORM

#include "../Math.h"
#include "../../LEngine/ConsoleLogger.h"

namespace lepus
{
    namespace math
    {
        class Transform
        {
            private:
            lepus::types::Vector3 m_Origin, m_Forward, m_Right, m_Up;
            // axis-angle
            lepus::types::Quaternion m_Rotation;

            public:
            Transform()
            {
                m_Origin = lepus::types::Vector3();

                m_Forward = lepus::types::Vector3(0.f, 0.f, 1.f);
                m_Right = lepus::types::Vector3(1.f, 0.f, 0.f);
                m_Up = lepus::types::Vector3(0.f, 1.f, 0.f);

                m_Rotation = lepus::types::Quaternion();
            }

            inline lepus::types::Vector3 Origin() const { return m_Origin; }
            inline void Origin(const lepus::types::Vector3& vec)
            {
                m_Origin.x(vec.x());
                m_Origin.y(vec.y());
                m_Origin.z(vec.z());
            }

            inline lepus::types::Vector3 Forward() const { return lepus::types::Vector3(m_Forward); }
            inline lepus::types::Vector3 Right() const { return lepus::types::Vector3(m_Right); }
            inline lepus::types::Vector3 Up() const { return lepus::types::Vector3(m_Up); }

            inline lepus::types::Quaternion Rotation() const { return m_Rotation; }
            inline void Rotation(const lepus::types::Quaternion& newRotation)
            {
                m_Rotation.x(newRotation.x());
                m_Rotation.y(newRotation.y());
                m_Rotation.z(newRotation.z());
                m_Rotation.w(newRotation.w());
            }

            inline void Rotate(const lepus::types::Quaternion& quat)
            {
                m_Rotation = m_Rotation * quat;

                // Rotate the basis vectors
                auto newForward = quat.Rotate(m_Forward);
                auto newRight = quat.Rotate(m_Right);
                auto newUp = quat.Rotate(m_Up);

                // Set basis vectors to rotated
                m_Forward.x(newForward.x());m_Forward.y(newForward.y());m_Forward.z(newForward.z());
                m_Right.x(newRight.x());m_Right.y(newRight.y());m_Right.z(newRight.z());
                m_Up.x(newUp.x());m_Up.y(newUp.y());m_Up.z(newUp.z());
            }

            static inline lepus::math::Matrix4x4 AxisAngle(lepus::types::Vector3& axis, float angle)
            {
                lepus::math::Matrix4x4 ret = lepus::math::Matrix4x4::Identity();

                lepus::types::Vector3 unitAxis = axis * (1.f / axis.Magnitude());

                float c = cosf(angle);
                float s = sinf(angle);

                ret.set<0, 0>(c + (unitAxis.x() * unitAxis.x() * (1.f - c)));
                ret.set<1, 0>((unitAxis.y() * unitAxis.x() * (1.f - c)) + (unitAxis.z() * s));
                ret.set<2, 0>((unitAxis.z() * unitAxis.x() * (1.f - c)) - (unitAxis.y() * s));

                ret.set<0, 1>((unitAxis.y() * unitAxis.x() * (1.f - c)) - (unitAxis.z() * s));
                ret.set<1, 1>(c + (unitAxis.y() * unitAxis.y() * (1.f - c)));
                ret.set<2, 1>((unitAxis.z() * unitAxis.y() * (1.f - c)) + (unitAxis.x() * s));

                ret.set<0, 2>((unitAxis.z() * unitAxis.x() * (1.f - c)) + (unitAxis.y() * s));
                ret.set<1, 2>((unitAxis.z() * unitAxis.y() * (1.f - c)) - (unitAxis.x() * s));
                ret.set<2, 2>(c + (unitAxis.z() * unitAxis.z() * (1.f - c)));

                return ret;
            }
        };
    }
}

#endif
