#ifndef _LEPUS_WORLD_
#define _LEPUS_WORLD_

#include <L3D/Renderable.h>
#include <LEngine/Physics/PhysicsRigidbody.h>
#include <map>

namespace LepusEngine {
	namespace Game {
		class Tickable {
		public:
			virtual void Update() = 0;
		};

		class Entity;

		// Base (internal) component types
		enum ComponentType { TYPE_COMPONENT_RENDERABLE, TYPE_COMPONENT_PHYSICS_RIGIDBODY };

		class Component : public Tickable {
		protected:
			std::map<std::string, void*> m_Properties;
			bool m_Enabled;
			Entity* m_Owner;
		public:
			void Enable();
			void Disable();
			bool IsEnabled();

			virtual ComponentType Type();
			virtual ComponentType BaseType() = 0;
		};

		class World;

		class Entity : public Tickable {
			friend class World;
		protected:
			std::vector<Entity*> m_Children;
			std::string m_Name;
			std::vector<Component*> m_Components;
			World* m_World;
		public:
			Entity(std::string name, std::vector<Component*> componentList = std::vector<Component*>(), std::vector<Entity*> childEntities = std::vector<Entity*>());
			void AddChildEntity(Entity* childEntity);

			// Update all Tickables linked to this Entity. Includes direct Components as well as Child Entities and their Components.
			virtual void Update();

			std::string GetName();
		};

		class TransformableEntity : public Entity {
		protected:
			Lepus3D::Transform m_Transform;
		public:
			TransformableEntity(std::string name, std::vector<Component*> componentList = std::vector<Component*>(), std::vector<Entity*> childEntities = std::vector<Entity*>());
			Lepus3D::Transform const& GetTransform();
		};

		class PhysicsRigidbodyComponent : public Component {
		protected:
			LepusEngine::PhysicsRigidbody* m_PhysicsRigidbody;
			LepusEngine::Physics* m_Physics;
		public:
			PhysicsRigidbodyComponent(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform, float frameRate = 60.0f);
			void Update();
			~PhysicsRigidbodyComponent();
			PhysicsRigidbody* GetPhysicsRigidbody();
			
			virtual ComponentType BaseType();
		};

		class RenderableComponent : public Component {
		private:
			Lepus3D::Renderable m_Renderable;
		public:
			RenderableComponent(Lepus3D::Mesh& mesh);
			Lepus3D::Renderable* GetRenderable();

			virtual void Update();
			virtual ComponentType BaseType();
		};

		class World {
		private:
			std::vector<Entity*> m_Entities;
			Lepus3D::Scene* m_3DScene;
			bool m_HasScene;
			bool m_HasPhysics;
			Physics* m_PhysicsEngine;
			std::map<std::string, void*> m_Properties;
		public:
			// Creates a new World. If physicsEngine is a valid Physics object, then that engine will be attached to this world.
			World(bool create3DScene = true, Physics* physicsEngine = nullptr);
			void AttachScene(Lepus3D::Scene* scene);
			void AttachPhysics(Physics* physicsEngine);
			void AddEntity(Entity* entity);
			Game::Entity * GetEntity(std::string name);
			void Update(float frameRate = 60.0f);
			Lepus3D::Scene* Get3DScene();
		};
	}
}

#endif