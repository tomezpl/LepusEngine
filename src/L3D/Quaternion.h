#pragma once

#include "Transform/Vector.h"

namespace LepusEngine {
	namespace Lepus3D {
		class Quaternion {
		public:
			float x, y, z, w;
			Quaternion(float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 1.f) { x = X; y = Y; z = Z; w = W; }
			// Converts Euler angles (in radians) to Quaternion
			Quaternion(Vector3 euler);
			Quaternion(Vector3 axis, float angle);
			// Converts Quaternion to Euler angles (in radians)
			Vector3 ToEuler();
			std::string ToString();
			static Quaternion Mult(Quaternion q1, Quaternion q2);
		};
	}
}