#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <lepus/system/Windowing/GLFW.h>
#include <lepus/gfx/GraphicsEngine/Apis/ApiGL.h>
#include <lepus/gfx/GraphicsEngine.h>
#include <lepus/gfx/GraphicsEngine/ShaderCompilers/ShaderCompilerGLSL.h>
#include <lepus/gfx/GraphicsEngine/GraphicsApi/Uniforms.h>

#include <lepus/engine/ConsoleLogger.h>

#include <lepus/system/IO.h>

#include <lepus/gfx/Camera.h>

#include <thread>
#include <string>

#include <lepus/system/BaseApp.h>

using namespace lepus;

class DemoApp;

/// @brief Wrapper for binding GLFW input events to DemoApp event handler methods.
namespace DemoAppGLFWCallbacks
{
    DemoApp* _demoApp = 0;

    void cursorPos(GLFWwindow* window, double xpos, double ypos);

    void scroll(GLFWwindow* window, double xoffset, double yoffset);
} // namespace DemoAppGLFWCallbacks

class DemoApp : public system::BaseApp
{
    private:
    float m_FOV;

    const float _camSpeed = 5.f;

    struct
    {
	double lastX, lastY;
    } m_MouseState;

    lepus::gfx::Camera m_Camera;

    virtual void Init(void callback()){};

    struct KeyboardState
    {
	bool w = false, a = false, s = false, d = false, e = false;
    };

    struct
    {
	lepus::math::Matrix4x4 projMatrix, viewMatrix;
    } m_UniformState;

    public:
    DemoApp()
    {
	m_FOV = 0.f;

	m_MouseState =
	    {
	        -1.0, -1.0};

	m_Camera = lepus::gfx::Camera();

	Init();
    }

    inline void Init()
    {
	assert(!m_Initialised);

	// Enable logging
	engine::ConsoleLogger::Global().Enabled = true;

	m_Initialised = true;

	DemoAppGLFWCallbacks::_demoApp = this;
    }

    inline void OnMouseMove(double xpos, double ypos)
    {
	float deltaX = (xpos - m_MouseState.lastX) / 300.0;
	float deltaY = (ypos - m_MouseState.lastY) / 300.0;
	lepus::types::Quaternion rotationYaw = lepus::types::Quaternion(0.f, 1.f, 0.f, -deltaX);

	auto combined = rotationYaw;
	float angle = combined.Angle();
	if (abs(angle) > 0.001f)
	{
	    m_Camera.Transform().Rotate(rotationYaw);
	}
	lepus::types::Quaternion rotationPitch = lepus::types::Quaternion(m_Camera.Transform().Right(), -deltaY);
	angle = rotationPitch.Angle();
	if (abs(angle) > 0.001f)
	{
	    m_Camera.Transform().Rotate(rotationPitch);
	}

	m_MouseState.lastX = xpos;
	m_MouseState.lastY = ypos;
    }

    inline void OnScroll(double xoffset, float yoffset)
    {
	m_FOV = fmax(1.f, fmin(179.f, m_FOV + (float)yoffset));
	engine::ConsoleLogger::Global().LogInfo("", "scrollCallback", "yoffset", std::to_string(xoffset).append(", ").append(std::to_string(yoffset)).append("FOV: ").append(std::to_string(m_FOV)).c_str());
    }

