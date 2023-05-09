#ifndef L3D_GRAPHICSENGINE_APIS_APIGL
#define L3D_GRAPHICSENGINE_APIS_APIGL

#include "../GraphicsApi.h"
#include "../ShaderCompilers/ShaderCompilerGLSL.h"
#include "../../GraphicsEngine.h"

typedef unsigned int GLuint;

namespace LepusEngine
{
	namespace Lepus3D
	{
		class GraphicsApiGLOptions : public GraphicsApiOptions
		{
		public:
			static const size_t ProgramCount = 8;
		private:
			GLuint m_FragmentShaders[ProgramCount];
			GLuint m_VertexShaders[ProgramCount];
			size_t m_ShaderCount = 0;
		public:
			GraphicsApiType GetType() override { return GraphicsApiOpenGL; }

			GraphicsApiGLOptions() : GraphicsApiOptions()
			{
				// Zero the shader arrays.
				memset(m_FragmentShaders, 0, ProgramCount * sizeof(GLuint));
				memset(m_VertexShaders, 0, ProgramCount * sizeof(GLuint));
			}

			inline GLuint const GetFragmentShader(size_t index) { return m_FragmentShaders[index]; }
			inline GLuint const GetVertexShader(size_t index) { return m_VertexShaders[index]; }

			const size_t RegisterShader(ShaderCompiledResult const* vertexShader = nullptr, ShaderCompiledResult const* fragShader = nullptr, ShaderCompiledResult const* geomShader = nullptr)
			{
				assert(m_ShaderCount < ProgramCount);

				if (vertexShader)
				{
					m_VertexShaders[m_ShaderCount] = vertexShader->GetShaderHandle<GLuint>();
				}
				
				if (fragShader)
				{
					m_FragmentShaders[m_ShaderCount] = fragShader->GetShaderHandle<GLuint>();
				}

				if (geomShader)
				{
					// TODO
				}

				return m_ShaderCount++;
			}
		};

		template GraphicsApiGLOptions& GraphicsApi::GetOptions<GraphicsApiGLOptions>();

		class GraphicsApiGL : public GraphicsApi
		{
			friend class GraphicsApiGLOptions;
		private:
			struct
			{
				/// @brief Handle to the vertex array objects.
				GLuint vao;
				
				/// @brief Handle to the global VBO.
				GLuint vbo;

				/// @brief Handle to the global IBO.
				GLuint ibo;
			} m_Pipeline;

			GLuint m_Programs[GraphicsApiGLOptions::ProgramCount];
		private:
			void SetupVertexArrays();
			void SetupBuffers();
			void SetupShaders();
		public:
			GraphicsApiGL(GraphicsApiGLOptions options)
			{
				Init(&options);
			}

			void Init(GraphicsApiOptions* options) override;

			void CreatePipeline() override;

			void Draw() override;

			void ClearFrameBuffer(float r, float g, float b) override;
			
			/// @brief Dummy method as OpenGL itself doesn't need to do anything for the swap chain to work.
			void SwapBuffers() override {}

			void Shutdown() override;
		};

		template GraphicsApiGL& GraphicsEngine::GetApi<GraphicsApiGL>();
	}
}

#endif