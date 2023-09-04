#ifndef LUTILITY_MATH_TRANSFORM
#define LUTILITY_MATH_TRANSFORM

#include "../Math.h"

namespace lepus
{
    namespace math
    {
        class Transform
        {
            private:
            lepus::types::Vector3 m_Origin, m_Forward, m_Right, m_Up;
            // axis-angle
            lepus::types::Vector4 m_Rotation;

            public:
            Transform()
            {
                m_Origin = lepus::types::Vector3();

                m_Forward = lepus::types::Vector3(0.f, 0.f, 1.f);
                m_Right = lepus::types::Vector3(1.f, 0.f, 0.f);
                m_Up = lepus::types::Vector3(0.f, 1.f, 0.f);

                m_Rotation = lepus::types::Vector4(0.f, 0.f, 0.f, 1.f);
            }

            inline lepus::types::Vector3& Origin() { return m_Origin; }
            inline lepus::types::Vector3 const& Forward() { return m_Forward; }
            inline lepus::types::Vector3 const& Right() { return m_Right; }
            inline lepus::types::Vector3 const& Up() { return m_Up; }
            inline lepus::types::Vector4& Rotation() { return m_Rotation; }
        };
    }
}

#endif