    inline int Run() override
    {
	std::shared_ptr<system::WindowingGLFW> windowing = std::make_shared<system::WindowingGLFW>(800, 600);

	// Create new graphics engine instance
	gfx::GraphicsApiGLOptions options = {};
	options.mainViewport = {800, 600};
	gfx::GraphicsEngine engine(&options, windowing);

	// Termination condition for main loop
	bool isRunning = true;

	// Set the window as the current OpenGL context.
	windowing->SetAsCurrentContext();

	// Output start message to console
	engine::ConsoleLogger::Global().LogInfo("", "main", "Demo starting!");

	// Load & compile shaders.
	std::string vertShaderSrc = system::FileSystem::Read("../../Content/GLSL/Unlit/RGBVertex.vert"), fragShaderSrc = system::FileSystem::Read("../../Content/GLSL/Unlit/RGBVertex.frag");
	gfx::ShaderCompiledResult
	    vertShader = gfx::ShaderCompilerGLSL::Singleton().CompileShader(vertShaderSrc.c_str(), vertShaderSrc.length(), gfx::VertexShader),
	    fragShader = gfx::ShaderCompilerGLSL::Singleton().CompileShader(fragShaderSrc.c_str(), fragShaderSrc.length(), gfx::FragmentShader);

	// Register shader with the API.
	auto& api = engine.GetApi<gfx::GraphicsApiGL>();
	api.GetOptions<gfx::GraphicsApiGLOptions>().RegisterShader(&vertShader, &fragShader);

	// Set up engine for drawing.
	engine.Setup();
	m_Camera.Transform().Origin(m_Camera.Transform().Forward() * -2.f);

	// Instantiate two Renderables in the scene graph, each with its own transform, using the same cube mesh data.
	auto cubeMesh = lepus::gfx::GLMesh(lepus::utility::Primitives::Cube());
	auto cube = lepus::gfx::Renderable<lepus::gfx::GLMesh>(&cubeMesh, lepus::math::Transform());
	auto cube2 = lepus::gfx::Renderable<lepus::gfx::GLMesh>(&cubeMesh, lepus::math::Transform());
	auto cube3 = lepus::gfx::Renderable<lepus::gfx::GLMesh>(&cubeMesh, lepus::math::Transform());
	auto cube4 = lepus::gfx::Renderable<lepus::gfx::GLMesh>(&cubeMesh, lepus::math::Transform());

	cube.GetTransform()->Origin(lepus::types::Vector3(0.f, 0.f, -2.f));
	cube2.GetTransform()->Origin(lepus::types::Vector3(2.f, 0.f, 0.f));
	cube3.GetTransform()->Origin(lepus::types::Vector3(-3.f, 0.f, -3.f));
	cube4.GetTransform()->Origin(lepus::types::Vector3(0.f, 0.f, 6.f));

	auto rootNode = api.GetSceneGraph().AddChild(&cube);
	auto childNode1 = rootNode->AddChild(&cube2);
	auto childNode2 = childNode1->AddChild(&cube3);
	auto childNode3 = childNode2->AddChild(&cube4);

	cube.GetTransform()->Rotate(lepus::types::Quaternion(0.f, 1.f, 0.f, (float)PI * -0.5f));
	cube2.GetTransform()->SetScale(1.f / 1.5f);
	cube3.GetTransform()->Rotate(lepus::types::Quaternion(0.f, 1.f, 0.f, (float)PI * (-50.f / 180.f)));

	auto cubeNode = api.GetSceneGraph().AddChild(&cube);

	// Update projection and view matrices with data from the camera object.
	m_UniformState.projMatrix = m_Camera.BuildPerspectiveMatrix();
	((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>(LEPUS_GFX_UNIFORMS_GLOBAL_PROJECTION_MATRIX))->Value((float*)m_UniformState.projMatrix.data());
	m_UniformState.viewMatrix = m_Camera.BuildViewMatrix();
	((lepus::gfx::GLMatrixUniformBinding*)api.GetUniform<lepus::gfx::GLMatrixUniformBinding>(LEPUS_GFX_UNIFORMS_GLOBAL_VIEW_MATRIX))->Value((float*)m_UniformState.viewMatrix.data());

	// Initialise the FOV variable and set up a callback so we can let the user adjust it with the mouse scroll wheel.
	m_FOV = m_Camera.FOV();
	glfwSetScrollCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), DemoAppGLFWCallbacks::scroll);

	float runningTime = glfwGetTime();

