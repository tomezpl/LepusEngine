#include "../Renderable.h"
#include <LEngine/Physics/PhysicsRigidbody.h>

using namespace LepusEngine;
using namespace Lepus3D;

Renderable::Renderable()
{
	mMesh = nullptr;
	mRigidbody = nullptr;
}

Renderable::Renderable(Mesh m, bool enablePhysics, Physics* physicsEngine)
{
	SetMesh(m, enablePhysics, physicsEngine);
}

::LepusEngine::PhysicsRigidbody* LepusEngine::Lepus3D::Renderable::GetPhysicsRigidbody()
{
	return mRigidbody;
}

void Renderable::SetMesh(Mesh m, bool enablePhysics, Physics* physicsEngine)
{
	mMesh = new Mesh(m);
	if (enablePhysics)
	{
		mRigidbody = new ::LepusEngine::PhysicsRigidbody(*physicsEngine, m, mTransform);
	}
}

Mesh* Renderable::GetMesh()
{
	return mMesh;
}

Renderable::~Renderable()
{
	delete mMesh;
	mMesh = nullptr;

	if(mRigidbody)
		delete mRigidbody;
	mRigidbody = nullptr;
}
