#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <L3D/GraphicsEngine.h>
#include <LEngine/Logger.h>
#include <LEngine/Physics.h>
#include <LEngine/Physics/PhysicsRigidbody.h>

#include <assimp/cimport.h>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <thread>

using namespace LepusEngine;

#ifndef _DEBUG
	#define NDEBUG
#endif

int main()
{

	// Enable logging
	LepusEngine::Logger::Enabled = true;

	// Create new graphics engine instance
	Lepus3D::GraphicsEngine engine("LepusDemo", 800, 600);

	// Test camera.
	Lepus3D::Camera camera;

	// Termination condition for main loop
	bool isRunning = true;

	// Output start message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo starting!");
	while (isRunning)
	{
		engine.Update(); // Update window before drawing

		engine.StartScene(&camera);
		engine.EndScene();

		isRunning = engine.Update();
	}
	// Output shutdown message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo shutting down!");

	engine.Shutdown();

	return 0;
}