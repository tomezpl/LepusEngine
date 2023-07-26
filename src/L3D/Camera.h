#ifndef LEPUS_GFX_CAMERA
#define LEPUS_GFX_CAMERA

#define LEPUS_GFX_CAMERA_DEFAULT_FOV 60.f
#define LEPUS_GFX_CAMERA_DEFAULT_CLIP_NEAR 0.003f
#define LEPUS_GFX_CAMERA_DEFAULT_CLIP_FAR 1000.f

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

            lepus::math::Matrix4x4 BuildMatrix()
            {
                lepus::math::Matrix4x4 projMatrix = lepus::math::Matrix4x4();

                float hypot = tanf((m_FOV / 2.f) * DEG2RAD);

                projMatrix.set<0, 0>(hypot);
                projMatrix.set<1, 1>(hypot);

                projMatrix.set<2, 2>(-(m_Far / (m_Far - m_Near)));
                projMatrix.set<3, 2>(-((m_Far * m_Near) / (m_Far - m_Near)));

                projMatrix.set<2, 3>(-1.f);

                return projMatrix;
            }
        };
    }
}

#endif
