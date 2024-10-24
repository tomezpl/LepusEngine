#include "../ApiGL.h"

#include "Types/GLShader.h"

using namespace lepus::gfx;

void GraphicsApiGL::Init(GraphicsApiOptions* options)
{
    InitInternal<GraphicsApiGLOptions>((GraphicsApiGLOptions*)options);

    m_Scene = GLSceneGraph();

    m_DrawStarted = false;

    m_ActiveProgram = 0;
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
}

void GraphicsApiGL::SetupUniforms()
{
}

void GraphicsApiGL::CreatePipeline()
{
    SetupVertexArrays();
    SetupBuffers();
    // SetupShaders();
    // SetupUniforms();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void GraphicsApiGL::UpdateUniforms()
{
    // TODO: this method should only update "global" uniforms that aren't specific to any renderable in particular.
}

void GraphicsApiGL::UpdateUniforms(const GLRenderable* const renderable, const MaterialAttributes& materialAttribs, const GLuint program, const lepus::math::Matrix4x4& worldMatrix)
{
    auto cam = m_Scene.Camera();
    auto proj = cam->BuildPerspectiveMatrix();
    auto view = cam->BuildViewMatrix();

    uint8_t count = materialAttribs.Count();
    bool appliedMvp = false;
    const float* mvpMatrixData = nullptr;
    for (uint8_t i = 0; !appliedMvp || i < count; i++)
    {
	if (!appliedMvp && i >= 3)
	{
	    i = 0;
	    appliedMvp = true;

	    if (count == 0)
	    {
		break;
	    }
	}

	const char* name = appliedMvp ? materialAttribs.GetName(i) : nullptr;
	UniformType type = appliedMvp ? materialAttribs.GetType(i) : UniformType::INVALID;
	if (!appliedMvp)
	{
	    type = UniformType::MATRIX4;
	    switch (i)
	    {
	    case 0:
		name = LEPUS_GFX_UNIFORMS_GLOBAL_MODEL_MATRIX;
		mvpMatrixData = worldMatrix.data();
		break;
	    case 1:
		name = LEPUS_GFX_UNIFORMS_GLOBAL_PROJECTION_MATRIX;
		mvpMatrixData = proj.data();
		break;
	    case 2:
		name = LEPUS_GFX_UNIFORMS_GLOBAL_VIEW_MATRIX;
		mvpMatrixData = view.data();
		break;
	    }
	}

	const GLint location = glGetUniformLocation(program, name);

	switch (type)
	{
	// TODO: with more shaders, we'll want to wrap the uniform objects in "instances" that specify a program handle and hold a reference to the uniform data
	// However, it'll work for now given there is only ever one GL program in use.
	case lepus::gfx::UniformType::MATRIX4:
	    glUniformMatrix4fv(location, 1, true, appliedMvp ? materialAttribs.Get<lepus::math::Matrix4x4>(i).data() : mvpMatrixData);
	    break;
	case lepus::gfx::UniformType::FLOAT:
	    glUniform1f(location, materialAttribs.Get<GLfloat>(i));
	    break;
	case lepus::gfx::UniformType::INVALID:
	default:
	    break;
	}
    }
}

void GraphicsApiGL::Draw()
{
    glBindVertexArray(m_Pipeline.vao);

    const GLSceneGraph::Node* currentNode = m_Scene.Root();

    bool branchComplete = false;

    while (currentNode)
    {
	if (!branchComplete && !currentNode->IsRoot())
	{
	    auto renderable = (const GLRenderable*)(currentNode->GetTransformable());
	    auto material = renderable->GetMaterial();
	    if (renderable)
	    {
		// Get the OpenGL shader for this renderable's material and use it for this drawcall.
		GLuint program = material->GetShader<GLShader>()->GetApiHandle();
		glUseProgram(program);

		UpdateUniforms(renderable, material->Attributes(), program, renderable->GetWorldMatrix(currentNode));

		glBindBuffer(GL_ARRAY_BUFFER, renderable->GetMesh()->GetVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->GetMesh()->GetIBO());

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(renderable->GetMesh()->IndexCount()), GL_UNSIGNED_INT, 0);
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

    m_DrawStarted = false;
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