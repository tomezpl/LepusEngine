#ifndef LUTILITY_MATH_TRANSFORM
#define LUTILITY_MATH_TRANSFORM

#include <lepus/utility/Math.h>
#include <lepus/engine/ConsoleLogger.h>

namespace lepus
{
    namespace math
    {
	class Transform
	{
	    private:
	    lepus::types::Vector3 m_Origin, m_Forward, m_Right, m_Up;

	    lepus::types::Quaternion m_Rotation;

	    lepus::types::Vector3 m_Scale;

	    public:
	    Transform()
	    {
		m_Origin = lepus::types::Vector3();

		m_Forward = lepus::types::Vector3(0.f, 0.f, 1.f);
		m_Right = lepus::types::Vector3(1.f, 0.f, 0.f);
		m_Up = lepus::types::Vector3(0.f, 1.f, 0.f);

		m_Rotation = lepus::types::Quaternion();

		m_Scale = lepus::types::Vector3(1.f, 1.f, 1.f);
	    }

	    Transform(Transform&& transform)
	    {
		m_Origin = transform.Origin();
		m_Forward = transform.Forward();
		m_Up = transform.Up();
		m_Right = transform.Right();
		m_Rotation = transform.Rotation();
		m_Scale = transform.Scale();
	    }

	    Transform(const Transform& transform)
	    {
		m_Origin = transform.Origin();
		m_Forward = transform.Forward();
		m_Up = transform.Up();
		m_Right = transform.Right();
		m_Rotation = transform.Rotation();
		m_Scale = transform.Scale();
	    }

	    Transform& operator=(const Transform& transform)
	    {
		m_Origin = transform.Origin();
		m_Forward = transform.Forward();
		m_Up = transform.Up();
		m_Right = transform.Right();
		m_Rotation = transform.Rotation();
		m_Scale = transform.Scale();

		return *this;
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
		m_Forward.x(newForward.x());
		m_Forward.y(newForward.y());
		m_Forward.z(newForward.z());
		m_Right.x(newRight.x());
		m_Right.y(newRight.y());
		m_Right.z(newRight.z());
		m_Up.x(newUp.x());
		m_Up.y(newUp.y());
		m_Up.z(newUp.z());
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

	    inline lepus::types::Vector3 Scale() const { return m_Scale; }

	    inline void SetPosition(float x, float y, float z)
	    {
		m_Origin.x(x);
		m_Origin.y(y);
		m_Origin.z(z);
	    }

	    inline void SetScale(float scale)
	    {
		this->SetScale(scale, scale, scale);
	    }

	    inline void SetScale(float x, float y, float z)
	    {
		m_Scale.x(x);
		m_Scale.y(y);
		m_Scale.z(z);
	    }

	    inline lepus::math::Matrix4x4 BuildMatrix() const
	    {
		lepus::math::Matrix4x4 model = lepus::math::Matrix4x4::Identity();

		// Translation
		model.set<0, 3>(m_Origin.x());
		model.set<1, 3>(m_Origin.y());
		model.set<2, 3>(m_Origin.z());

		// Normalised rotation quaternion for the rotation matrix
		lepus::types::Quaternion normRot = m_Rotation.Normalised();
		lepus::types::Vector3 axis = normRot.Axis();
		float angle = normRot.Angle();

		float cosTheta = cosf(angle);
		float invCosTheta = 1.f - cosTheta;
		float sinTheta = sinf(angle);

		// Build the rotation & scaling components of the matrix.
		model.set<0, 0>(m_Scale.x() * (axis.x() * axis.x() * invCosTheta + cosTheta));
		model.set<0, 1>(m_Scale.y() * (axis.x() * axis.y() * invCosTheta + axis.z() * sinTheta));
		model.set<0, 2>(m_Scale.z() * (axis.x() * axis.z() * invCosTheta - axis.y() * sinTheta));

		model.set<1, 0>(m_Scale.x() * (axis.y() * axis.x() * invCosTheta - axis.z() * sinTheta));
		model.set<1, 1>(m_Scale.y() * (axis.y() * axis.y() * invCosTheta + cosTheta));
		model.set<1, 2>(m_Scale.z() * (axis.y() * axis.z() * invCosTheta + axis.x() * sinTheta));

		model.set<2, 0>(m_Scale.x() * (axis.z() * axis.x() * invCosTheta + axis.y() * sinTheta));
		model.set<2, 1>(m_Scale.y() * (axis.z() * axis.y() * invCosTheta - axis.x() * sinTheta));
		model.set<2, 2>(m_Scale.z() * (axis.z() * axis.z() * invCosTheta + cosTheta));

		return model;
	    }
	};
    } // namespace math
} // namespace lepus

#endif
