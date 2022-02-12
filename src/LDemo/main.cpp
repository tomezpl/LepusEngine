#include <L3D/RenderEngine.h>
#include <L3D/Camera/FPPCamera.h>
#include <L3D/Assets.h>
#include <LEngine/Logger.h>
#include <LEngine/Physics.h>
#include <LEngine/Physics/PhysicsRigidbody.h>

using namespace LepusEngine;

#ifndef _DEBUG
	#define NDEBUG
#endif

int main()
{
	// Prepare Bullet3
	//APIBullet apiBullet;

	// Enable logging
	LepusEngine::Logger::Enabled = true;

	// Create new graphics engine instance
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	// Termination condition for main loop
	bool isRunning = true;

	// Initialise the scene
	Lepus3D::Scene scene;

	// Create physics engine for this scene
	LepusEngine::Physics physicsEngine = LepusEngine::Physics();

	// Prepare the shading
	Lepus3D::Material testMat("Material", "Phong"); // Use the phong shader, assign material name "Material"
	Lepus3D::Texture2D testTex("Textures/cube_test.jpg"); // Load texture from file
	// Pass attributes to shader
	testMat.SetAttributeF("_SpecularStrength", 0.5f);
	testMat.SetAttributeI("_SpecularShininess", 256);
	testMat.SetAttributeF3("_DiffColor", Lepus3D::Color(100, 149, 237, 255).GetVector3());
	testMat.SetAttributeTex("_Texture1", testTex);

	// A box that's going to fall down from (0, 0, 0).
	// It will be 1/4 of the original size.
	LepusEngine::Entity box = LepusEngine::Entity(new Lepus3D::Renderable(Lepus3D::BoxMeshUnindexed()), new PhysicsRigidbody(physicsEngine, Lepus3D::BoxMeshUnindexed(), Lepus3D::Transform()));
	box.SetScale(0.25f);

	// This will be our static box placed below the first box to test collisions and dynamics
	LepusEngine::Entity box2 = LepusEngine::Entity(new Lepus3D::Renderable(Lepus3D::BoxMeshUnindexed()), new PhysicsRigidbody(physicsEngine, Lepus3D::BoxMeshUnindexed(), Lepus3D::Transform(Lepus3D::Vector3(0.3f, -15.f, 0.f), Lepus3D::Vector3::Zero(), Lepus3D::Vector3(3.f/2.f, 3.f/2.f, 3.f/2.f)), 0.f));

	// Prepare the lighting
	// A Light is created at xyz(0, 2.5, 0) with a white RGBA colour and intensity 1.0
	// TODO: The light colour doesn't need Alpha, that one channel could go away
	Lepus3D::Light sceneLight(Lepus3D::Vector3(0.0f, 2.50f, 0.0f), Lepus3D::Color(255, 255, 255, 255), 1.0f);

	// Assign materials to meshes
	box.GetRenderable()->GetMesh()->SetMaterial(testMat);
	box2.GetRenderable()->GetMesh()->SetMaterial(testMat);

	// Initialise a transformable FPPCamera (reacts to keyboard & mouse input)
	Lepus3D::FPPCamera cam(*(new Lepus3D::Transform()));
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
		cam.ProcessInput(dTime); // Move camera according to input using delta time to maintain consistent speed
		engine.StartScene(&cam); // Set current camera, prepare engine for drawing
		engine.DrawScene(scene); // Draw objects in scene
		engine.EndScene(); // Finish rendering and present in window/screen
		isRunning = engine.Update(); // Update window and check if engine is still running

		// Allow frame dumps (screenshots) on F12 keypress
		// Default filepath is "output.bmp"
		if (glfwGetKey(engine.GetWindowPtr(), GLFW_KEY_F12) == GLFW_PRESS)
		{
			engine.DumpToFile();
		}

		if (glfwGetKey(engine.GetWindowPtr(), GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			physicsActive = !physicsActive;
		}
	}
	// Output shutdown message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo shutting down!");

	// Close the rendering context(s), release resources
	engine.Shutdown();

	return 0;
}