#include <LEngine/Entity.h>
#include <L3D/Renderable.h>
#include <LEngine/Physics/PhysicsRigidbody.h>

using namespace LepusEngine;

Entity::Entity(Lepus3D::Renderable* renderable, LepusEngine::PhysicsRigidbody* rigidbody)
{
	mRenderable = renderable == nullptr ? new Lepus3D::Renderable() : renderable;
	mRigidbody = rigidbody;

	if(!HasPhysics())
	{
		mTransform = mRenderable->GetTransformPtr();
		mIsTransformSharedWithRenderable = true;
	}
	else
	{
		CopyRigidbodyTransform();
		mIsTransformSharedWithRenderable = false;
	}

	mWorld = nullptr;
}

void Entity::CopyRigidbodyTransform()
{
	// TODO: Adding a rigidbody AFTER an entity has been created could result in a memory leak.
	mTransform = &mRigidbody->GetTransform();
}

void Entity::Update(float deltaTime)
{
	if (!mIsTransformSharedWithRenderable && HasPhysics())
	{
		CopyRigidbodyTransform();
	}
}

void Entity::SetWorld(LepusEngine::World* world)
{
	mWorld = world;
}