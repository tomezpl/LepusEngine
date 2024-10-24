#ifndef LEPUS_GFX_MATERIAL_ATTRIBUTES
#define LEPUS_GFX_MATERIAL_ATTRIBUTES
#include "lepus/gfx/GraphicsEngine/GraphicsApi/Uniforms.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace lepus
{
    namespace gfx
    {
	class MaterialAttributes
	{
	    private:
	    uint8_t m_AttribCount;

	    /// @brief Number of elements to allocate whenever capacity is exceeded.
	    static constexpr uint8_t ALLOC_CHUNK_SIZE = 8;

	    const char** m_AttribNames;

	    void** m_AttribValues;

	    UniformType* m_AttribTypes;

	    inline void _EnsureCapacity(uint8_t toAdd)
	    {
		uint8_t required = (uint8_t)(m_AttribCount + toAdd);
		uint8_t currentCapacity = (uint8_t)(ALLOC_CHUNK_SIZE * (m_AttribNames ? ceil(m_AttribCount / ALLOC_CHUNK_SIZE) : 0));

		if (required > currentCapacity)
		{
		    const char* const* names = m_AttribNames;
		    const void* const* values = m_AttribValues;
		    const UniformType* types = m_AttribTypes;

		    uint8_t newCapacity = (uint8_t)(currentCapacity + ALLOC_CHUNK_SIZE);

		    m_AttribNames = new const char*[newCapacity];
		    m_AttribValues = new void*[newCapacity];
		    m_AttribTypes = new UniformType[newCapacity];

		    memcpy((void*)m_AttribNames, names, sizeof(const char*) * m_AttribCount);
		    memcpy((void*)m_AttribValues, values, sizeof(const void*) * m_AttribCount);
		    memcpy((void*)m_AttribTypes, types, sizeof(UniformType) * m_AttribCount);

		    memset((void*)(m_AttribNames + m_AttribCount * sizeof(const char*)), 0, sizeof(const char*) * (newCapacity - m_AttribCount));
		    memset((void*)(m_AttribValues + m_AttribCount * sizeof(const void*)), 0, sizeof(const void*) * (newCapacity - m_AttribCount));
		    memset((void*)(m_AttribTypes + m_AttribCount * sizeof(UniformType)), 0, sizeof(UniformType) * (newCapacity - m_AttribCount));

		    delete[] names;
		    delete[] values;
		    delete[] types;
		}
	    }

	    protected:
	    template <typename TValue = const void*>
	    inline void UnsafeSet(uint8_t index, TValue value)
	    {
		assert(index < m_AttribCount);

		memcpy((void*)m_AttribValues[index], &value, sizeof(TValue));
	    }

	    public:
	    MaterialAttributes()
	    {
		m_AttribCount = 0;
		m_AttribNames = nullptr;
		m_AttribValues = nullptr;
	    }

	    /// @brief Adds an attribute and returns its index.
	    template <typename TValue = const void*>
	    uint8_t Add(const char* name, TValue value)
	    {
		this->_EnsureCapacity(1);

		size_t numChars = strlen(name);
		m_AttribNames[m_AttribCount] = new char[numChars];
		memcpy((void*)m_AttribNames[m_AttribCount], name, numChars);
		m_AttribValues[m_AttribValues] = value;

		return m_AttribCount++;
	    }

	    template <typename TValue = const void*>
	    void Set(uint8_t index, TValue value)
	    {
		assert(index < m_AttribCount);

		m_AttribValues[index] = value;
	    }

	    template <>
	    inline void Set<float>(uint8_t index, float value)
	    {
		m_AttribTypes[index] = UniformType::FLOAT;
		UnsafeSet(index, value);
	    }

	    template <>
	    inline void Set<const lepus::math::Matrix4x4&>(uint8_t index, const lepus::math::Matrix4x4& value)
	    {
		m_AttribTypes[index] = UniformType::MATRIX4;
		memcpy((void*)m_AttribValues[index], value.data(), sizeof(float) * 4 * 4);
	    }

	    template <typename TValue = void*>
	    inline TValue Get(uint8_t index) const
	    {
		assert(index < m_AttribCount);

		return m_AttribValues[index];
	    }

	    inline const char* GetName(uint8_t index) const
	    {
		assert(index < m_AttribCount);

		return m_AttribNames[index];
	    }

	    inline const uint8_t Count() const
	    {
		return m_AttribCount;
	    }

	    inline const UniformType GetType(uint8_t index) const
	    {
		return m_AttribTypes[index];
	    }

	    ~MaterialAttributes()
	    {
		for (uint8_t i = 0; i < m_AttribCount; i++)
		{
		    delete[] m_AttribNames[i];
		}

		delete[] m_AttribNames;
		m_AttribNames = nullptr;

		delete[] m_AttribValues;
		m_AttribValues = nullptr;
	    }
	};

	template <>
	inline float MaterialAttributes::Get<float>(uint8_t index) const
	{
	    float ret = 0.f;
	    memcpy((void*)&ret, m_AttribValues[index], sizeof(float));
	    return ret;
	}

	template <>
	inline lepus::math::Matrix4x4 MaterialAttributes::Get<lepus::math::Matrix4x4>(uint8_t index) const
	{
	    lepus::math::Matrix4x4 mat(m_AttribValues[index]);

	    return mat;
	}
    } // namespace gfx
} // namespace lepus

#endif
