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

            lepus::math::Matrix4x4 BuildMatrix()
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
        };
    }
}

#endif
