#ifndef LEPUS_GFX_MATERIAL
#define LEPUS_GFX_MATERIAL
#include "Shader.h"
#include "Material/MaterialAttributes.h"

namespace lepus
{
    namespace gfx
    {
	/// @brief A Material class. Defines what shader will be used to render an object, and the parameters applied to that shader.
	class Material
	{
	    protected:
	    const ShaderInfo* m_Shader;

	    lepus::gfx::MaterialAttributes m_Attributes;

	    public:
	    template <typename TShaderHandle = ShaderInfo>
	    [[nodiscard]] inline TShaderHandle* const GetShader() const
	    {
		return (TShaderHandle* const)m_Shader;
	    }

	    [[nodiscard]] inline const ShaderInfo* GetShaderInfo() const
	    {
		return m_Shader;
	    }

	    Material()
	    {
		m_Shader = nullptr;
	    }

	    void SetShader(const ShaderInfo* const shader)
	    {
		m_Shader = shader;
	    }

	    [[nodiscard]] const lepus::gfx::MaterialAttributes& Attributes() const
	    {
		return m_Attributes;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif
