#ifndef LUTILITY_MATH_MATRIX4X4
#define LUTILITY_MATH_MATRIX4X4

#include "../Math.h"

namespace lepus
{
    // TODO: this should go into lepus::types technically?
    namespace math
    {
        class Matrix4x4
        {
            private:
            // TODO: use an array instead?
            // Elements in row-major order.
            float m_Elements[4 * 4];

            public:
            Matrix4x4()
            {
                memset(m_Elements, 0, sizeof(float) * 4 * 4);
            }

            Matrix4x4(const Matrix4x4& other)
            {
                memcpy(m_Elements, other.m_Elements, sizeof(float) * 4 * 4);
            }

            Matrix4x4& operator=(const Matrix4x4& other)
            {
                memmove(m_Elements, other.m_Elements, sizeof(float) * 4 * 4);
                return *this;
            }

            static Matrix4x4 Identity()
            {
                Matrix4x4 matrix = Matrix4x4();

                matrix.set<0, 0>(1.f);
                matrix.set<1, 1>(1.f);
                matrix.set<2, 2>(1.f);
                matrix.set<3, 3>(1.f);

                return matrix;
            }

            template<const uint8_t row, const uint8_t col>
            inline void set(float value)
            {
                m_Elements[row * 4 + col] = value;
            }

            inline void set(uint8_t row, uint8_t col, float value)
            {
                m_Elements[row * 4 + col] = value;
            }

            inline float get(uint8_t row, uint8_t col) const
            {
                return m_Elements[row * 4 + col];
            }

            Matrix4x4 Multiply(const Matrix4x4& other) const
            {
                Matrix4x4 ret = Matrix4x4();

                for (uint8_t row = 0; row < 4; row++)
                {
                    for (uint8_t col = 0; col < 4; col++)
                    {
                        ret.set(row, col, get(row, 0) * other.get(0, col) + get(row, 1) * other.get(1, col) + get(row, 2) * other.get(2, col) + get(row, 3) * other.get(3, col));
                    }
                }

                return ret;
            }
        };
    }
}
#endif