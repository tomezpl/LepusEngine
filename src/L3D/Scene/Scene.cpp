#include "../Scene.h"

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
	m_ObjArr.push_back(renderable);
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
