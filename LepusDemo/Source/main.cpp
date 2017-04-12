#include "../../Lepus3D/Source/RenderEngine.h"

using namespace LepusEngine;
using namespace Lepus3D;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	// Some diamond-like shape:
	VertexArray verts = {
		Vertex(0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex(0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f) };

	Lepus3D::Material testMat("Test material", "PerVertexUnlit");
	Lepus3D::Mesh testMesh(verts);
	testMat.SetAttributeF4("_Color", new float[4]{ 0.75f, 0.75f, 0.75f, 1.0f });

	sf::Clock timer;

	bool running = true;
	while (running)
	{
		testMat.SetAttributeF("_Time", timer.getElapsedTime().asSeconds());
		engine.DrawMesh(testMesh, testMat);
		running = engine.Update();
	}
	engine.Shutdown();
	return 0;
}