#include "../../Lepus3D/Source/RenderEngine.h"

using namespace LepusEngine;
using namespace Lepus3D;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	// Vertex declaration
	/*Vertex verts[] = {
		Vertex(-0.5f, 0.5f, 0.0f), // Top left
		Vertex(0.5f, 0.5f, 0.0f), // Top right
		Vertex(-0.5f, -0.5f, 0.0f), // Bottom left
		Vertex(0.5f, -0.5f, 0.0f) }; // Bottom right*/

	// Some diamond-like shape:
	Vertex verts[] = {
		Vertex(0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex(0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f) };
	GLuint indices[] = {
		0, 1, 2,
		1, 2, 3 };

	Lepus3D::Material testMat("Test material", "PerVertexUnlit");
	//testMat.SetAttributeF4("_Color", new float[4]{ 0.75f, 0.75f, 0.75f, 1.0f });

	bool running = true;
	while (running)
	{
		engine.DrawVertices(verts, sizeof(verts), indices, sizeof(indices) / sizeof(GLuint), testMat);
		running = engine.Update();
	}
	engine.Shutdown();
	return 0;
}