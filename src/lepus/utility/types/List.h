#ifndef LEPUS_UTILITY_TYPES_LIST
#define LEPUS_UTILITY_TYPES_LIST
#include <cassert>
#include <cstring>

namespace lepus
{
    namespace utility
    {
	template <typename TElement>
	class List
	{
	    private:
	    size_t m_Elements;
	    size_t m_Capacity;
	    TElement* m_Data;
	    size_t m_ElementSize;

	    public:
	    List(const TElement* const data, size_t numElements)
	    {
		m_Data = new TElement[numElements];
		memcpy(m_Data, data, numElements * sizeof(TElement));
		m_Elements = numElements;
		m_Capacity = numElements + 1;
		m_ElementSize = sizeof(TElement);
	    }

	    List()
	    {
		m_Capacity = 1;
		m_Data = new TElement[m_Capacity];
		m_Elements = 0;
		m_ElementSize = sizeof(TElement);
	    }

	    [[nodiscard]] inline size_t Capacity() const
	    {
		return m_Capacity;
	    }

	    [[nodiscard]] inline size_t Count() const
	    {
		return m_Elements;
	    }

	    template <size_t index>
	    inline TElement& Get() const
	    {
		assert(index < m_Elements);
		return m_Data[index];
	    }

	    inline TElement& Get(size_t index) const
	    {
		assert(index < m_Elements);
		return m_Data[index];
	    }

	    inline TElement* Raw()
	    {
		return m_Data;
	    }

	    inline void Set(size_t index, const TElement& value)
	    {
		assert(index < m_Elements);

		m_Data[index] = value;
	    }

	    inline size_t Push(const TElement& element)
	    {
		EnsureCapacity(m_Elements + 1);
		m_Data[m_Elements] = element;
		return ++m_Elements;
	    }

	    template <size_t index>
	    inline size_t Remove()
	    {
		assert(index < m_Elements);
		return Remove(index);
	    }

	    inline size_t Remove(size_t index)
	    {
		assert(index < m_Elements);
		size_t slice = (m_Elements - index) - 1;
		if (slice)
		{
		    memmove(m_Data + index, m_Data + index + 1, slice * m_ElementSize);
		}

		return --m_Elements;
	    }

	    size_t EnsureCapacity(size_t capacity)
	    {
		if (m_Capacity < capacity)
		{
		    size_t newCapacity = m_Capacity;
		    // TODO: this could probably be sped up if we kept capacities at powers of 2
		    while (newCapacity < capacity)
		    {
			newCapacity *= 2;
		    }
		    TElement* newData = new TElement[newCapacity];
		    memcpy((void*)newData, m_Data, m_Elements * m_ElementSize);
		    delete[] m_Data;
		    m_Data = newData;
		    m_Capacity = newCapacity;
		}

		return m_Capacity;
	    }

	    size_t UnsafeSetCount(size_t count)
	    {
		m_Elements = count;
		return m_Elements;
	    }

	    ~List()
	    {
		delete[] m_Data;
	    }
	};
    } // namespace utility
} // namespace lepus

#endif