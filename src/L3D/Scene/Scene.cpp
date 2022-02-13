#include <L3D/Scene.h>
#include <L3D/Mesh.h>

using namespace LepusEngine;
using namespace Lepus3D;

Scene::Scene()
{
	m_ObjArr.clear();
	m_LightArr.clear();
	Color ambient(255, 255, 255, 255);
	m_AmbientColor = ambient;
	m_AmbientIntensity = 0.5f;
}

int Scene::AddMesh(Mesh m, Transform t)
{
	Renderable* obj = new Renderable(m);
	obj->mTransform = t;
	return this->AddMesh(obj);
}

int Scene::AddMesh(Renderable* renderable)
{
	Mesh* mesh = renderable->GetMesh();

	if (!mesh->HasGLBuffers())
	{
		GLuint ibo, vbo, vao;

		glGenBuffers(1, &ibo);
		glGenBuffers(1, &vbo);

		glGenVertexArrays(1, &vao);
		mesh->GLSetVAO(vao);

		mesh->GLSetIBO(ibo);
		mesh->GLSetVBO(vbo);

		mesh->GLUpdateIBO();
		mesh->GLUpdateVBO();
	}

	m_ObjArr.push_back(renderable);
	//m_PhysicsEngine->AddObject(*renderable->mRigidbody);
	return GetRenderableCount() - 1;
}

int Scene::AddLight(Light* light)
{
	m_LightArr.push_back(light);
	return GetLightCount() - 1;
}

int Scene::GetRenderableCount()
{
	return m_ObjArr.size();
}

int Scene::GetLightCount()
{
	return m_LightArr.size();
}

void Scene::RemoveLight(int index)
{
	m_LightArr.erase(m_LightArr.begin() + (index - 1));
}

void Scene::RemoveMesh(int index)
{
	m_ObjArr.erase(m_ObjArr.begin() + (index - 1));
}

void Scene::SetAmbient(Color color)
{
	m_AmbientColor = color;
}

void Scene::SetAmbient(float intensity)
{
	m_AmbientIntensity = intensity;
}

void Scene::SetAmbient(Color color, float intensity)
{
	SetAmbient(color);
	SetAmbient(intensity);
}

void Scene::Shutdown()
{
	size_t nbRenderables = GetRenderableCount();
	for (size_t i = 0; i < nbRenderables; i++)
	{
		Mesh* mesh = m_ObjArr[i]->GetMesh();
		if (mesh->HasGLBuffers())
		{
			glDeleteBuffers(1, &mesh->GLGetVBO());
			glDeleteBuffers(1, &mesh->GLGetIBO());
			glDeleteVertexArrays(1, &mesh->GLGetVAO());
		}
	}
}