#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <LSystem/Windowing/GLFW.h>
#include <L3D/GraphicsEngine/Apis/ApiGL.h>
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

	std::shared_ptr<LepusEngine::LepusSystem::WindowingGLFW> windowing = std::make_shared<LepusSystem::WindowingGLFW>(800, 600);

	// Create new graphics engine instance
	Lepus3D::GraphicsApiGLOptions options = {};
	options.mainViewport = { 800, 600 };
	Lepus3D::GraphicsEngine engine(&options, windowing);

	// Test camera.
	//Lepus3D::Camera camera;

	// Termination condition for main loop
	bool isRunning = true;
	
	// Set the window as the current OpenGL context.
	windowing->SetAsCurrentContext();

	// Output start message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo starting!");
	while (isRunning)
	{
		windowing->Update(); // Update window before drawing
		engine.Render<unsigned char, Lepus3D::GraphicsEngine::PixelFormat::RGBA32>(100, 149, 237);

		glfwSwapBuffers((GLFWwindow*)windowing->GetWindowPtr());

		//engine.StartScene(&camera);
		//engine.EndScene();

		isRunning = windowing->Update();
	}
	// Output shutdown message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo shutting down!");

	windowing->Shutdown();

	glfwTerminate();

	return 0;
}