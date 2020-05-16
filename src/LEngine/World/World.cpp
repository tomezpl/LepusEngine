#include <LEngine/World.h>
#include <L3D/Scene.h>
#include <iterator>

using namespace LepusEngine;

// ComponentType returned for base types will just be their base type
Game::ComponentType Game::Component::Type()
{
	return BaseType();
}

Game::ComponentType Game::RenderableComponent::BaseType()
{
	return ComponentType::TYPE_COMPONENT_RENDERABLE;
}

Game::ComponentType Game::PhysicsRigidbodyComponent::BaseType()
{
	return ComponentType::TYPE_COMPONENT_PHYSICS_RIGIDBODY;
}

Game::Entity::Entity(std::string name, std::vector<Component*> componentList, std::vector<Entity*> childEntities)
{
	m_Name = name;
	m_Components = componentList;
	m_Children = childEntities;
	m_World = nullptr;
}

void Game::Entity::AddChildEntity(Entity* childEntity)
{
	m_Children.push_back(childEntity);
}

void Game::Entity::Update()
{
	// Update Components.
	for (std::vector<Component*>::iterator it = m_Components.begin(); it < m_Components.end(); it++)
	{
		(*it)->Update();
	}

	// Update Child Entities.
	for (std::vector<Entity*>::iterator it = m_Children.begin(); it < m_Children.end(); it++)
	{
		(*it)->Update();
	}
}

std::string Game::Entity::GetName()
{
	return m_Name;
}

Game::TransformableEntity::TransformableEntity(std::string name, std::vector<Game::Component*> componentList, std::vector<Game::Entity*> childEntities) : Game::Entity(name, componentList, childEntities)
{
	// dummy
}

Lepus3D::Transform const& Game::TransformableEntity::GetTransform()
{
	return m_Transform;
}

Game::World::World(bool create3DScene, Physics* physicsEngine)
{
	if (create3DScene)
	{
		AttachScene(new Lepus3D::Scene());
	}
	else
	{
		m_3DScene = nullptr;
		m_HasScene = false;
	}

	if (physicsEngine != nullptr)
	{
		AttachPhysics(physicsEngine);
	}
	else
	{
		m_HasPhysics = false;
		m_PhysicsEngine = nullptr;
	}
}

void Game::World::AttachPhysics(Physics* physicsEngine)
{
	m_HasPhysics = true;
	m_PhysicsEngine = physicsEngine;
}

void Game::World::AttachScene(Lepus3D::Scene* scene)
{
	m_3DScene = scene;
	m_HasScene = true;

	// TODO: add already created Renderables from this World to the Scene
}

void Game::World::AddEntity(Entity* entity)
{
	m_Entities.push_back(entity);
	entity->m_World = this;
	for (std::vector<Game::Component*>::iterator it = entity->m_Components.begin(); it != entity->m_Components.end(); it++)
	{
		if ((*it)->BaseType() == TYPE_COMPONENT_RENDERABLE)
		{
			m_3DScene->AddMesh(((RenderableComponent*)(*it))->GetRenderable());
		}
	}
}

Game::Entity* Game::World::GetEntity(std::string name)
{
	// Find the entity with specified name or return null otherwise
	for (std::vector<Entity*>::iterator it = m_Entities.begin(); it < m_Entities.end(); it++)
	{
		if ((*it)->GetName() == name)
			return (*it);
	}

	return nullptr;
}

Lepus3D::Scene* LepusEngine::Game::World::Get3DScene()
{
	return m_3DScene;
}

void Game::World::Update(float frameRate)
{
	// Update belonging Entities.
	for (std::vector<Entity*>::iterator it = m_Entities.begin(); it < m_Entities.end(); it++)
	{
		(*it)->Update();
	}

	// Update physics if necessary.
	if (m_HasPhysics)
	{
		m_PhysicsEngine->Run(frameRate);
	}
}