#pragma once

#include <glm/glm.hpp>
#include <string>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Vector2 {
		public:
			float x, y;
			Vector2(float X = 0.f, float Y = 0.f) { x = X; y = Y; };
			Vector2(glm::vec2 v) { x = v.x; y = v.y; };
			glm::vec2 vec2() { return glm::vec2(x, y); };
			Vector2& operator=(Vector2& rs) { return rs; };
			static Vector2 Create(float X = 0.f, float Y = 0.f) { Vector2 ret(X, Y); return ret; }
			void Set(float X = 0.f, float Y = 0.f) { x = X; y = Y; };
			static Vector2 Zero() { return Vector2(); };
			std::string ToString() { std::string ret = "("; 
				ret += std::to_string(x);
				ret += ", ";
				ret += std::to_string(y);
				ret += ")";
				return ret;
			};
		};
		
		class Vector3 : public Vector2 {
		public:
			float z;
			Vector3(float X = 0.f, float Y = 0.f, float Z = 0.f) : Vector2(X, Y) { z = Z; };
			Vector3(glm::vec3 v) { x = v.x; y = v.y; z = v.z; };
			glm::vec3 vec3() { return glm::vec3(x, y, z); };
			Vector3& operator=(Vector3& rs) { return rs; };
			static Vector3 Create(float X = 0.f, float Y = 0.f, float Z = 0.f) { Vector3 ret(X, Y, Z); return ret; }
			void Set(float X = 0.f, float Y = 0.f, float Z = 0.f) { x = X; y = Y; z = Z; };
			static Vector3 Zero() { return Vector3(); };
			std::string ToString() {
				std::string ret = "(";
				// Convert floats to strings and reduce number of zeroes
				auto trimmed = [](float num) { auto ret = std::to_string(num); ret = ret.substr(0, ret.length() - 4); return ret; };
				ret += trimmed(x);
				ret += ", ";
				ret += trimmed(y);
				ret += ", ";
				ret += trimmed(z);
				ret += ")";
				return ret;
			};
		};

		class Vector4 : public Vector3 {
		public:
			float w;
			Vector4(float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f) : Vector3(X, Y, Z) { w = W; };
			Vector4(glm::vec4 v) { x = v.x; y = v.y; z = v.z; w = v.w; };
			glm::vec4 vec4() { return glm::vec4(x, y, z, w); };
			Vector4& operator=(Vector4& rs) { return rs; };
			static Vector4 Create(float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f) { Vector4 ret(X, Y, Z, W); return ret; }
			void Set(float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f) { x = X; y = Y; z = Z; w = W; };
			static Vector4 Zero() { return Vector4(); };
			std::string ToString() {
				std::string ret = "(";
				ret += std::to_string(x);
				ret += ", ";
				ret += std::to_string(y);
				ret += ", ";
				ret += std::to_string(z);
				ret += ", ";
				ret += std::to_string(w);
				ret += ")";
				return ret;
			};
		};
	}
}
