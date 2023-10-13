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
const float camSpeed = 0.005f;
double xposLast = -1.0;
double yposLast = -1.0;
float deltaTime = 0.f;
lepus::gfx::Camera camera;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov = fmax(1.f, fmin(179.f, fov + (float)yoffset));
	LepusEngine::ConsoleLogger::Global().LogInfo("", "scrollCallback", "yoffset", (char*)std::to_string(xoffset).append(", ").append(std::to_string(yoffset)).append("FOV: ").append(std::to_string(fov)).c_str());
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

	if (xposLast < 0.0)
	{
		xposLast = xpos;
	}

	if (yposLast < 0.0)
	{
		yposLast = ypos;
	}

	float deltaX = (xpos - xposLast) / 100.0;
	float deltaY = (ypos - yposLast) / 100.0;
	angle += deltaX * 0.3f;
	camera.Transform().Rotate(lepus::types::Vector3(0.0f, 1.f, 0.f), deltaX * 0.3f);

	LepusEngine::ConsoleLogger::Global().LogInfo("", "mouseCallback", (char*)std::to_string(angle).c_str());
	xposLast = xpos;
	yposLast = ypos;
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

	camera = lepus::gfx::Camera();
	lepus::math::Matrix4x4 projMatrix = camera.BuildPerspectiveMatrix();
	((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("PROJ"))->Value((float*)projMatrix.data());

	lepus::math::Matrix4x4 viewMatrix = camera.BuildViewMatrix();
	((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("VIEW"))->Value((float*)viewMatrix.data());

	fov = camera.FOV();

	glfwSetScrollCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), scrollCallback);
	//glfwSetKeyCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), keyInputCallback);
	glfwSetCursorPosCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), mouseCallback);

	float runningTime = glfwGetTime();

	camera.Transform().Rotation().y(1.f);
	//camera.Transform().Rotation().z(1.f);

	GLFWwindow* window = reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr());

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, 1);

	while (isRunning)
	{
		camera.FOV(fov);
		projMatrix = camera.BuildPerspectiveMatrix();

		int left = glfwGetKey(window, GLFW_KEY_A);
		int right = glfwGetKey(window, GLFW_KEY_D);
		int forward = glfwGetKey(window, GLFW_KEY_W);
		int back = glfwGetKey(window, GLFW_KEY_S);
		lepus::types::Vector3 forwardDelta, rightDelta;
		if (back == GLFW_PRESS)
		{
			forwardDelta = forwardDelta - (camera.Transform().Forward() * deltaTime * camSpeed);
		}
		if (forward == GLFW_PRESS)
		{
			forwardDelta = forwardDelta + (camera.Transform().Forward() * deltaTime * camSpeed);
		}
		if (right == GLFW_PRESS)
		{
			rightDelta = rightDelta + (camera.Transform().Right() * deltaTime * camSpeed);
		}
		if (left == GLFW_PRESS)
		{
			rightDelta = rightDelta - (camera.Transform().Right() * deltaTime * camSpeed);
		}

		camera.Transform().Origin(camera.Transform().Origin() + forwardDelta + rightDelta);


		/*
			  camera.Transform().Origin().x(x);
			  camera.Transform().Origin().y(y);
			  camera.Transform().Origin().z(z);
	  */
		camera.Transform().Rotation().w(angle);
		viewMatrix = camera.BuildViewMatrix();

		((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("PROJ"))->Value((float*)projMatrix.data());
		((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>("VIEW"))->Value((float*)viewMatrix.data());
		((lepus::gfx::GLFloatUniformBinding*)api.GetUniform<lepus::gfx::GLFloatUniformBinding>("runningTime"))->Value(runningTime);

		windowing->Update(); // Update window before drawing
		engine.Render<unsigned char, Lepus3D::GraphicsEngine::PixelFormat::RGBA32>(100, 149, 237);

		float newRunningTime = glfwGetTime();
		deltaTime = newRunningTime - runningTime;

		ConsoleLogger::Global().LogInfo("", "main", (char*)camera.Transform().Forward().ToString().c_str());

		isRunning = windowing->Update();
	}

	// Output shutdown message to console
	LepusEngine::ConsoleLogger::Global().LogInfo("", "main", "Demo shutting down!");

	windowing->Shutdown();

	LepusEngine::LepusSystem::WindowingGLFW::Terminate();

	LepusEngine::ConsoleLogger::Shutdown();

	return 0;
}