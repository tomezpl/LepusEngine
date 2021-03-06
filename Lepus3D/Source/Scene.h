#pragma once

#include "Renderable.h"
#include "Transform.h"
#include "Light.h"
#include <vector>>

namespace LepusEngine {
	namespace Lepus3D {
		// Scene class
		// Stores references to objects that appear in or contribute to the rendered scene (e.g. Renderables, Lights)
		// Describes Ambient Lighting properties.
		class Scene {
		friend class RenderEngine;
		private:
			std::vector<Renderable*> m_ObjArr;
			std::vector<Light*> m_LightArr;
			Color m_AmbientColor;
			float m_AmbientIntensity;
		public:
			// Default constructor. Creates a white ambient light with 0.5 intensity.
			Scene();

			// Adds mesh to the Scene and wraps it in a Renderable.
			// Returns the index of the newly added mesh.
			int AddMesh(Mesh mesh, Transform t = Transform(Vector3::Zero(), Vector3::Zero(), Vector3::Create(1.f, 1.f, 1.f)));

			// Adds mesh to the Scene, provided it's already wrapped in a Renderable object.
			// Returns the index of the newly added mesh.
			int AddMesh(Renderable* renderable);

			// Adds light to the Scene.
			// Returns the index of the newly added light.
			int AddLight(Light* light);

			// Returns the length of the scene's ObjArr
			int GetRenderableCount();

			// Returns the length of the scene's LightArr
			int GetLightCount();

			// Removes a mesh from the scene at a specified index
			void RemoveMesh(int index);

			// Removes a light from the scene at a specified index
			void RemoveLight(int index);

			// Sets Ambient Light colour
			void SetAmbient(Color ambientColor);

			// Sets Ambient Light intensity
			void SetAmbient(float ambientIntensity);

			// Sets Ambient Light colour & intensity
			void SetAmbient(Color ambientColor, float ambientIntensity);
		};
	}
}
