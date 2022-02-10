#include <L3D/Quaternion.h>
#include <L3D\Transform.h>
#include <string>

using namespace LepusEngine;
using namespace Lepus3D;

Quaternion::Quaternion(Vector3 euler)
{
	double cosBank = cos(euler.z / 2.f);
	double sinBank = sin(euler.z / 2.f);

	double cosAttitude = cos(euler.x / 2.f);
	double sinAttitude = sin(euler.x / 2.f);

	double cosHeading = cos(euler.y / 2.f);
	double sinHeading = sin(euler.y / 2.f);

	Quaternion quat = Quaternion(
		/* X */ cosBank * sinAttitude * cosHeading + sinBank * cosAttitude * sinHeading,
		/* Y */ cosBank * cosAttitude * sinHeading - sinBank * sinAttitude * cosHeading,
		/* Z */ sinBank * cosAttitude * cosHeading - cosBank * sinAttitude * sinHeading,
		/* W */ cosBank * cosAttitude * cosHeading + sinBank * sinAttitude * sinHeading
	);

	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;
}

Quaternion::Quaternion(Vector3 axis, float angle)
{
	glm::quat quat = glm::angleAxis(angle, axis.vec3());
	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;
}

Vector3 Quaternion::ToEuler()
{
	// Roll
	double bank = atan2(2. * ((double)w * (double)z + (double)x * (double)y), 1. - 2. * ((double)z * (double)z + (double)x + (double)x));
	// Pitch
	double pSin = 2. * ((double)w * (double)x - (double)y * (double)z);
	double attitude = copysign((double)PI / 2., pSin);
	//attitude = asin(pSin);
	if (abs(pSin) < 1.)
	{
		attitude = asin(pSin);
	}
	// Yaw
	double heading = atan2(2. * ((double)w * (double)y + (double)z * (double)x), 1. - 2. * ((double)x * (double)x + (double)y + (double)y));

	return Vector3(attitude, heading, bank);
}

std::string Quaternion::ToString()
{
	return "(X: " + std::to_string(x) + ", Y: " + std::to_string(y) + ", Z: " + std::to_string(z) + ", W: " + std::to_string(w) + ")";
}

Quaternion Quaternion::Mult(Quaternion q1, Quaternion q2)
{
	return Quaternion(
	/* X */	q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
	/* Y */	q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
	/* Z */	q1.z * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
	/* W */	q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
	);
}
