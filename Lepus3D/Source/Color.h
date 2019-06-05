#ifndef _LEPUS3D_COLOR_
#define _LEPUS3D_COLOR_

#include "Transform/Vector.h"

namespace LepusEngine {
	namespace Lepus3D {
		// Color class
		// Represents an RGBA colour stored in a 4D Vector ({x, y, z, w} used for {r, g, b, a} respectively)
		class Color {
		private:
			Vector4 mVec; // color vector
		public:
			// Default constructor. Initialises all values with zero.
			Color();

			// Constructor. Initialises 8-bit RGBA colour values with the provided arguments.
			Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A);

			// Retrieves 4D (RGBA) vector representing the colour.
			Vector4 GetVector4();

			// Retrieves 3D (RGB) vector representing the colour. As it lacks the alpha channel, transparency values are not provided.
			Vector3 GetVector3();

			// Sets the Red channel using an 8-bit value in range 0-255.
			void SetR(unsigned char R);

			// Sets the Red channel using a floating-point value in normalised range 0.0-1.0
			void SetR(float R);

			// Sets the Green channel using an 8-bit value in range 0-255.
			void SetG(unsigned char G);

			// Sets the Green channel using a floating-point value in normalised range 0.0-1.0
			void SetG(float G);

			// Sets the Blue channel using an 8-bit value in range 0-255.
			void SetB(unsigned char B);

			// Sets the Blue channel using a floating-point value in normalised range 0.0-1.0
			void SetB(float B);

			// Sets the Alpha channel using an 8-bit value in range 0-255.
			void SetA(unsigned char A);

			// Sets the Alpha channel using a floating-point value in normalised range 0.0-1.0
			void SetA(float A);
		};
	}
}

#endif
