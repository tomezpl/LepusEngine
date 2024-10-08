#include "../ApiGL.h"

using namespace lepus::gfx;

void GraphicsApiGL::Init(GraphicsApiOptions* options)
{
    InitInternal<GraphicsApiGLOptions>((GraphicsApiGLOptions*)options);

    m_Scene = GLSceneGraph();
}

void GraphicsApiGL::SetupVertexArrays()
{
    // Create a global VAO and bind it.
    glCreateVertexArrays(1, &m_Pipeline.vao);
    glBindVertexArray(m_Pipeline.vao);
}

// TODO: rename this, or move to SetupVertexArrays?
void GraphicsApiGL::SetupBuffers()
{
    glBindVertexArray(m_Pipeline.vao);
}

void GraphicsApiGL::SetupShaders()
{
    // Zero the program array.
    memset(m_Programs, 0, GraphicsApiGLOptions::ProgramCount * sizeof(GLuint));

    for (size_t i = 0; i < GraphicsApiGLOptions::ProgramCount; i++)
    {
	auto& options = GetOptions<GraphicsApiGLOptions>();
	GLuint fragShader = options.GetFragmentShader(i), vertShader = options.GetVertexShader(i);
	if (fragShader && vertShader)
	{
	    m_Programs[i] = glCreateProgram();
	    glAttachShader(m_Programs[i], fragShader);
	    glAttachShader(m_Programs[i], vertShader);
	    glLinkProgram(m_Programs[i]);
	}
    }
}

void GraphicsApiGL::SetupUniforms()
{
    // Proj matrix
    auto* proj = new lepus::gfx::GLMatrixUniformBinding(glGetUniformLocation(m_Programs[0], LEPUS_GFX_UNIFORMS_GLOBAL_PROJECTION_MATRIX));
    m_Pipeline.uniforms.push_front((lepus::gfx::GLUniformBinding<void*>*)(proj));
    m_Pipeline.uniformMap.insert_or_assign(LEPUS_GFX_UNIFORMS_GLOBAL_PROJECTION_MATRIX, reinterpret_cast<lepus::gfx::GLUniformBinding<void*>*>(proj));

    // View matrix
    auto* view = new lepus::gfx::GLMatrixUniformBinding(glGetUniformLocation(m_Programs[0], LEPUS_GFX_UNIFORMS_GLOBAL_VIEW_MATRIX));
    m_Pipeline.uniforms.push_front((lepus::gfx::GLUniformBinding<void*>*)(view));
    m_Pipeline.uniformMap.insert_or_assign(LEPUS_GFX_UNIFORMS_GLOBAL_VIEW_MATRIX, reinterpret_cast<lepus::gfx::GLUniformBinding<void*>*>(view));

    // Model matrix
    auto* model = new lepus::gfx::GLMatrixUniformBinding(glGetUniformLocation(m_Programs[0], LEPUS_GFX_UNIFORMS_GLOBAL_MODEL_MATRIX));
    m_Pipeline.uniforms.push_front((lepus::gfx::GLUniformBinding<void*>*)(model));
    m_Pipeline.uniformMap.insert_or_assign(LEPUS_GFX_UNIFORMS_GLOBAL_MODEL_MATRIX, reinterpret_cast<lepus::gfx::GLUniformBinding<void*>*>(model));
}

void GraphicsApiGL::CreatePipeline()
{
    SetupVertexArrays();
    SetupBuffers();
    SetupShaders();
    SetupUniforms();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void GraphicsApiGL::UpdateUniforms()
{
    for (auto uniform = m_Pipeline.uniforms.begin(); uniform != m_Pipeline.uniforms.end(); uniform++)
    {
	auto uniformVal = *uniform;
	if (uniformVal->IsDirty())
	{
	    const GLint& location = uniformVal->Location();
	    switch (uniformVal->Type())
	    {
	    // TODO: with more shaders, we'll want to wrap the uniform objects in "instances" that specify a program handle and hold a reference to the uniform data
	    // However, it'll work for now given there is only ever one GL program in use.
	    case lepus::gfx::UniformType::MATRIX4:
		glUniformMatrix4fv(location, 1, true, (reinterpret_cast<lepus::gfx::GLMatrixUniformBinding*>(uniformVal))->Value());
		break;
	    case lepus::gfx::UniformType::FLOAT:
		glUniform1f(location, (reinterpret_cast<lepus::gfx::GLFloatUniformBinding*>(uniformVal))->Value());
		break;
	    }
	}
    }
}

void GraphicsApiGL::Draw()
{
    glUseProgram(m_Programs[0]);

    glBindVertexArray(m_Pipeline.vao);

    const GLSceneGraph::Node* currentNode = m_Scene.Root();

    bool branchComplete = false;

    while (currentNode)
    {
	if (!branchComplete && !currentNode->IsRoot())
	{
	    typedef lepus::gfx::Renderable<GLMesh> GLRenderable;
	    auto renderable = (const GLRenderable*)(currentNode->GetTransformable());
	    if (renderable)
	    {
		lepus::math::Matrix4x4 modelMat = renderable->GetTransform().BuildMatrix();
		auto modelMatUniformLoation = GetUniform<GLuint, GLMatrixUniformBinding>(LEPUS_GFX_UNIFORMS_GLOBAL_MODEL_MATRIX);
		// TODO: integrate this with UpdateUniforms somehow, the model matrix uniform needs to be different for each renderable!
		glUniformMatrix4fv(modelMatUniformLoation->Location(), 1, true, modelMat.data());

		glBindBuffer(GL_ARRAY_BUFFER, renderable->GetMesh()->GetVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->GetMesh()->GetIBO());

		glDrawElements(GL_TRIANGLES, (GLsizei)renderable->GetMesh()->IndexCount(), GL_UNSIGNED_INT, 0);
	    }
	}

	if (!branchComplete && currentNode->FirstChild())
	{
	    currentNode = currentNode->FirstChild();
	}
	else if (currentNode->NextSibling())
	{
	    currentNode = currentNode->NextSibling();
	    branchComplete = false;
	}
	else
	{
	    branchComplete = true;
	    currentNode = currentNode->Parent();
	}
    }

    // for (uint8_t meshIndex = 0; meshIndex < _meshCount; meshIndex++)
    // {
    // 	glBindBuffer(GL_ARRAY_BUFFER, m_Pipeline.vbo[meshIndex]);
    // 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Pipeline.ibo[meshIndex]);

    // 	glDrawElements(GL_TRIANGLES, (GLsizei)m_Meshes[meshIndex].IndexCount(), GL_UNSIGNED_INT, 0);
    // }
}

void GraphicsApiGL::ClearFrameBuffer(float r, float g, float b)
{
    glClearColor(r, g, b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsApiGL::Shutdown()
{
    GraphicsApi::Shutdown();
}