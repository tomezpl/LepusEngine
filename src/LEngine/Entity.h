#pragma once

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Renderable;
		class Transform;
		class Vector3;
		class Quaternion;
	}

	class World;
	class PhysicsRigidbody;

	class Entity
	{
	private:
		Lepus3D::Renderable* mRenderable;
		LepusEngine::PhysicsRigidbody* mRigidbody;

		// True if the transform is a pointer to the renderable's transform (and NOT reconstructed from the btPhysics rigidbody).
		// This is important as we don't want to delete the Renderable's transform in the destructor, but we don't want to duplicate data.
		bool mIsTransformSharedWithRenderable;

		Lepus3D::Transform* mTransform;

		inline void CopyRigidbodyTransform();

		LepusEngine::World* mWorld;
	public:
		Entity(Lepus3D::Renderable* renderable = nullptr, LepusEngine::PhysicsRigidbody* rigidbody = nullptr);

		inline LepusEngine::PhysicsRigidbody* GetRigidbody() { return mRigidbody; }
		inline Lepus3D::Renderable* GetRenderable() { return mRenderable; }
		inline bool HasPhysics() { return mRigidbody != nullptr; }
		inline bool HasRenderable() { return mRenderable != nullptr; }

		virtual void Update(float deltaTime);

		void SetWorld(LepusEngine::World* world);

		void SetScale(float scale);
		void SetScale(Lepus3D::Vector3 scale);
		void SetPosition(Lepus3D::Vector3 position);
		void SetRotation(Lepus3D::Quaternion rotation);

		Lepus3D::Transform GetTransform();
	};
}