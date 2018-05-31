#include "../Scene.h"

using namespace LepusEngine;
using namespace Lepus3D;

Scene::Scene()
{
	m_ObjArr.clear();
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
	return GetSize() - 1;
}

int Scene::GetSize()
{
	return m_ObjArr.size();
}

void Scene::RemoveMesh(int index)
{
	m_ObjArr.erase(m_ObjArr.begin() + (index - 1));
}
