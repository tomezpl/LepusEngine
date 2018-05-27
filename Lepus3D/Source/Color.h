#ifndef _LEPUS3D_COLOR_
#define _LEPUS3D_COLOR_

#include "Transform/Vector.h"

namespace LepusEngine {
	namespace Lepus3D {
		class Color {
		private:
			Vector4 mVec; // color vector
		public:
			Color();
			Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
			Vector4 GetVector4();
			Vector3 GetVector3();
			void SetR(unsigned char R);
			void SetG(unsigned char G);
			void SetB(unsigned char B);
			void SetA(unsigned char A);
		};
	}
}

#endif
