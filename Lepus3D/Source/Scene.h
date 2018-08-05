#pragma once

#include "Renderable.h"
#include "Transform.h"
#include "Light.h"
#include <vector>>

namespace LepusEngine {
	namespace Lepus3D {
		class Scene {
		friend class RenderEngine;
		private:
			std::vector<Renderable*> m_ObjArr;
			std::vector<Light*> m_LightArr;
			Color m_AmbientColor;
			float m_AmbientIntensity;
		public:
			Scene();

			// Returns the index of the newly added mesh
			int AddMesh(Mesh mesh, Transform t = Transform(Vector3::Zero(), Vector3::Zero(), Vector3::Create(1.f, 1.f, 1.f)));
			int AddMesh(Renderable* renderable);

			// Returns the index of the newly added light
			int AddLight(Light* light);

			// Returns the length of the scene's ObjArr
			int GetRenderableCount();

			int GetLightCount();

			// Removes a mesh from the scene at a specified index
			void RemoveMesh(int index);

			void RemoveLight(int index);

			void SetAmbient(Color ambientColor);
			void SetAmbient(float ambientIntensity);
			void SetAmbient(Color ambientColor, float ambientIntensity);
		};
	}
}
