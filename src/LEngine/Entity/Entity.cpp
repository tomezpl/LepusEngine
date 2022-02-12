#include <LEngine/Entity.h>
#include <L3D/Renderable.h>
#include <L3D/Transform/Vector.h>
#include <L3D/Quaternion.h>
#include <LEngine/Physics/PhysicsRigidbody.h>
#include <bullet/btBulletDynamicsCommon.h>

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
	mTransform = new Lepus3D::Transform(mRigidbody->GetTransform());

	if(HasRenderable())
	{
		Lepus3D::Transform& renderableTransform = mRenderable->GetTransformRef();
		renderableTransform = *mTransform;
	}
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


void Entity::SetScale(float scale)
{
	SetScale(Lepus3D::Vector3(scale, scale, scale));
}

void Entity::SetScale(Lepus3D::Vector3 scale)
{
	if (HasPhysics())
	{
		mRigidbody->GetBtRigidbody()->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	}

	if (mIsTransformSharedWithRenderable)
	{
		mRenderable->SetScale(scale);
	}
	else
	{
		CopyRigidbodyTransform();
	}
}

void Entity::SetPosition(Lepus3D::Vector3 position)
{
	if (HasPhysics())
	{
		mRigidbody->GetBtRigidbody()->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));
	}

	if (mIsTransformSharedWithRenderable)
	{
		mRenderable->SetPosition(position);
	}
	else
	{
		CopyRigidbodyTransform();
	}
}

void Entity::SetRotation(Lepus3D::Quaternion rotation)
{
	if (HasPhysics())
	{
		mRigidbody->GetBtRigidbody()->getWorldTransform().setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
	}

	if (mIsTransformSharedWithRenderable)
	{
		mRenderable->SetRotation(rotation);
	}
	else
	{
		CopyRigidbodyTransform();
	}
}

Lepus3D::Transform Entity::GetTransform()
{
	return *mTransform;
}