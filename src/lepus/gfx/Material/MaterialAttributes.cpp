#include "MaterialAttributes.h"

using namespace lepus::gfx;

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

MaterialAttributeView<MaterialAttributeTexture, void*> MaterialAttributes::Add(const char* name, lepus::engine::TextureAsset& value, const MaterialAttributes::BindingHint&& bindingHint)
{
    auto dataType = getDataType<MaterialAttributeTexture>();
    utility::List<MaterialAttributeTexture>& targetDataList = getDataList<MaterialAttributeTexture>(dataType);

    size_t numChars = strlen(name);
    const char* newName = new char[numChars + 1];
    memcpy((void*)newName, name, numChars);
    memset((void*)(newName + numChars), 0, sizeof(char));
    m_AttribNames.Push(newName);

    auto newValueIndex = targetDataList.Count();
    ++m_AttribCount;
    m_AttribTypes.Push(dataType);
    m_AttribIndex.Push(static_cast<uint8_t>(newValueIndex));
    m_AttribBindingHints.Push(bindingHint);
    targetDataList.Push({value.width, value.height, std::shared_ptr<uint8_t[]>(reinterpret_cast<uint8_t*>(value.data)), {}, true});
    // targetDataList.EnsureCapacity(newValueIndex + 1);
    // Set<TRaw>(newValueIndex, value);
    // targetDataList.UnsafeSetCount(newValueIndex + 1);

    return lepus::gfx::MaterialAttributeView<MaterialAttributeTexture, void*>(reinterpret_cast<utility::List<void*>&>(targetDataList), static_cast<uint8_t>(newValueIndex), static_cast<MaterialAttributeHandle>(m_AttribCount - 1), dataType);
}