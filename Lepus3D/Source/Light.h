#ifndef _LEPUS3D_LIGHT_
#define _LEPUS3D_LIGHT_

#include "Transform.h"
#include "Color.h"

namespace LepusEngine {
	namespace Lepus3D {
		// Light class
		// Represents a light in 3D space that can be used in shading.
		class Light : public Transformable {
		private:
			Color mCol;
			float mIntensity;
		public:
			// Default constructor. Creates a new white-colour light of intensity 1.0
			Light();

			// Constructor. Creates a new white of specified colour, intensity and 3D position.
			Light(Vector3 position, Color color, float intensity);

			// Retrieves Light position from the Transform object.
			Vector3 GetPosition();

			// Retrieves the Color of the Light.
			Color GetColor();

			// Retrieves the intensity value of the Light.
			float GetIntensity();

			// Sets the Color of the Light.
			void SetColor(Color color);

			// Sets the intensity value of the Light.
			void SetIntensity(float intensity);
		};
	}
}

#endif
