#ifndef LEPUS_GFX_SHADER_SHADERUTILS
#define LEPUS_GFX_SHADER_SHADERUTILS

namespace lepus::gfx
{

// This macro defines shader's uniform/pushConstant model as a struct, and stores the members in a reflection string for the engine to be able to provide metadata.
#define DEFINE_SHADER_MODEL(name, members) \
    struct name                            \
        members;                           \
    const char __lepus_gfx_shadermodel_def_##name##__[] = ## #members;

#define USE_SHADER_MODEL(modelName, attribName, value)                                                             \
    #attribName, value,                                                                                            \
    {                                                                                                              \
	lepus::gfx::MaterialAttributes::BindingHintType::BindingHintAgnosticModel, offsetof(modelName, attribName) \
    }

#define USE_REGISTERED_SHADER_MODEL(shaderName, attribName, value) USE_SHADER_MODEL(generatedShaderModel_##shaderName, attribName, value)

    // Creates a shader model struct from the provided members and name, then registers it using the shader name and variadic shader code list, in this order:
    // 1. vertex shader
    // 2. fragment shader
#define REGISTER_SHADER(engineObject, shaderName, shaderModelMembers, ...)     \
    DEFINE_SHADER_MODEL(generatedShaderModel_##shaderName, shaderModelMembers) \
    const auto* shaderName##_shader = engineObject##.RegisterShader<generatedShaderModel_##shaderName>(#shaderName, __VA_ARGS__)

} // namespace lepus::gfx

#endif