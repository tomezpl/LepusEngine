#ifndef LUTILITY_TYPES_VECTOR
#define LUTILITY_TYPES_VECTOR

#include <cstdint>
#include <cstring>
#include <memory>

namespace lepus
{
    namespace types
    {
        template<const uint8_t nbComponents>
        class Vector
        {
            protected:
            float m_Components[nbComponents];

            void init()
            {
                memset(m_Components, 0, sizeof(float) * nbComponents);
            }

            void init(float* data)
            {
                memcpy(m_Components, data, sizeof(float) * nbComponents);
            }

            void init(const Vector<nbComponents>& other)
            {
                memcpy(m_Components, other.m_Components, sizeof(float) * nbComponents);
            }

            public:

            Vector<nbComponents>& operator=(const Vector<nbComponents>& other)
            {
                memmove(m_Components, other.m_Components, sizeof(float) * nbComponents);
                return *this;
            }

            void Multiply(const Vector<nbComponents>& b)
            {
                for (uint8_t i = 0; i < nbComponents; i++) { m_Components[i] *= b.m_Components[i]; }
            }

            void Multiply(float scalar)
            {
                for (uint8_t i = 0; i < nbComponents; i++) { m_Components[i] *= scalar; }
            }

            void Add(const Vector<nbComponents>& b)
            {
                for (uint8_t i = 0; i < nbComponents; i++) { m_Components[i] += b.m_Components[i]; }
            }

            void Subtract(const Vector<nbComponents>& b)
            {
                for (uint8_t i = 0; i < nbComponents; i++) { m_Components[i] -= b.m_Components[i]; }
            }

            Vector<nbComponents> operator-(const Vector<nbComponents>& b)
            {
                Vector<nbComponents> out = Vector<nbComponents>(*this);
                out.Subtract(b);
                return out;
            }

            Vector<nbComponents> operator+(const Vector<nbComponents>& b)
            {
                Vector<nbComponents> out = Vector<nbComponents>(*this);
                out.Add(b);
                return out;
            }

            Vector<nbComponents> operator*(const Vector<nbComponents>& b)
            {
                Vector<nbComponents> out = Vector<nbComponents>(*this);
                out.Multiply(b);
                return out;
            }

            Vector<nbComponents> operator*(float b)
            {
                Vector<nbComponents> out = Vector<nbComponents>(*this);
                out.Multiply(b);
                return out;
            }

            bool operator==(const Vector<nbComponents>& b) const
            {
                for (uint8_t i = 0; i < nbComponents; i++)
                {
                    if (m_Components[i] != b.m_Components[i])
                    {
                        return false;
                    }
                }

                return true;
            }

            float SqrMagnitude() const
            {
                float sum = 0.f;

                for (uint8_t i = 0; i < nbComponents; i++)
                {
                    sum += m_Components[i] * m_Components[i];
                }

                return sum;
            }

            float Magnitude() const
            {
                return sqrtf(SqrMagnitude());
            }

            const float* const GetData() const
            {
                return m_Components;
            }
        };

        class Vector2 : public Vector<2>
        {
            public:
            Vector2()
            {
                init();
            }

            Vector2(float x, float y)
            {
                m_Components[0] = x;
                m_Components[1] = y;
            }

            Vector2(float* data)
            {
                init(data);
            }

            Vector2(const Vector2& other)
            {
                init(other);
            }

            inline float x() const { return m_Components[0]; }
            inline float y() const { return m_Components[1]; }
            inline float x(float newX) { m_Components[0] = newX; }
            inline float y(float newY) { m_Components[1] = newY; }

            inline Vector2 operator-(const Vector2& b) { Vector<2>::Subtract(b); return *this; }
            inline Vector2 operator+(const Vector2& b) { Vector<2>::Add(b); return *this; }
            inline Vector2 operator*(const Vector2& b) { Vector<2>::Multiply(b); return *this; }
            inline Vector2 operator*(float scalar) { Vector<2>::Multiply(scalar); return *this; }
        };

        class Vector3 : public Vector<3>
        {
            public:
            Vector3()
            {
                init();
            }

            Vector3(float x, float y, float z)
            {
                m_Components[0] = x;
                m_Components[1] = y;
                m_Components[2] = z;
            }

            Vector3(float* data)
            {
                init(data);
            }

            Vector3(const Vector3& other)
            {
                init(other);
            }

            inline float x() const { return m_Components[0]; }
            inline float y() const { return m_Components[1]; }
            inline float z() const { return m_Components[2]; }
            inline float x(float newX) { m_Components[0] = newX; }
            inline float y(float newY) { m_Components[1] = newY; }
            inline float z(float newZ) { m_Components[2] = newZ; }

            inline Vector3 operator-(const Vector3& b) { Vector<3>::Subtract(b); return *this; }
            inline Vector3 operator+(const Vector3& b) { Vector<3>::Add(b); return *this; }
            inline Vector3 operator*(const Vector3& b) { Vector<3>::Multiply(b); return *this; }
            inline Vector3 operator*(float scalar) { Vector<3>::Multiply(scalar); return *this; }
        };

        class Vector4 : public Vector<4>
        {
            public:
            Vector4()
            {
                init();
            }

            Vector4(float x, float y, float z, float w)
            {
                m_Components[0] = x;
                m_Components[1] = y;
                m_Components[2] = z;
                m_Components[3] = w;
            }

            Vector4(float* data)
            {
                init(data);
            }

            Vector4(const Vector4& other)
            {
                init(other);
            }

            Vector4(const Vector3& vec3)
            {
                memcpy(m_Components, vec3.GetData(), sizeof(float) * 3);

                // Set W to 1.0, as we're assuming an implicit conversion to homogenous coordinates.
                m_Components[3] = 1.f;
            }

            inline float x() const { return m_Components[0]; }
            inline float y() const { return m_Components[1]; }
            inline float z() const { return m_Components[2]; }
            inline float w() const { return m_Components[3]; }
            inline float x(float newX) { m_Components[0] = newX; }
            inline float y(float newY) { m_Components[1] = newY; }
            inline float z(float newZ) { m_Components[2] = newZ; }
            inline float w(float newW) { m_Components[3] = newW; }

            inline Vector4 operator-(const Vector4& b) { Vector<4>::Subtract(b); return *this; }
            inline Vector4 operator+(const Vector4& b) { Vector<4>::Add(b); return *this; }
            inline Vector4 operator*(const Vector4& b) { Vector<4>::Multiply(b); return *this; }
            inline Vector4 operator*(float scalar) { Vector<4>::Multiply(scalar); return *this; }
        };
    }
}

#endif