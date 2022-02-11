#include <L3D/Renderable.h>
#include <LEngine/Physics/PhysicsRigidbody.h>
#include <L3D/Mesh.h>

using namespace LepusEngine;
using namespace Lepus3D;

Renderable::Renderable()
{
	mMesh = nullptr;
}

Renderable::Renderable(Mesh m)
{
	SetMesh(m);
}

void Renderable::SetMesh(Mesh m)
{
	mMesh = new Mesh(m);
}

Mesh* Renderable::GetMesh()
{
	return mMesh;
}

Renderable::~Renderable()
{
	delete mMesh;
	mMesh = nullptr;
}
