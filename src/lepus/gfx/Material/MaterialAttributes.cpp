#include "MaterialAttributes.h"

// template <typename TValue, typename TRaw>
// lepus::gfx::MaterialAttributeView<TValue, TRaw> lepus::gfx::MaterialAttributes::Add(const char* name, TValue value)
// {
//     UniformType dataType = getDataType<TValue>();
//     utility::List<TValue>& targetDataList = getDataList<TValue>(dataType);
//
//     size_t numChars = strlen(name);
//     const char* newName = new char[numChars + 1];
//     memcpy((void*)newName, name, numChars);
//     memset((void*)(newName + numChars), 0, sizeof(char));
//     m_AttribNames.Push(newName);
//
//     auto newValueIndex = targetDataList.Count();
//     m_AttribTypes.Push(dataType);
//     Set<TValue>(newValueIndex, value);
//     m_AttribIndex.Push(newValueIndex);
//
//     ++m_AttribCount;
//     return lepus::gfx::MaterialAttributeView<TValue, TRaw>(targetDataList, newValueIndex, m_AttribCount, dataType);
// }