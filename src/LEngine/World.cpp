#include <LEngine/World.h>
#include <L3D/Scene.h>
#include <LEngine/Entity.h>

using namespace LepusEngine;

World::World(Lepus3D::Scene* scene, bool create3DScene)
{
	if (scene == nullptr && create3DScene)
	{
		mScene = new Lepus3D::Scene();
		mIsSceneShared = false;
	}
	else {
		mScene = scene;
		mIsSceneShared = true;
	}
}

Entity* World::Instantiate(Entity entity)
{
	// Make a copy of the entity that we'll use in this world.
	Entity* instantiated = new Entity(entity);

	instantiated->SetWorld(this);

	// Add it to the world.
	mEntities.push_back(instantiated);

	// Return a pointer to the instantiated entity.
	return instantiated;
}