#include "../Scene.h"

using namespace LepusEngine;
using namespace Lepus3D;

Scene::Scene()
{
	m_MeshArr.clear();
	m_TransfArr.clear();
}

int Scene::AddMesh(Mesh m, Transform t)
{
	m_MeshArr.push_back(m);
	m_TransfArr.push_back(t);
	return GetSize() - 1;
}

int Scene::GetSize()
{
	return m_MeshArr.size();
}

void Scene::RemoveMesh(int index)
{
	m_MeshArr.erase(m_MeshArr.begin() + (index - 1));
	m_TransfArr.erase(m_TransfArr.begin() + (index - 1));
}