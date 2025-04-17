#ifndef L3D_GRAPHICSENGINE_SHADERCOMPILER_VK
#define L3D_GRAPHICSENGINE_SHADERCOMPILER_VK

#include "lepus/gfx/GraphicsEngine/ShaderCompiler.h"

#include <cassert>
#include <vulkan/vulkan_core.h>

namespace lepus
{
    namespace gfx
    {
	class ShaderCompilerVk : public ShaderCompiler<VkShaderModule>
	{
	    private:
	    static ShaderCompilerVk m_Instance;
	    bool m_Initialised;
	    friend class GraphicsApiVk;
	    VkDevice m_vkDevice;

	    public:
	    static constexpr size_t ShaderStageCount()
	    {
		return 2;
	    }

	    // Inherited via ShaderCompiler
	    virtual void Init() override
	    {
		m_Initialised = true;
		m_vkDevice = VK_NULL_HANDLE;
	    }

	    inline ShaderCompiledResult<VkShaderModule> CompileShader(const void* shaderSource, size_t shaderSrcLength, ShaderStage type) override
	    {
		assert(shaderSrcLength <= SIZE_MAX);
		assert(m_vkDevice != VK_NULL_HANDLE);

		size_t szShaderCode = shaderSrcLength + (shaderSrcLength % 4);
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = szShaderCode;
		createInfo.pCode = (uint32_t*)(shaderSource);
		VkShaderModule shaderModule = VK_NULL_HANDLE;
		vkCreateShaderModule(m_vkDevice, &createInfo, VK_NULL_HANDLE, &shaderModule);

		return ShaderCompiledResult<VkShaderModule>(shaderModule);
	    }

	    static ShaderCompilerVk& Singleton()
	    {
		if (!m_Instance.m_Initialised)
		{
		    m_Instance.Init();
		}

		return m_Instance;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif
