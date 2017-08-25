#include "../../Lepus3D/Source/RenderEngine.h"
#include "../../Lepus3D/Source/Camera/FPPCamera.h"

using namespace LepusEngine;
using namespace Lepus3D;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	/*VertexArray verts = 
	{
				// Positions			// Colours		 // Texture 
														 // coords
		Vertex(0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f), // Top-right corner
		Vertex(0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f), // Bottom-right corner
		Vertex(-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f), // Top-left corner
		Vertex(-0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f)  // Bottom-left corner
	};*/

	Lepus3D::Material testMat("Test material", "PerVertexUnlit");
	//Lepus3D::Mesh testMesh(verts, true);
	Lepus3D::BoxMesh testMesh, testMesh2, testMesh3, testMesh4;
	//std::vector<unsigned int> indices = { 2, 0, 3, 0, 1, 3 };
	//testMesh.SetIndices(indices);

	// Be sure to get your own images, these are not provided with the Git repository
	Lepus3D::Texture2D firstTx("container.jpg"); // Loads from Solution/Content/
	testMat.SetAttributeTex("_Texture1", firstTx);

	Lepus3D::Transform transform, transform2, transform3, transform4, camTransform;
	Lepus3D::FPPCamera camera(camTransform);
	camera.SetWindow(engine.GetWindowPtr());

	camTransform.SetPosition(Vector3(0.f, 0.f, -2.0f));
	camera.SetTransform(camTransform);

	sf::Clock timer;

	bool running = true;
	float deltaTime = 0.0f;
	sf::Clock deltaTimer;

	while (running)
	{
		// Radius of the circle for the camera to rotate around
		const float circleRadius = 3.f;

		float timeSeconds = timer.getElapsedTime().asSeconds();
		deltaTime = deltaTimer.restart().asSeconds();

		testMat.SetAttributeF("_Time", timeSeconds);
		engine.Update(); // Update window events etc.

		camera.ProcessInput(deltaTime);

		engine.StartScene(&camera); // Start drawing (clear buffers etc.)

		transform.SetPosition(Vector3(-1.f, 0.f, 0.f));
		engine.DrawMesh(testMesh, testMat, transform);

		transform2.SetPosition(Vector3(1.f, 0.f, 0.f));
		engine.DrawMesh(testMesh2, testMat, transform2);

		transform3.SetPosition(Vector3(0.f, -1.f, 0.f));
		engine.DrawMesh(testMesh3, testMat, transform3);

		transform4.SetPosition(Vector3(0.f, 1.f, 0.f));
		engine.DrawMesh(testMesh4, testMat, transform4);

		engine.EndScene(); // Finish drawing (display in window)
		running = engine.Update();
	}
	engine.Shutdown();
	return 0;
}
