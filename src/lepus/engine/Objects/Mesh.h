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
            /// @brief Base class for Mesh data in the engine.
            ///
            /// Can be specialised for different parts of the engine (e.g.rendering, physics) by inheriting and adding API-specific logic as needed.
            class Mesh
            {
                private:
                /// @brief The vertex format used by the mesh data.
                MeshVertexFormat m_Format = MeshVertexFormat::Invalid;

                /// @brief Vertex data
                void* m_Vertices = nullptr;
                size_t m_szVertices = 0;

                /// @brief Index data (optional, but common enough to be included in the base class for simplicity's sake)
                uint32_t* m_Indices = nullptr;
                size_t m_IndexCount = 0;

                /// @brief Does this Mesh own the vertex/index data? If true, this Mesh will delete the data when Dispose is called.
                bool m_OwnData = false;

                /// @brief Is the Mesh indexed?
                bool m_IsIndexed = false;

                protected:
                inline void CopyInternal(const Mesh& other)
                {
                    m_Format = other.m_Format;
                    m_Vertices = other.m_Vertices;
                    m_szVertices = other.m_szVertices;
                    m_IndexCount = other.m_IndexCount;
                    m_Indices = other.m_Indices;
                    m_IsIndexed = other.m_IsIndexed;

                    // We're not making a copy of the data, so we don't own it.
                    // TODO: Add some kind of reference counting via a resource manager, so if we copy an existing mesh, its data doesn't get disposed until its copies are.
                    m_OwnData = false;
                }

                /// @brief Always called after constructing a new Mesh instance (excluding move/copy ctors).
                inline virtual void InitInternal()
                {

                }

#define LEPUS_MESH_CONSTRUCTOR(MeshClass) \
                public:\
                inline MeshClass(lepus::engine::MeshVertexFormat format = lepus::engine::MeshVertexFormat::VVV) {Init(format);} \
                inline MeshClass(const lepus::utility::Primitive& primitive, bool copy = false) {Init((void*)primitive.GetVertices(), primitive.VertexBufferSize(), lepus::engine::MeshVertexFormat::VVV, (uint32_t*)primitive.GetIndices(), primitive.IndexCount(), copy);} \
                inline MeshClass(void* vertices, size_t szVertices, lepus::engine::MeshVertexFormat format = lepus::engine::MeshVertexFormat::VVV, uint32_t* indices = nullptr, size_t indexCount = 0, bool copy = false) {Init(vertices, szVertices, format, indices, indexCount, copy);} \
                inline virtual ~MeshClass() {Dispose();}

                LEPUS_MESH_CONSTRUCTOR(Mesh);

                public:

                /// @brief Copy constructor
                /// @param other Mesh object to copy
                Mesh(const Mesh& other)
                {
                    CopyInternal(other);
                }

                /// @brief Copy assignment
                /// @param other Mesh object to copy
                /// @return
                Mesh& operator=(const Mesh& other)
                {
                    CopyInternal(other);
                    return *this;
                }

                /// @brief Move constructor
                /// @param other Rvalue to initialise the Mesh object with.
                Mesh(Mesh&& other)
                {
                    m_Format = MeshVertexFormat::Invalid;
                    m_Vertices = nullptr;
                    m_szVertices = 0;
                    m_OwnData = false;
                    m_Indices = nullptr;
                    m_IndexCount = 0;
                    m_IsIndexed = false;

                    *this = std::move(other);
                }

                /// @brief Move assignment
                /// @param other Mesh rvalue to assign
                /// @return The current object with vertices, indices and any metadata from the rvalue. 
                Mesh& operator=(Mesh&& other)
                {
                    // Prevent self-assignment.
                    if (this != &other)
                    {
                        // Make sure our data is disposed first if necessary so memory doesn't leak.
                        Dispose();

                        m_Format = other.m_Format;

                        m_Vertices = other.m_Vertices;
                        m_szVertices = other.m_szVertices;

                        m_Indices = other.m_Indices;
                        m_IndexCount = other.m_IndexCount;

                        m_IsIndexed = other.m_IsIndexed;
                        m_OwnData = other.m_OwnData;

                        other.m_Format = MeshVertexFormat::Invalid;
                        other.m_IndexCount = 0;
                        other.m_szVertices = 0;
                        other.m_Indices = nullptr;
                        other.m_Vertices = nullptr;
                        other.m_OwnData = false;
                        other.m_IsIndexed = false;
                    }

                    return *this;
                }

                /// @brief Default constructor. Initialises an empty mesh with just a vertex format and no vertices.
                /// @param format Vertex format to use.
                inline void Init(MeshVertexFormat format = MeshVertexFormat::VVV)
                {
                    m_Format = format;
                    m_OwnData = false;
                    m_IsIndexed = false;
                    m_szVertices = 0;
                    m_IndexCount = 0;
                    m_Vertices = nullptr;
                    m_Indices = nullptr;

                    InitInternal();
                }

                /// @brief Initialises the Mesh with geometry data from a built-in primitive.
                /// @param primitive A built-in primitive instance from LepusUtility.
                /// @param copy Should the data be copied?
                inline void Init(const lepus::utility::Primitive& primitive, bool copy = false)
                {
                    Init((void*)primitive.GetVertices(), primitive.VertexBufferSize(), MeshVertexFormat::VVV, (uint32_t*)primitive.GetIndices(), primitive.IndexCount(), copy);
                }

                /// @brief Initialises the Mesh with provided geometry data.
                /// @param vertices Vertex data
                /// @param szVertices Size of the vertex data (in bytes)
                /// @param format Vertex data format
                /// @param indices Index data. Optional - pass nullptr for non-indexed.
                /// @param indexCount Number of indices.
                /// @param copy Should the data be copied?
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

                /// @brief Gets the vertex format used by this Mesh.
                /// @return The MeshVertexFormat this Mesh was initialised with.
                inline MeshVertexFormat GetFormat() const { return m_Format; }

                /// @brief Gets the pointer to vertex data used by this Mesh.
                /// @return A constant pointer to the vertex data.
                const void* GetVertices() const
                {
                    return m_Vertices;
                }

                /// @brief Gets the pointer to index data used by this Mesh.
                /// @return A constant pointer to the index data.
                const uint32_t* GetIndices() const
                {
                    return m_Indices;
                }

                /// @brief Gets the size of a single vertex based on the vertex format used by this Mesh.
                /// @return An unsigned 8-bit integer defining a single vertex size.
                uint8_t GetSingleVertexSize() const
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

                /// @brief Gets the number of vertices in this Mesh.
                /// @return An unsigned 64-bit integer defining the number of vertices (TODO: that's a maximum of 18,446,744,073,709,551,615 vertices... do we really need 64-bit for that?)
                size_t inline VertexCount() const
                {
                    return m_szVertices / GetSingleVertexSize();
                }

                /// @brief Gets the size of the vertex data.
                /// @return An unsigned 64-bit integer defining the size of the vertex data (in bytes).
                size_t inline VertexBufferSize() const
                {
                    return m_szVertices;
                }

                /// @brief Gets the size of the index data.
                /// @return An unsigned 64-bit integer defining the size of the index data (in bytes).
                size_t inline IndexBufferSize() const
                {
                    return m_IndexCount * sizeof(uint32_t);
                }

                /// @brief Gets the number of indices in this Mesh.
                /// @return An unsigned 64-bit integer defining the number of indices that make up this Mesh (TODO: again, isn't 64 bits overkill?)
                size_t inline IndexCount() const
                {
                    return m_IndexCount;
                }

                /// @brief Releases any resources and data held by this Mesh as needed.
                inline virtual void Dispose()
                {
                    if (m_OwnData)
                    {
                        if (m_Vertices)
                        {
                            free(m_Vertices);
                            m_Vertices = nullptr;
                            m_szVertices = 0;
                        }

                        if (m_Indices)
                        {
                            delete[] m_Indices;
                            m_Indices = nullptr;
                            m_IndexCount = 0;
                        }
                    }
                }
            };
        }
    }
}

#endif