#ifndef LUTILITY_PRIMITIVES
#define LUTILITY_PRIMITIVES

#include <cassert>
#include <memory>
#include <cstring>

namespace lepus
{
    namespace utility
    {
        class Primitive
        {
            private:
            float* m_Verts;
            size_t m_VertCount;
            uint32_t* m_Indices;
            size_t m_IndexCount;

            public:
            /// @brief Number of floats for each vertex. Currently, we only store position (XYZ).
            static const unsigned char NbComponents = 3;

            // Move constructor
            Primitive(Primitive&& other)
            {
                Primitive();

                m_Verts = other.m_Verts;
                m_VertCount = other.m_VertCount;
                m_Indices = other.m_Indices;
                m_IndexCount = other.m_IndexCount;

                // Prevent the moved resource from altering the pointers.
                other.m_Verts = nullptr;
                other.m_Indices = nullptr;
            }

            Primitive& operator=(Primitive&& other)
            {
                if (this != &other)
                {
                    if (m_Verts)
                    {
                        delete[] m_Verts;
                    }

                    if (m_Indices)
                    {
                        delete[] m_Indices;
                    }

                    m_Verts = other.m_Verts;
                    m_Indices = other.m_Indices;
                    m_VertCount = other.m_VertCount;
                    m_IndexCount = other.m_IndexCount;
                }

                return *this;
            }

            /// @brief Creates a new primitive with the provided vertices.
            /// @param vertexPositions Array of tightly packed XYZ positions for each vertex.
            /// @param nbVertexPositions Number of vertex positions - do not confuse with length of vertexPositions.
            Primitive(const float* const vertexPositions, size_t nbVertexPositions, const uint32_t* const indices, size_t nbIndices)
            {
                m_Verts = nullptr;
                m_VertCount = 0;
                m_Indices = nullptr;
                m_IndexCount = 0;

                Init(vertexPositions, nbVertexPositions, indices, nbIndices);
            }

            Primitive()
            {
                m_Verts = nullptr;
                m_VertCount = 0;
                m_Indices = nullptr;
                m_IndexCount = 0;
            }

            /// @brief Initialises the primitive with the provided vertices.
            /// @param vertexPositions Array of tightly packed XYZ positions for each vertex.
            /// @param nbVertexPositions Number of vertex positions - do not confuse with length of vertexPositions.
            void Init(const float* const vertexPositions, size_t nbVertexPositions, const uint32_t* const indices, size_t nbIndices)
            {
                assert(m_Verts == nullptr);

                m_Verts = new float[nbVertexPositions * NbComponents];
                m_VertCount = nbVertexPositions;
                memcpy(m_Verts, vertexPositions, nbVertexPositions * NbComponents * sizeof(float));

                m_Indices = new uint32_t[nbIndices];
                m_IndexCount = nbIndices;
                memcpy(m_Indices, indices, nbIndices * sizeof(uint32_t));
            }

            const float* GetVertices() const
            {
                return m_Verts;
            }

            const uint32_t* GetIndices() const
            {
                return m_Indices;
            }

            const size_t inline VertexCount() const
            {
                return m_VertCount;
            }

            constexpr size_t const inline VertexBufferSize() const
            {
                return m_VertCount * sizeof(float) * NbComponents;
            }

            constexpr size_t const inline IndexBufferSize() const
            {
                return m_IndexCount * sizeof(uint32_t);
            }

            const size_t inline IndexCount() const
            {
                return m_IndexCount;
            }
        };

        class Primitives
        {
            private:
            static Primitives _shared;
            public:
            static Primitives& Shared() { return _shared; };

#define LEPUS_UTILITY_PRIMITIVE_SHARED(PrimitiveName) \
private:\
Primitive m_ ## PrimitiveName ## Instance = Create ## PrimitiveName(); \
public:\
static const inline Primitive& PrimitiveName() {return Shared().m_ ## PrimitiveName ## Instance;}

            public:
            static const inline Primitive CreateCube()
            {
                // Assuming negative left and negative up
                const float cubeVertices[] = {
                    // Front:
                    -0.5f, 0.5f, -0.5f, // top-left (0)
                    0.5f, 0.5f, -0.5f, // top-right (1)
                    -0.5f, -0.5f, -0.5f, // bottom-left (2)
                    0.5f, -0.5f, -0.5f, // bottom-right (3)

                    // Left:
                    -0.5f, 0.5f, 0.5f, // top-left (4)
                    // use front top-left (0) as top-right
                    -0.5f, -0.5f, 0.5f, // bottom-left (5)
                    // use front bottom-left (2) as bottom-right

                    // Back:
                    0.5f, 0.5f, 0.5f, // top-left (6)
                    // reuse left top-left (4) as top-right
                    0.5f, -0.5f, 0.5f, // bottom-left (7)
                    // use left bottom-left (5) as bottom-right

                    // Top:
                    // use left top-left (4) as top-left
                    // use back top-left (6) as top-right
                    // use left top-right (0) as bottom-left
                    // use front top-right (1) as bottom-right

                    // Right:
                    // use front top-right (1) as top-left
                    // use back top-left (6) as top-right
                    // use front bottom-right (3) as bottom-left
                    // use back bottom-left (7) as bottom-right

                    // Bottom:
                    // use front bottom-left (2) as top-left
                    // use front bottom-right (3) as top-right
                    // use back bottom-right (5) as bottom-left
                    // use back bottom-left (7) as bottom-right
                };

                const uint32_t indices[] = {
                    // Front:
                    1, 3, 0,
                    3, 2, 0,


                    // Left:
                    0, 2, 4,
                    2, 5, 4,


                    // Back:
                    5, 6, 4,
                    5, 7, 6,

                    // Top:
                    1, 4, 6,
                    1, 0, 4,

                    // Right:
                    7, 1, 6,
                    7, 3, 1,

                    // Bottom:
                    7, 2, 3,
                    7, 5, 2
                };

                return Primitive(cubeVertices, sizeof(cubeVertices) / sizeof(float) / 3, indices, sizeof(indices) / sizeof(uint32_t));
            }

            LEPUS_UTILITY_PRIMITIVE_SHARED(Cube);
        };
    }
}

#endif