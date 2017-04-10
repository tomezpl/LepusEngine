#include "../../Lepus3D/Source/RenderEngine.h"

using namespace LepusEngine;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	// Vertex declaration
	GLfloat verts[] = {
		-0.5f, 0.5f, 0.0f, // Top left
		0.5f, 0.5f, 0.0f, // Top right
		-0.5f, -0.5f, 0.0f, // Bottom left
		0.5f, -0.5f, 0.0f }; // Bottom right
	GLuint indices[] = {
		0, 1, 2,
		1, 3, 2 };

	Lepus3D::Material testMat("Test material", "Unlit");
	testMat.SetAttributeF4("_Color", new float[4]{ 0.75f, 0.75f, 0.75f, 1.0f });

	bool running = true;
	while (running)
	{
		engine.DrawVertices(verts, sizeof(verts) / sizeof(GLfloat), indices, sizeof(indices) / sizeof(GLuint), testMat);
		running = engine.Update();
	}
	engine.Shutdown();
	return 0;
}