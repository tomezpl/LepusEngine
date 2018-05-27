#ifndef _LEPUS3D_LIGHT_
#define _LEPUS3D_LIGHT_

#include "Transform.h"
#include "Color.h"

namespace LepusEngine {
	namespace Lepus3D {
		class Light : Transformable {
		private:
			Color mCol;
			float mIntensity;
		public:
			Light();
			Light(Vector3 position, Color color, float intensity);
			Vector3 GetPosition();
			Color GetColor();
			float GetIntensity();
			void SetPosition(Vector3 position);
			void SetColor(Color color);
			void SetIntensity(float intensity);
		};
	}
}

#endif
