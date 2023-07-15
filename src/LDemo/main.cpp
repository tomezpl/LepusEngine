#ifdef _DEBUG
#define LEPUS_ALLOW_STDOUT
#endif

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <LSystem/Windowing/GLFW.h>
#include <L3D/GraphicsEngine/Apis/ApiGL.h>
#include <L3D/GraphicsEngine.h>
#include <L3D/GraphicsEngine/ShaderCompilers/ShaderCompilerGLSL.h>

#include <LEngine/ConsoleLogger.h>
#include <LEngine/Physics.h>
#include <LEngine/Physics/PhysicsRigidbody.h>

#include <LSystem/IO.h>

#include <thread>

using namespace LepusEngine;

#ifndef _DEBUG
#define NDEBUG
#endif

int main()
{
	// Enable logging
	LepusEngine::ConsoleLogger.Enabled = true;

	std::shared_ptr<LepusEngine::LepusSystem::WindowingGLFW> windowing = std::make_shared<LepusSystem::WindowingGLFW>(800, 600);

	// Create new graphics engine instance
	Lepus3D::GraphicsApiGLOptions options = {};
	options.mainViewport = { 800, 600 };
	Lepus3D::GraphicsEngine engine(&options, windowing);

	// Termination condition for main loop
	bool isRunning = true;

	// Set the window as the current OpenGL context.
	windowing->SetAsCurrentContext();

	// Output start message to console
	LepusEngine::ConsoleLogger.LogInfo("", "main", "Demo starting!");

	// Load & compile shaders.
	std::string vertShaderSrc = LepusSystem::FileSystem::Read("../../Content/GLSL/Unlit/RGBVertex.vert"), fragShaderSrc = LepusSystem::FileSystem::Read("../../Content/GLSL/Unlit/RGBVertex.frag");
	Lepus3D::ShaderCompiledResult
		vertShader = Lepus3D::ShaderCompilerGLSL::Singleton().CompileShader(vertShaderSrc.c_str(), vertShaderSrc.length(), Lepus3D::VertexShader),
		fragShader = Lepus3D::ShaderCompilerGLSL::Singleton().CompileShader(fragShaderSrc.c_str(), fragShaderSrc.length(), Lepus3D::FragmentShader);

	// Register shader with the API.
	engine.GetApi<Lepus3D::GraphicsApiGL>().GetOptions<Lepus3D::GraphicsApiGLOptions>().RegisterShader(&vertShader, &fragShader);

	// Set up engine for drawing.
	engine.Setup();

	while (isRunning)
	{
		windowing->Update(); // Update window before drawing
		engine.Render<unsigned char, Lepus3D::GraphicsEngine::PixelFormat::RGBA32>(100, 149, 237);

		isRunning = windowing->Update();
	}

	// Output shutdown message to console
	LepusEngine::ConsoleLogger.LogInfo("", "main", "Demo shutting down!");

	windowing->Shutdown();

	LepusEngine::LepusSystem::WindowingGLFW::Terminate();

	return 0;
}