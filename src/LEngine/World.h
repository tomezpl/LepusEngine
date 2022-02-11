#pragma once

#include <vector>

namespace LepusEngine
{
	class Entity;

	namespace Lepus3D
	{
		class Scene;
	}

	class World
	{
	private:
		std::vector<Entity*> mEntities;
		Lepus3D::Scene* mScene;

		// True if a scene pointer passed to the constructor. If the World has its own Scene, false.
		bool mIsSceneShared;

	public:
		World(Lepus3D::Scene* scene = nullptr, bool create3DScene = false);

		Entity* Instantiate(Entity entity);
	};
}