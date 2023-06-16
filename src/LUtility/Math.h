#ifndef LUTILITY_MATH
#define LUTILITY_MATH
#include <memory>

namespace lepus
{
    namespace math
    {
        struct Vector4
        {
            float x, y, z = y = x = 0.f, w = 1.f;
        };

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

        class Vector3
        {
            private:
            float m_Components[3];

            public:
            Vector3()
            {
                memset(m_Components, 0, sizeof(float) * 3);
            }

            Vector3(const Vector3& other)
            {
                memcpy(m_Components, other.m_Components, sizeof(float) * 3);
            }

            Vector3& operator=(const Vector3& other)
            {
                memmove(m_Components, other.m_Components, sizeof(float) * 3);
            }

            Vector3(float x, float y, float z)
            {
                m_Components[0] = x;
                m_Components[1] = y;
                m_Components[2] = z;
            }

            Vector3(float* xyz)
            {
                memcpy(m_Components, xyz, sizeof(float) * 3);
            }

            void Multiply(Vector3& b)
            {
                m_Components[0] *= b.m_Components[0];
                m_Components[1] *= b.m_Components[1];
                m_Components[2] *= b.m_Components[2];
            }

            void Multiply(float scalar)
            {
                m_Components[0] *= scalar;
                m_Components[1] *= scalar;
                m_Components[2] *= scalar;
            }

            void Add(Vector3& b)
            {
                m_Components[0] += b.m_Components[0];
                m_Components[1] += b.m_Components[1];
                m_Components[2] += b.m_Components[2];
            }

            void Subtract(Vector3& b)
            {
                m_Components[0] -= b.m_Components[0];
                m_Components[1] -= b.m_Components[1];
                m_Components[2] -= b.m_Components[2];
            }

            Vector4 Multiply(Matrix4x4& matrix)
            {
                float x = m_Components[0] * matrix.get(0, 0) + m_Components[1] * matrix.get(1, 0) + m_Components[2] * matrix.get(2, 0) + matrix.get(3, 0);
                float y = m_Components[0] * matrix.get(0, 1) + m_Components[1] * matrix.get(1, 1) + m_Components[2] * matrix.get(2, 1) + matrix.get(3, 1);
                float z = m_Components[0] * matrix.get(0, 2) + m_Components[1] * matrix.get(1, 2) + m_Components[2] * matrix.get(2, 2) + matrix.get(3, 2);
                float w = m_Components[0] * matrix.get(0, 3) + m_Components[1] * matrix.get(1, 3) + m_Components[2] * matrix.get(2, 3) + matrix.get(3, 3);

                Vector4 vec4 = { x, y, z, w };
                return vec4;
            }
        };
    }
}

#endif