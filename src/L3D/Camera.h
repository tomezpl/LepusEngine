#ifndef LEPUS_GFX_CAMERA
#define LEPUS_GFX_CAMERA

#define LEPUS_GFX_CAMERA_DEFAULT_FOV 110.f
#define LEPUS_GFX_CAMERA_DEFAULT_CLIP_NEAR 0.1f
#define LEPUS_GFX_CAMERA_DEFAULT_CLIP_FAR 100.f

#include "../LUtility/Math.h"

namespace lepus
{
    namespace gfx
    {
        class Camera
        {
            protected:
            lepus::math::Transform m_Transform;
            float m_FOV;
            float m_Near, m_Far;

            public:
            Camera()
            {
                m_FOV = LEPUS_GFX_CAMERA_DEFAULT_FOV;
                m_Near = LEPUS_GFX_CAMERA_DEFAULT_CLIP_NEAR;
                m_Far = LEPUS_GFX_CAMERA_DEFAULT_CLIP_FAR;
                m_Transform = lepus::math::Transform();
            }

            inline float FOV()
            {
                return m_FOV;
            }

            inline void FOV(float fov)
            {
                m_FOV = fov;
            }

            inline lepus::math::Transform& Transform() { return m_Transform; }

            lepus::math::Matrix4x4 BuildPerspectiveMatrix()
            {
                lepus::math::Matrix4x4 projMatrix = lepus::math::Matrix4x4();

                float hypot = tanf((180.f - m_FOV) * DEG2RAD * 0.5f);

                projMatrix.set<0, 0>(hypot);
                projMatrix.set<1, 1>(hypot);

                projMatrix.set<2, 2>(-(m_Far / (m_Far - m_Near)));
                projMatrix.set<2, 3>(-((m_Far * m_Near) / (m_Far - m_Near)));

                projMatrix.set<3, 2>(-1.f);

                return projMatrix;
            }

            lepus::math::Matrix4x4 BuildViewMatrix()
            {
                lepus::math::Matrix4x4 pos = lepus::math::Matrix4x4::Identity();

                auto f = m_Transform.Forward() * (1.f / m_Transform.Forward().Magnitude());
                auto s = m_Transform.Right() * (1.f / m_Transform.Right().Magnitude());
                auto u = m_Transform.Up() * (1.f / m_Transform.Up().Magnitude());

                auto e = m_Transform.Origin();

                pos.set<0, 3>(-e.x());
                pos.set<1, 3>(-e.y());
                pos.set<2, 3>(-e.z());

                lepus::math::Matrix4x4 lookAt = lepus::math::Matrix4x4::Identity();

                lookAt.set<0, 0>(s.x());lookAt.set<0, 1>(s.y());lookAt.set<0, 2>(s.z());
                lookAt.set<1, 0>(u.x());lookAt.set<1, 1>(u.y());lookAt.set<1, 2>(u.z());
                lookAt.set<2, 0>(f.x());lookAt.set<2, 1>(f.y());lookAt.set<2, 2>(f.z());

                return lookAt.Multiply(pos);
            }
        };
    }
}

#endif
