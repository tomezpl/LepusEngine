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

                /*
    rot[0] = vec4(c+axis.x*axis.x*(1.0 - c), axis.y*axis.x*(1.0 - c)+axis.z*s, axis.z*axis.x*(1.0 - c)-axis.y*s, 0.0);
    rot[1] = vec4(axis.y*axis.x*(1.0 - c)-axis.z*s, c+axis.y*axis.y*(1.0 - c), axis.z*axis.y*(1.0 - c)+axis.x*s, 0.0);
    rot[2] = vec4(axis.z*axis.x*(1.0 - c)+axis.y*s, axis.z*axis.y*(1.0 - c)-axis.x*s, c+axis.z*axis.z*(1.0 - c), 0.0);
    rot[3] = vec4(0.0, 0.0, 0.0, 1.0);
                */

                lepus::types::Vector4& axisAngle = m_Transform.Rotation();

                // TODO: shouldn't need the non-const float* cast
                lepus::math::Matrix4x4 rot = lepus::math::Transform::AxisAngle(lepus::types::Vector3((float*)axisAngle.GetData()), axisAngle.w());
                lepus::math::Matrix4x4 pos = lepus::math::Matrix4x4::Identity();

                pos.set<0, 3>(-m_Transform.Origin().x());
                pos.set<1, 3>(-m_Transform.Origin().y());
                pos.set<2, 3>(-m_Transform.Origin().z());

                return rot.Multiply(pos);
            }
        };
    }
}

#endif
