#ifndef LEPUS_GFX_SHADER
#define LEPUS_GFX_SHADER
#include "Material/MaterialAttributes.h"
#include "Shader/ShaderUtils.h"

#include <cstring>
#include <utility>

namespace lepus
{
    namespace gfx
    {
	enum ShaderStage
	{
	    ShaderStageInvalid = 0,
	    ShaderStageVertex = 1 << 0,
	    ShaderStageFragment = 1 << 1
	};

	/// @brief A base class for any shader. Defines metadata about the shader, such as its name.
	class ShaderInfo
	{
	    private:
	    const char* m_ShaderName;
	    ShaderStage m_ShaderStages = ShaderStage::ShaderStageInvalid;

	    protected:
	    inline void _InitShaderName(const char* shaderName)
	    {
		m_ShaderName = new char[strlen(shaderName) + 1];
		memcpy((void*)(m_ShaderName), shaderName, sizeof(char) * (strlen(shaderName) + 1));
	    }

	    public:
	    explicit ShaderInfo(const char* shaderName, ShaderStage shaderStageUnion)
	    {
		_InitShaderName(shaderName);
		m_ShaderStages = shaderStageUnion;
	    }

	    ShaderInfo(ShaderInfo&& other)
	    {
		m_ShaderName = other.m_ShaderName;
		other.m_ShaderName = nullptr;
	    }

	    ShaderInfo& operator=(ShaderInfo&& other)
	    {
		m_ShaderName = other.m_ShaderName;
		other.m_ShaderName = nullptr;

		return *this;
	    }

	    ShaderInfo(const ShaderInfo& copy)
	    {
		_InitShaderName(copy.m_ShaderName);
	    }

	    ShaderInfo& operator=(const ShaderInfo& copy)
	    {
		_InitShaderName(copy.m_ShaderName);
	    }

	    virtual ~ShaderInfo()
	    {
		if (m_ShaderName)
		{
		    delete[] m_ShaderName;
		    m_ShaderName = nullptr;
		}
	    }
	};

	struct ShaderUniformModelBase
	{
	    lepus::gfx::MaterialAttributeMatrix4 MODEL;
	    lepus::gfx::MaterialAttributeMatrix4 VIEW;
	    lepus::gfx::MaterialAttributeMatrix4 PROJ;
	};

	/// @brief An API-specific shader class that implements a handle getter for that API's shader objects (e.g. shader handle integer in GL, objects in D3D/Vk etc.).
	template <typename TApiNativeShaderHandle = void*, class TUniformModel = ShaderUniformModelBase>
	class Shader : public ShaderInfo
	{
	    public:
	    virtual TApiNativeShaderHandle GetApiHandle() const = 0;

	    static const type_info& GetUniformModel()
	    {
		return typeid(TUniformModel);
	    }

	    explicit Shader(ShaderInfo&& other)
	        : ShaderInfo(std::move(other))
	    {
	    }

	    explicit Shader(const ShaderInfo& copy)
	        : ShaderInfo(copy)
	    {
	    }

	    Shader& operator=(ShaderInfo&& other)
	    {
		*this = std::move(other);
		return *this;
	    }
	};

	template <class UniformModel = ShaderUniformModelBase>
	using AnyShader = Shader<void*, UniformModel>;
    } // namespace gfx
} // namespace lepus

#endif
