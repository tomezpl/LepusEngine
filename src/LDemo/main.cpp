#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <L3D/RenderEngine.h>
#include <L3D/Camera/FPPCamera.h>
#include <L3D/Assets.h>
#include <LEngine/Logger.h>
#include <LEngine/Physics.h>
#include <LEngine/Physics/PhysicsRigidbody.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace LepusEngine;

#ifndef _DEBUG
	#define NDEBUG
#endif

std::string ToString(GLfloat* vec, size_t n)
{
	std::string ret = "";

	for (size_t i = 0; i < n; i++)
	{
		ret += std::to_string(vec[i]);
		
		if (i != n - 1)
		{
			ret += ", ";
		}
	}

	return ret;
}

int main()
{
	const aiScene* sponza = aiImportFile("../../Content/Models/sponza/sponza.obj", aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_JoinIdenticalVertices | aiPostProcessSteps::aiProcess_GenNormals);

	std::vector<Lepus3D::Mesh> sponzaMeshes;

	// Enable logging
	LepusEngine::Logger::Enabled = true;

	// Create new graphics engine instance
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	// Setup DearImgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	bool showDemoWindow = true;

	ImGui_ImplGlfw_InitForOpenGL(engine.GetWindowPtr(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// Termination condition for main loop
	bool isRunning = true;

	// Initialise the scene
	Lepus3D::Scene scene;

	Lepus3D::Texture2D testTex("Textures/cube_test.jpg"); // Load texture from file

	Lepus3D::Material defaultMat("DefaultMaterial", "Phong");
	defaultMat.SetAttributeF("_SpecularStrength", 0.2f);
	defaultMat.SetAttributeI("_SpecularShininess", 64);
	defaultMat.SetAttributeF3("_DiffColor", Lepus3D::Color(128, 128, 128, 255).GetVector3());

	std::vector<Lepus3D::Material*> sponzaMaterials;
	sponzaMaterials.reserve(sponza->mNumMaterials);

	for (size_t i = 0; i < sponza->mNumMaterials; i++)
	{
		aiMaterial* currentMaterial = sponza->mMaterials[i];

		Lepus3D::Material* convertedMaterial = new Lepus3D::Material();

		for (size_t j = 0; j < currentMaterial->mNumProperties; j++)
		{
			aiMaterialProperty* currentProp = currentMaterial->mProperties[j];

			aiString key = currentProp->mKey;

			if (key == aiString("?mat.name"))
			{
				aiString materialName = aiString();

				aiGetMaterialString(currentMaterial, key.C_Str(), currentProp->mType, currentProp->mIndex, &materialName);
				
				convertedMaterial->SetName(materialName.C_Str());
			}

			if (key == aiString("$clr.specular") || key == aiString("$clr.diffuse"))
			{
				convertedMaterial->SetShader("Phong");
				float data[3];

				memcpy_s(data, currentProp->mDataLength, currentProp->mData, currentProp->mDataLength);

				if (key == aiString("$clr.specular"))
				{
					convertedMaterial->SetAttributeF("_SpecularStrength", (data[0] + data[1] + data[2]) / 3.f);
				}
				else
				{
					convertedMaterial->SetAttributeF3("_DiffColor", Lepus3D::Vector3(data[0], data[1], data[2]));
				}
			}
			else if (key == aiString("$mat.shinpercent"))
			{
				convertedMaterial->SetAttributeI("_SpecularShininess", (int)std::floor(*(float*)(currentProp->mData)) * 256.f);
			}
			else if (key == aiString("$tex.file"))
			{
				aiString textureFileName = aiString();

				aiGetMaterialString(currentMaterial, key.C_Str(), currentProp->mType, currentProp->mIndex, &textureFileName);

				// Only interested in diffuse (albedo/colour) textures.
				std::string strTexFn = std::string(textureFileName.C_Str());
				if (strTexFn.find("_diff.") != std::string::npos ||
					(
						strTexFn.find("_bump.") == std::string::npos && 
						strTexFn.find("_spec.") == std::string::npos &&
						strTexFn.find("_mask.") == std::string::npos &&
						strTexFn.find("_gloss.") == std::string::npos
					)
				)
				{
					Lepus3D::Texture2D* texture = new Lepus3D::Texture2D(textureFileName.C_Str(), "../../Content/Models/sponza");

					convertedMaterial->SetAttributeTex("_Texture1", *texture);
				}
			}
		}

		sponzaMaterials.push_back(convertedMaterial);
	}

	for (size_t i = 0; i < sponza->mNumMeshes; i++)
	{
		aiMesh* currentMesh = sponza->mMeshes[i];

		Lepus3D::VertexArray verts;
		verts.reserve(currentMesh->mNumVertices);

		for (size_t j = 0; j < currentMesh->mNumVertices; j++)
		{
			aiVector3t currentVertex = currentMesh->mVertices[j];
			aiVector3t currentNormal = currentMesh->mNormals[j];

			verts.push_back(Lepus3D::Vertex(currentVertex.x, currentVertex.y, currentVertex.z, 0.f, 0.f, currentNormal.x, currentNormal.y, currentNormal.z));
		}

		Lepus3D::Renderable* renderable = new Lepus3D::Renderable(Lepus3D::Mesh(verts, true));
		
		Lepus3D::IndexArray indices;
		indices.reserve((size_t)currentMesh->mNumFaces * 3);
		for (size_t j = 0; j < currentMesh->mNumFaces; j++)
		{
			aiFace* currentFace = &currentMesh->mFaces[j];

			for (size_t k = 0; k < currentFace->mNumIndices; k++)
			{
				indices.push_back(currentFace->mIndices[k]);
			}
		}

		renderable->GetMesh()->SetIndices(indices);

		renderable->GetMesh()->SetMaterial(*sponzaMaterials[currentMesh->mMaterialIndex]);
		renderable->SetScale(1.f / 100.f);

		scene.AddMesh(renderable);
	}

	// Create physics engine for this scene
	LepusEngine::Physics physicsEngine = LepusEngine::Physics();

	// Prepare the shading
	Lepus3D::Material testMat("Material", "Phong"); // Use the phong shader, assign material name "Material"
	// Pass attributes to shader
	testMat.SetAttributeF("_SpecularStrength", 0.5f);
	testMat.SetAttributeI("_SpecularShininess", 256);
	testMat.SetAttributeF3("_DiffColor", Lepus3D::Color(100, 149, 237, 255).GetVector3());

	// A box that's going to fall down from (0, 0, 0).
	// It will be 1/4 of the original size.
	LepusEngine::Entity box = LepusEngine::Entity(new Lepus3D::Renderable(Lepus3D::BoxMesh()), new PhysicsRigidbody(physicsEngine, Lepus3D::BoxMeshUnindexed(), Lepus3D::Transform()));
	box.SetScale(0.25f);

	// This will be our static box placed below the first box to test collisions and dynamics
	LepusEngine::Entity box2 = LepusEngine::Entity(new Lepus3D::Renderable(Lepus3D::BoxMesh()), new PhysicsRigidbody(physicsEngine, Lepus3D::BoxMeshUnindexed(), Lepus3D::Transform(Lepus3D::Vector3(0.3f, -15.f, 0.f), Lepus3D::Vector3::Zero(), Lepus3D::Vector3(3.f/2.f, 3.f/2.f, 3.f/2.f)), 0.f));

	// Prepare the lighting
	// A Light is created at xyz(0, 2.5, 0) with a white RGBA colour and intensity 1.0
	// TODO: The light colour doesn't need Alpha, that one channel could go away
	Lepus3D::Light sceneLight(Lepus3D::Vector3(0.0f, 2.50f, 0.0f), Lepus3D::Color(255, 255, 255, 255), 1.0f);

	// Assign materials to meshes
	box.GetRenderable()->GetMesh()->SetMaterial(testMat);
	box2.GetRenderable()->GetMesh()->SetMaterial(testMat);

	// Initialise a transformable FPPCamera (reacts to keyboard & mouse input)
	Lepus3D::FPPCamera cam(*(new Lepus3D::Transform(Lepus3D::Vector3(0.f, 0.f, 0.f), Lepus3D::Vector3::Zero(), Lepus3D::Vector3(1.f, 1.f, 1.f))));
	// Bind to window in order to receive keyboard & mouse input
	cam.SetWindow(engine.GetWindowPtr());

	// Add the box renderable and the light to scene
	scene.AddLight(&sceneLight);
	scene.AddMesh(box.GetRenderable());
	scene.AddMesh(box2.GetRenderable());

	physicsEngine.AddObject(*box.GetRigidbody());
	physicsEngine.AddObject(*box2.GetRigidbody());

	// dTime: delta time between frames
	// elapsedTime: total running time, needed for the scene light to orbit around the box
	double dTime, elapsedTime = 0.0;

	// This will be toggled after hitting space to start/stop physics simulation
	bool physicsActive = false;

	double lastYRot = 0.0;

	size_t materialCount = sponzaMaterials.size();

	// Output start message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo starting!");
	while (isRunning)
	{
		// Get delta time from graphics engine
		dTime = engine.LastFrameTime();
		// Add delta time to update total running time
		elapsedTime += dTime;

		// Orbit the light around the box over the application's running time
		sceneLight.SetPosition(Lepus3D::Vector3(2.50f * sin(elapsedTime), 2.50f * sin(elapsedTime), 2.50f * cos(elapsedTime)));

		if (physicsActive)
		{
			physicsEngine.Run(dTime);
		}

		Lepus3D::Transform boxCurrentPose = box.GetTransform();
		Lepus3D::Vector3 euler = boxCurrentPose.GetRotation().ToEuler();

		euler.y = lastYRot + (double)dTime * 1.;
		lastYRot = euler.y;

		box.Update(dTime);
		box2.Update(dTime);

		engine.Update(); // Update window before drawing

		if (cam.LockInput)
		{
			cam.ProcessInput(dTime); // Move camera according to input using delta time to maintain consistent speed
		}

		// Draw ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (size_t i = 0; i < materialCount; i++)
		{
			Lepus3D::Material& material = *sponzaMaterials[i];
			ImGui::Text("Material %d: %s\n\tSpecularStrength: %d\n\tSpecularShininess: %d\n\tDiffColor: %s\n\t", 
				i, 
				material.GetName(), 
				material.GetAttributeF("_SpecularStrength"),
				material.GetAttributeI("_SpecularShininess"),
				ToString(material.GetAttributeVec3("_DiffColor"), 3).c_str()
			);
		}

		ImGui::Render();


		engine.StartScene(&cam); // Set current camera, prepare engine for drawing
		engine.DrawScene(scene); // Draw objects in scene

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		engine.EndScene(); // Finish rendering and present in window/screen
		isRunning = engine.Update(); // Update window and check if engine is still running

		// Allow frame dumps (screenshots) on F9 keypress
		// Default filepath is "output.bmp"
		if (glfwGetKey(engine.GetWindowPtr(), GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
		{
			engine.DumpToFile();
		}

		if (glfwGetKey(engine.GetWindowPtr(), GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			physicsActive = !physicsActive;
		}

		if (glfwGetKey(engine.GetWindowPtr(), GLFW_KEY_TAB) == GLFW_PRESS)
		{
			cam.LockInput = !cam.LockInput;
			cam.UpdateCursorLock();
		}
	}
	// Output shutdown message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo shutting down!");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Close the rendering context(s), release resources
	scene.Shutdown();
	engine.Shutdown();

	size_t nbMaterials = sponzaMaterials.size();
	for (size_t i = 0; i < nbMaterials; i++)
	{
		delete sponzaMaterials[i];
	}

	return 0;
}