	GLFWwindow* window = reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr());

	// Set up mouse input for camera freelook.
	glfwGetCursorPos(window, &m_MouseState.lastX, &m_MouseState.lastY);
	glfwSetCursorPosCallback(reinterpret_cast<GLFWwindow*>(windowing->GetWindowPtr()), DemoAppGLFWCallbacks::cursorPos);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, 1);

	// Initialise all keys as released.
	KeyboardState keys = {false, false, false, false, false};

	float deltaTime = 0.f;
	auto transform = cube2.GetTransform();
	// transform->Origin(lepus::types::Vector3(0.f, 0.f, -2.f));
	// transform->SetScale(0.5f, 0.25f, 1.f);

	// Parent the second cube to the first cube.
	// cubeNode->AddChild(&cube2);

	while (isRunning)
	{
	    windowing->Update(); // Update window before drawing

	    bool eKeyPressedLastFrame = keys.e;
	    UpdateInput(keys, windowing);

	    // Rotate the parent cube
	    cube.GetTransform()->Rotate(lepus::types::Quaternion(lepus::types::Vector3(0.f, 1.f, 0.f), deltaTime * 0.5f));

	    // Move the child cube back and forth along the parent's Z-axis
	    // cube2.GetTransform()->Origin(lepus::types::Vector3(0.f, 0.f, -1.f + ((sinf(runningTime) + 1.f) * 0.5f) * -2.f));

	    Tick(deltaTime, keys);
	    UpdateUniforms(&api);

	    engine.Render<unsigned char, gfx::GraphicsEngine::PixelFormat::RGBA32>(100, 149, 237);

	    float newRunningTime = glfwGetTime();
	    deltaTime = newRunningTime - runningTime;
	    runningTime = newRunningTime;

	    isRunning = windowing->Update();
	}

	Shutdown(windowing);

	return 0;
    }

    inline void UpdateInput(KeyboardState& keys, std::shared_ptr<system::WindowingGLFW> windowing)
    {
	GLFWwindow* window = (GLFWwindow*)windowing->GetWindowPtr();

	keys.w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	keys.a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	keys.s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	keys.d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	keys.e = glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE;
    }

    inline void UpdateUniforms(gfx::GraphicsApi* api)
    {
	m_UniformState.projMatrix = m_Camera.BuildPerspectiveMatrix();
	m_UniformState.viewMatrix = m_Camera.BuildViewMatrix();

	((lepus::gfx::GLMatrixUniformBinding*)api->GetUniform<lepus::gfx::GLMatrixUniformBinding>(LEPUS_GFX_UNIFORMS_GLOBAL_PROJECTION_MATRIX))->Value((float*)m_UniformState.projMatrix.data());
	((lepus::gfx::GLMatrixUniformBinding*)api->GetUniform<lepus::gfx::GLMatrixUniformBinding>(LEPUS_GFX_UNIFORMS_GLOBAL_VIEW_MATRIX))->Value((float*)m_UniformState.viewMatrix.data());
    }

    inline void Tick(float deltaTime, const KeyboardState& keys)
    {
	m_Camera.FOV(m_FOV);

	lepus::types::Vector3 forwardDelta, rightDelta;

	// Process camera movement input based on keyboard state.
	if (keys.s)
	{
	    forwardDelta = forwardDelta - (m_Camera.Transform().Forward() * deltaTime * _camSpeed);
	}
	if (keys.w)
	{
	    forwardDelta = forwardDelta + (m_Camera.Transform().Forward() * deltaTime * _camSpeed);
	}
	if (keys.d)
	{
	    rightDelta = rightDelta + (m_Camera.Transform().Right() * deltaTime * _camSpeed);
	}
	if (keys.a)
	{
	    rightDelta = rightDelta - (m_Camera.Transform().Right() * deltaTime * _camSpeed);
	}

	// lepus::engine::ConsoleLogger::Global().LogInfo("DemoApp", "Tick", m_Camera.Transform().Rotation().Axis().ToString().c_str());
	// lepus::engine::ConsoleLogger::Global().LogInfo("DemoApp", "Tick", std::to_string(m_Camera.Transform().Rotation().Angle() * (1.f / PI) * 180.f).c_str());

	m_Camera.Transform().Origin(m_Camera.Transform().Origin() + forwardDelta + rightDelta);
    }

    inline void Shutdown(std::shared_ptr<system::WindowingGLFW> windowing)
    {
	// Output shutdown message to console
	engine::ConsoleLogger::Global().LogInfo("", "main", "Demo shutting down!");

	windowing->Shutdown();

	system::WindowingGLFW::Terminate();

	engine::ConsoleLogger::Shutdown();

	m_Initialised = false;
    }
};

inline void DemoAppGLFWCallbacks::cursorPos(GLFWwindow* windowing, double xpos, double ypos)
{
    DemoAppGLFWCallbacks::_demoApp->OnMouseMove(xpos, ypos);
}

inline void DemoAppGLFWCallbacks::scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    DemoAppGLFWCallbacks::_demoApp->OnScroll(xoffset, yoffset);
}