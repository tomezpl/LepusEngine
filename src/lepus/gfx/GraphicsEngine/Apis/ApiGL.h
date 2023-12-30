#ifndef L3D_GRAPHICSENGINE_APIS_APIGL
#define L3D_GRAPHICSENGINE_APIS_APIGL

#include <lepus/gfx/GraphicsEngine/GraphicsApi.h>
#include "../ShaderCompilers/ShaderCompilerGLSL.h"
#include <lepus/gfx/GraphicsEngine.h>
#include <lepus/utility/Primitives.h>
#include <forward_list>
#include <unordered_map>

#include "ApiGL/Bindings.h"
#include "ApiGL/Types/GLMesh.h"

namespace lepus
{
	namespace gfx
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

			const size_t RegisterShader(GLShaderCompiledResult const* vertexShader = nullptr, GLShaderCompiledResult const* fragShader = nullptr, GLShaderCompiledResult const* geomShader = nullptr)
			{
				assert(m_ShaderCount < ProgramCount);

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

		template GraphicsApiGLOptions& GraphicsApi::GetOptions<GraphicsApiGLOptions>();

		class GraphicsApiGL : public GraphicsApi
		{
			friend class GraphicsApiGLOptions;
			private:
			static const uint8_t _meshCount = 2;
			struct
			{
				/// @brief Handle to the vertex array objects.
				GLuint vao;

				/// @brief Handle to the global VBO.
				GLuint vbo[_meshCount] = { 0, 0 };

				/// @brief Handle to the global IBO.
				GLuint ibo[_meshCount] = { 0, 0 };

				/// @brief List with all uniforms used by the API.
				// TODO: Change to array - might get better cache/locality to improve access times.
				std::forward_list<lepus::gfx::GLUniformBinding<void*>*> uniforms;

				/// @brief Uniform map to update the values.
				// TODO: Move to a Material class?
				std::unordered_map<const char*, lepus::gfx::GLUniformBinding<void*>*> uniformMap;
			} m_Pipeline;

			GLuint m_Programs[GraphicsApiGLOptions::ProgramCount];

			GLMesh m_Meshes[_meshCount];

			private:
			void SetupVertexArrays();
			void SetupBuffers();
			void SetupShaders();
			void SetupUniforms();

			private:
			inline void* GetUniformInternal(char* name) override
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
				Init(&options);
			}

			void Init(GraphicsApiOptions* options) override;

			void CreatePipeline() override;

			void UpdateUniforms() override;

			void Draw() override;

			void ClearFrameBuffer(float r, float g, float b) override;

			/// @brief Dummy method as OpenGL itself doesn't need to do anything for the swap chain to work.
			void SwapBuffers() override {}

			void Shutdown() override;
		};

		template const lepus::gfx::GLUniformBinding<void*>* GraphicsApi::GetUniform<lepus::gfx::GLUniformBinding<void*>*>(char* name);

		template GraphicsApiGL& GraphicsEngine::GetApi<GraphicsApiGL>();
	}
}

#endif