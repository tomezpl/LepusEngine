#include "../../Lepus3D/Source/RenderEngine.h"

using namespace LepusEngine;
using namespace Lepus3D;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	VertexArray verts = 
	{
				// Positions			// Colours		 // Texture 
														 // coords
		Vertex(0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f), // Top-right corner
		Vertex(0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f), // Bottom-right corner
		Vertex(-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f), // Top-left corner
		Vertex(-0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f)  // Bottom-left corner
	};

	Lepus3D::Material testMat("Test material", "PerVertexUnlit");
	//Lepus3D::Mesh testMesh(verts, true);
	Lepus3D::BoxMesh testMesh;
	//std::vector<unsigned int> indices = { 2, 0, 3, 0, 1, 3 };
	//testMesh.SetIndices(indices);

	// Be sure to get your own images, these are not provided with the Git repository
	Lepus3D::Texture2D firstTx("container.jpg"); // Loads from Solution/Content/
	testMat.SetAttributeTex("_Texture1", firstTx);

	Lepus3D::Transform transform;

	sf::Clock timer;

	bool running = true;
	while (running)
	{
		float timeSeconds = timer.getElapsedTime().asSeconds();
		testMat.SetAttributeF("_Time", timeSeconds);
		engine.DrawMesh(testMesh, testMat, transform);
		running = engine.Update();
	}
	engine.Shutdown();
	return 0;
}