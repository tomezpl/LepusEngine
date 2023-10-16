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

                lepus::types::Vector3 axis = m_Transform.Rotation().Axis();

                auto fwdDot = lepus::types::Vector3::Dot(m_Transform.Forward(), lepus::types::Vector3(0.f, 0.f, -1.f));
                auto rgtDot = lepus::types::Vector3::Dot(m_Transform.Right(), lepus::types::Vector3(1.f, 0.f, 0.f));
                auto upDot = lepus::types::Vector3::Dot(m_Transform.Up(), lepus::types::Vector3(0.f, 1.f, 0.f));

                float angle = m_Transform.Rotation().Angle();


                /*axis.x(axis.x() * rgtDot);
                axis.y(axis.y() * upDot);
                axis.z(axis.z() * fwdDot);*/

                // TODO: shouldn't need the non-const float* cast
                lepus::math::Matrix4x4 rot = lepus::math::Transform::AxisAngle(axis, angle);
                lepus::math::Matrix4x4 pos = lepus::math::Matrix4x4::Identity();

                //LepusEngine::ConsoleLogger::Global().LogInfo("Camera", "BuildViewMatrix", (char*)std::to_string(angle).c_str());
                //LepusEngine::ConsoleLogger::Global().LogInfo("Camera", "BuildViewMatrix", (char*)std::to_string(fwdDot).c_str());



                auto f = m_Transform.Forward() * (1.f / m_Transform.Forward().Magnitude());
                auto s = m_Transform.Right() * (1.f / m_Transform.Right().Magnitude());
                auto u = m_Transform.Up() * (1.f / m_Transform.Up().Magnitude());

                //LepusEngine::ConsoleLogger::Global().LogInfo("Camera", "BuildViewMatrix", (char*)s.ToString().c_str());

                auto e = m_Transform.Origin();

                pos.set<0, 3>(-e.x());
                pos.set<1, 3>(-e.y());
                pos.set<2, 3>(-e.z());

                /*pos.set<0, 0>(s.x());pos.set<0, 1>(u.x());pos.set<0, 2>(f.x());
                pos.set<1, 0>(s.y());pos.set<1, 1>(u.y());pos.set<1, 2>(f.y());
                pos.set<2, 0>(s.z());pos.set<2, 1>(u.z());pos.set<2, 2>(f.z());*/

                //return pos;


                lepus::math::Matrix4x4 lookAt = lepus::math::Matrix4x4::Identity();

                lookAt.set<0, 0>(s.x());lookAt.set<0, 1>(s.y());lookAt.set<0, 2>(s.z());
                lookAt.set<1, 0>(u.x());lookAt.set<1, 1>(u.y());lookAt.set<1, 2>(u.z());
                lookAt.set<2, 0>(f.x());lookAt.set<2, 1>(f.y());lookAt.set<2, 2>(f.z());

                return lookAt.Multiply(pos);
                return rot.Multiply(pos);
            }
        };
    }
}

#endif
