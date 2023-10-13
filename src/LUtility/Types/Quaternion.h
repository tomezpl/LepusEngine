#ifndef LEPUS_UTILITY_TYPES_QUATERNION
#define LEPUS_UTILITY_TYPES_QUATERNION

#include "Vector.h"

namespace lepus
{
    namespace types
    {
        class Quaternion : public Vector4
        {
            public:
            Quaternion()
            {
                const float const identity[] = { 0.f, 0.f, 0.f, 1.f };
                init((float*)identity);
            }

            Quaternion(const Quaternion& quat)
            {
                init(quat);
            }

            Quaternion(float* quatData)
            {
                init(quatData);
            }

            inline Quaternion operator*(const Quaternion& b)
            {
                // TODO: use scalar/vector notation for multiplying quaternions
                Quaternion result = Quaternion();
                result.x(x() * b.x());
                result.y(y() * b.y());
                result.z(z() * b.z());
                result.w(w() * b.w());
            }
        };
    }
}

#endif