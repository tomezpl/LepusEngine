#ifndef L3D_GRAPHICSENGINE_API_VK_SHADER
#define L3D_GRAPHICSENGINE_API_VK_SHADER
#include "lepus/gfx/Shader.h"

#include <cassert>
#include <vulkan/vulkan_core.h>

namespace lepus
{
    namespace gfx
    {
	class VkShader : public Shader<const VkPipelineShaderStageCreateInfo*>
	{
	    private:
	    friend class GraphicsApiVk;
	    GraphicsApiVk* m_Api;

	    VkPipelineShaderStageCreateInfo m_ShaderStageCreateInfo[2] = {{}, {}};
	    static const char ENTRY_POINT_NAME[];

	    public:
	    VkShader() = delete;

	    explicit VkShader(const ShaderInfo& shaderInfo)
	        : Shader<const VkPipelineShaderStageCreateInfo*>(shaderInfo)
	    {
		VkPipelineShaderStageCreateInfo defaultInfo = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM, VK_NULL_HANDLE, ENTRY_POINT_NAME, nullptr};
		m_ShaderStageCreateInfo[0] = defaultInfo;
		m_ShaderStageCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		m_ShaderStageCreateInfo[1] = defaultInfo;
		m_ShaderStageCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	    }

	    inline void SetShaderModule(VkShaderModule shaderModule, ShaderStage stage)
	    {
		assert(shaderModule != VK_NULL_HANDLE);

		switch (stage)
		{
		case ShaderStageVertex:
		    m_ShaderStageCreateInfo[0].module = shaderModule;
		    break;
		case ShaderStageFragment:
		    m_ShaderStageCreateInfo[1].module = shaderModule;
		    break;
		case ShaderStageInvalid:
		default:
		    assert(false);
		}
	    }

	    [[nodiscard]] const VkPipelineShaderStageCreateInfo* GetApiHandle() const override
	    {
		return m_ShaderStageCreateInfo;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif