#ifndef L3D_GRAPHICSENGINE_APIS_APIGL
#define L3D_GRAPHICSENGINE_APIS_APIGL

#include <lepus/gfx/GraphicsEngine/GraphicsApi.h>
#include "../ShaderCompilers/ShaderCompilerGLSL.h"
#include <lepus/gfx/GraphicsEngine.h>
#include <forward_list>
#include <unordered_map>

#include "ApiGL/Bindings.h"
#include "ApiGL/Types/GLMesh.h"
#include <lepus/gfx/SceneGraph.h>

namespace lepus
{
    namespace gfx
    {
	class GraphicsApiGLOptions : public GraphicsApiOptions
	{
	    public:
	    static constexpr size_t s_ProgramCount = 8;

	    private:
	    GLuint m_FragmentShaders[s_ProgramCount];
	    GLuint m_VertexShaders[s_ProgramCount];
	    size_t m_ShaderCount = 0;
	    std::shared_ptr<system::WindowingGLFW> m_Windowing;

	    public:
	    GraphicsApiType GetType() override { return GraphicsApiOpenGL; }
	    inline system::Windowing* GetWindowing() override
	    {
		return m_Windowing.get();
	    }

	    GraphicsApiGLOptions(std::shared_ptr<system::WindowingGLFW> windowing)
	        : GraphicsApiOptions()
	    {
		// Zero the shader arrays.
		memset(m_FragmentShaders, 0, s_ProgramCount * sizeof(GLuint));
		memset(m_VertexShaders, 0, s_ProgramCount * sizeof(GLuint));
		m_Windowing = windowing;
	    }

	    [[nodiscard]] inline GLuint GetFragmentShader(size_t index) const { return m_FragmentShaders[index]; }
	    [[nodiscard]] inline GLuint GetVertexShader(size_t index) const { return m_VertexShaders[index]; }

	    size_t RegisterShader(GLShaderCompiledResult const* vertexShader = nullptr, GLShaderCompiledResult const* fragShader = nullptr, GLShaderCompiledResult const* geomShader = nullptr)
	    {
		assert(m_ShaderCount < s_ProgramCount);

		if (vertexShader)
		{
		    m_VertexShaders[m_ShaderCount] = vertexShader->ShaderHandle;
		}

		if (fragShader)
		{
		    m_FragmentShaders[m_ShaderCount] = fragShader->ShaderHandle;
		}

		if (geomShader)
		{
		    // TODO
		}

		return m_ShaderCount++;
	    }
	};

	typedef lepus::gfx::SceneGraph GLSceneGraph;

	typedef lepus::gfx::Renderable<GLMesh> GLRenderable;

	template GraphicsApiGLOptions& GraphicsApi::GetOptions<GraphicsApiGLOptions>();

	class GraphicsApiGL : public GraphicsApi
	{
	    friend class GraphicsApiGLOptions;

	    struct
	    {
		/// @brief Handle to the vertex array objects.
		GLuint vao = 0;

		/// @brief List with all uniforms used by the API.
		// TODO: Change to array - might get better cache/locality to improve access times.
		std::forward_list<lepus::gfx::GLUniformBinding<void*>*> uniforms;

		/// @brief Uniform map to update the values.
		// TODO: Move to a Material class?
		std::unordered_map<const char*, lepus::gfx::GLUniformBinding<void*>*> uniformMap;

		std::vector<std::tuple<GLenum, GLuint>> samplers{};
	    } m_Pipeline;

	    GLuint m_Programs[GraphicsApiGLOptions::s_ProgramCount];

	    bool m_DrawStarted;

	    GLuint m_ActiveProgram;

	    GLenum m_TextureTypes[1] = {GL_TEXTURE_2D};
	    std::map<GLuint, GLuint> m_TextureSamplers{};
	    std::map<GLuint, GLenum> m_Textures{};

	    private:
	    void SetupVertexArrays();
	    void SetupBuffers();
	    void SetupShaders();
	    void SetupUniforms();
	    void SetupSamplers();

	    private:
	    inline void* GetUniformInternal(const char* name) override
	    {
		size_t targetKeyLength = strlen(name);

		// TODO: unordered_map doesn't really work with string keys... add actual hashing!
		for (auto it = m_Pipeline.uniformMap.begin(); it != m_Pipeline.uniformMap.end(); it++)
		{
		    size_t keyLength = strlen(it->first);
		    if (targetKeyLength == keyLength && !strcmp(name, it->first))
		    {
			return it->second;
		    }
		}

		return nullptr;
	    }

	    public:
	    GraphicsApiGL(GraphicsApiGLOptions options)
	    {
		GraphicsApiGL::Init(&options);
	    }

	    GraphicsApiGL(GraphicsApiGLOptions* options)
	    {
		GraphicsApiGL::Init(options);
	    }

	    void Init(GraphicsApiOptions* options) override;

	    void CreatePipeline() override;

	    void UpdateUniforms(const SceneGraph& scene) override;

	    void UpdateUniforms(const SceneGraph& scene, const GLRenderable* renderable, MaterialAttributes& materialAttribs, GLuint program, const lepus::math::Matrix4x4& worldMatrix);

	    void Draw(const SceneGraph& scene) override;

	    void ClearFrameBuffer(float r, float g, float b) override;

	    /// @brief Dummy method as OpenGL itself doesn't need to do anything for the swap chain to work.
	    void SwapBuffers() override {}

	    void Shutdown() override;

	    const char* GetShaderFileName(const char* shaderName, ShaderStage stage) const override;

	    [[nodiscard]] engine::ShaderAssetType GetShaderAssetType() const override
	    {
		return engine::ShaderAssetTypeGLSL;
	    }

	    inline engine::objects::Mesh* WrapMesh(engine::objects::Mesh* mesh) override { return new GLMesh((void*)mesh->GetVertices(), mesh->VertexBufferSize(), mesh->GetFormat(), (uint32_t*)mesh->GetIndices(), mesh->IndexCount(), true); }

	    TextureHandle AddTexture(const engine::TextureAsset& textureAsset) override;
	};

	template lepus::gfx::GLUniformBinding<void*>* GraphicsApi::GetUniform<lepus::gfx::GLUniformBinding<void*>*>(const char* name);

	// template GraphicsApiGL& GraphicsEngine::GetApi<GraphicsApiGL>();
    } // namespace gfx
} // namespace lepus

#endif