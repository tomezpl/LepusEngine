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

#include <LSystem/IO.h>

#include <L3D/Camera.h>

#include <thread>
#include <string>

using namespace LepusEngine;

#ifndef _DEBUG
#define NDEBUG
#endif

float fov = 0.f;
float angle = 0.f;
float x = 0.f, y = 0.f, z = 0.f;
const float camSpeed = 0.1f;
double xposLast = -1.0;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov = fmax(1.f, fmin(179.f, fov + (float)yoffset));
	LepusEngine::ConsoleLogger::Global().LogInfo("", "scrollCallback", "yoffset", (char*)std::to_string(xoffset).append(", ").append(std::to_string(yoffset)).append("FOV: ").append(std::to_string(fov)).c_str());
}

void keyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	LepusEngine::ConsoleLogger::Global().LogInfo("", "keyInputCallback", (char*)std::to_string(key).c_str());

	switch (key)
	{
		case 87:
			// W
			z -= camSpeed;
			break;
		case 65:
			// A
			x -= camSpeed;
			break;
		case 83:
			// S
			z += camSpeed;
			break;
		case 68:
			x += camSpeed;
			break;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (xposLast < 0.0)
	{
		xposLast = xpos;
	}

	angle += (xpos - xposLast) / 100.0;
	xposLast = xpos;
}

int main()
{
	// Enable logging
	LepusEngine::ConsoleLogger::Global().Enabled = true;

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
	LepusEngine::ConsoleLogger::Global().LogInfo("", "main", "Demo starting!");

	// Load & compile shaders.
	std::string vertShaderSrc = LepusSystem::FileSystem::Read("../../Content/GLSL/Unlit/RGBVertex.vert"), fragShaderSrc = LepusSystem::FileSystem::Read("../../Content/GLSL/Unlit/RGBVertex.frag");
	Lepus3D::ShaderCompiledResult
		vertShader = Lepus3D::ShaderCompilerGLSL::Singleton().CompileShader(vertShaderSrc.c_str(), vertShaderSrc.length(), Lepus3D::VertexShader),
		fragShader = Lepus3D::ShaderCompilerGLSL::Singleton().CompileShader(fragShaderSrc.c_str(), fragShaderSrc.length(), Lepus3D::FragmentShader);

	// Register shader with the API.
	auto& api = engine.GetApi<Lepus3D::GraphicsApiGL>();
	api.GetOptions<Lepus3D::GraphicsApiGLOptions>().RegisterShader(&vertShader, &fragShader);

	// Set up engine for drawing.
	engine.Setup();

	lepus::gfx::Camera camera = lepus::gfx::Camera();
	lepus::math::Matrix4x4 projMatrix = camera.BuildPerspectiveMatrix();
	((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("PROJ"))->Value((float*)projMatrix.data());

	lepus::math::Matrix4x4 viewMatrix = camera.BuildViewMatrix();
	((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("VIEW"))->Value((float*)viewMatrix.data());

	fov = camera.FOV();

	glfwSetScrollCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), scrollCallback);
	glfwSetKeyCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), keyInputCallback);
	glfwSetCursorPosCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), mouseCallback);

	float runningTime = glfwGetTime();

	camera.Transform().Rotation().y(1.f);
	//camera.Transform().Rotation().z(1.f);
	camera.Transform().Origin().y(-2.f);
	camera.Transform().Origin().z(-4.f);

	while (isRunning)
	{
		camera.FOV(fov);
		projMatrix = camera.BuildPerspectiveMatrix();

		camera.Transform().Origin().x(x);
		camera.Transform().Origin().y(y);
		camera.Transform().Origin().z(z);
		camera.Transform().Rotation().w(angle);
		viewMatrix = camera.BuildViewMatrix();

		((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("PROJ"))->Value((float*)projMatrix.data());
		((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("VIEW"))->Value((float*)viewMatrix.data());
		((lepus::gfx::GLFloatUniformBinding*)api.GetUniform<lepus::gfx::GLFloatUniformBinding>("runningTime"))->Value(runningTime);

		windowing->Update(); // Update window before drawing
		engine.Render<unsigned char, Lepus3D::GraphicsEngine::PixelFormat::RGBA32>(100, 149, 237);

		runningTime = glfwGetTime();

		isRunning = windowing->Update();
	}

	// Output shutdown message to console
	LepusEngine::ConsoleLogger::Global().LogInfo("", "main", "Demo shutting down!");

	windowing->Shutdown();

	LepusEngine::LepusSystem::WindowingGLFW::Terminate();

	LepusEngine::ConsoleLogger::Shutdown();

	return 0;
}