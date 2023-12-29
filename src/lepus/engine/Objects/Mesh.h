#ifndef LEPUS_ENGINE_OBJECTS_MESH
#define LEPUS_ENGINE_OBJECTS_MESH

#include <cstdlib>
#include <memory>
#include <lepus/utility/Primitives.h>

namespace lepus
{
    namespace engine
    {
        /// @brief Different vertex formats supported by the engine.
        enum MeshVertexFormat
        {
            /// @brief (Default) 3 positional components (XYZ) for each vertex
            VVV,
            Invalid
        };

        namespace objects
        {
            class Mesh
            {
                private:
                MeshVertexFormat m_Format = MeshVertexFormat::Invalid;

                void* m_Vertices = nullptr;
                size_t m_szVertices = 0;

                uint32_t* m_Indices = nullptr;
                size_t m_IndexCount = 0;

                bool m_OwnData = false;
                bool m_IsIndexed = false;

                protected:
                inline virtual void InitInternal()
                {

                }

                public:
#define LEPUS_MESH_CONSTRUCTOR(MeshClass) \
                inline MeshClass(lepus::engine::MeshVertexFormat format = lepus::engine::MeshVertexFormat::VVV) {Init(format);} \
                inline MeshClass(const lepus::utility::Primitive& primitive, bool copy = false) {Init((void*)primitive.GetVertices(), primitive.VertexBufferSize(), lepus::engine::MeshVertexFormat::VVV, (uint32_t*)primitive.GetIndices(), primitive.IndexCount(), copy);} \
                inline MeshClass(void* vertices, size_t szVertices, lepus::engine::MeshVertexFormat format = lepus::engine::MeshVertexFormat::VVV, uint32_t* indices = nullptr, size_t indexCount = 0, bool copy = false) {Init(vertices, szVertices, format, indices, indexCount, copy);} \
                inline virtual ~MeshClass() {Dispose();}

                LEPUS_MESH_CONSTRUCTOR(Mesh);

                inline void Init(MeshVertexFormat format = MeshVertexFormat::VVV)
                {
                    m_Format = format;
                    m_OwnData = true;

                    InitInternal();
                }

                inline void Init(const lepus::utility::Primitive& primitive, bool copy = false)
                {
                    Init((void*)primitive.GetVertices(), primitive.VertexBufferSize(), MeshVertexFormat::VVV, (uint32_t*)primitive.GetIndices(), primitive.IndexCount(), copy);
                }

                inline void Init(void* vertices, size_t szVertices, MeshVertexFormat format = MeshVertexFormat::VVV, uint32_t* indices = nullptr, size_t indexCount = 0, bool copy = false)
                {
                    m_Format = format;
                    m_IsIndexed = indexCount > 0 && indices != nullptr;
                    m_szVertices = szVertices;

                    if (copy)
                    {
                        m_Vertices = malloc(szVertices);
                        memcpy(m_Vertices, vertices, szVertices);

                        if (m_IsIndexed)
                        {
                            m_IndexCount = indexCount;
                            m_Indices = new uint32_t[indexCount];
                            memcpy(m_Indices, indices, indexCount * sizeof(uint32_t));
                        }

                        m_OwnData = true;
                    }
                    else
                    {
                        m_Vertices = vertices;

                        if (m_IsIndexed)
                        {
                            m_IndexCount = indexCount;
                            m_Indices = indices;
                        }

                        m_OwnData = false;
                    }

                    InitInternal();
                }

                inline MeshVertexFormat GetFormat() const { return m_Format; }

                const void* GetVertices()
                {
                    return m_Vertices;
                }

                const uint32_t* GetIndices()
                {
                    return m_Indices;
                }

                const uint8_t GetSingleVertexSize()
                {
                    switch (m_Format)
                    {
                        case MeshVertexFormat::VVV:
                            return sizeof(float) * 3;
                        case MeshVertexFormat::Invalid:
                        default:
                            return 0;
                    }
                }

                const size_t inline VertexCount()
                {
                    return m_szVertices / GetSingleVertexSize();
                }

                constexpr size_t const inline VertexBufferSize()
                {
                    return m_szVertices;
                }

                constexpr size_t const inline IndexBufferSize()
                {
                    return m_IndexCount * sizeof(uint32_t);
                }

                const size_t inline IndexCount()
                {
                    return m_IndexCount;
                }

                inline void Dispose()
                {
                    if (m_OwnData)
                    {
                        if (m_Vertices)
                        {
                            free(m_Vertices);
                        }

                        if (m_Indices)
                        {
                            delete[] m_Indices;
                        }
                    }
                }
            };
        }
    } // namespace engine

} // namespace lepus


#endif