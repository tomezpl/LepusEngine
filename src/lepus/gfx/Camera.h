#ifndef LEPUS_GFX_CAMERA
#define LEPUS_GFX_CAMERA

#define LEPUS_GFX_CAMERA_DEFAULT_FOV 110.f
#define LEPUS_GFX_CAMERA_DEFAULT_CLIP_NEAR 0.1f
#define LEPUS_GFX_CAMERA_DEFAULT_CLIP_FAR 100.f

#include <lepus/utility/Math.h>

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

            /// @brief Gets field of view
            /// @return Camera's field of view (in degrees)
            inline float FOV() const
            {
                return m_FOV;
            }

            /// @brief Sets field of view
            /// @param fov New field of view (in degrees) to be used by this camera
            inline void FOV(float fov)
            {
                m_FOV = fov;
            }

            /// @brief Gets the Camera's Transform.
            /// @return A reference to the Transform used by the Camera.
            inline lepus::math::Transform& Transform() { return m_Transform; }

            /// @brief Generates a perspective projection matrix using the current FOV angle and near/far clipping planes.
            /// @return A 4x4 projection matrix that can be used to apply camera perspective to vertices.
            lepus::math::Matrix4x4 BuildPerspectiveMatrix() const
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

            /// @brief Generates a look-at view matrix using the Camera's Transform.
            /// @return A 4x4 view matrix that can be used to rotate and translate vertices according to eye position and camera orientation.
            lepus::math::Matrix4x4 BuildViewMatrix() const
            {
                lepus::math::Matrix4x4 pos = lepus::math::Matrix4x4::Identity();

                auto f = m_Transform.Forward() * (-1.f / m_Transform.Forward().Magnitude());
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
