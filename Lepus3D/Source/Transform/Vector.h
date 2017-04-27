#pragma once

#include <glm/glm.hpp>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Vector2 {
		public:
			float x, y;
			Vector2(float X = 0.f, float Y = 0.f) { x = X; y = Y; };
			glm::vec2 vec2() { return glm::vec2(x, y); };
			Vector2& operator=(Vector2& rs) { Vector2 ret(rs.x, rs.y); return ret; };
			Vector2 Zero() { return Vector2(); };
		};
		
		class Vector3 : public Vector2 {
		public:
			float z;
			Vector3(float X = 0.f, float Y = 0.f, float Z = 0.f) : Vector2(X, Y) { z = Z; };
			glm::vec3 vec3() { return glm::vec3(x, y, z); };
			Vector3& operator=(Vector3& rs) { Vector3 ret(rs.x, rs.y, rs.z); return ret; };
			Vector3 Zero() { return Vector3(); };
		};

		class Vector4 : public Vector3 {
		public:
			float w;
			Vector4(float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f) : Vector3(X, Y, Z) { w = W; };
			glm::vec4 vec4() { return glm::vec4(x, y, z, w); };
			Vector4& operator=(Vector4& rs) { Vector4 ret(rs.x, rs.y, rs.z, rs.w); return ret; };
			Vector4 Zero() { return Vector4(); };
		};
	}
}
