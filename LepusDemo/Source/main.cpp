#include "../../Lepus3D/Source/RenderEngine.h"
#include "../../Lepus3D/Source/Camera/FPPCamera.h"

using namespace LepusEngine;
using namespace Lepus3D;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	bool isRunning = true;

	Lepus3D::Scene scene;
	Lepus3D::Transform camTransform;
	Lepus3D::FPPCamera cam(camTransform);
	cam.SetWindow(engine.GetWindowPtr());

	sf::Clock timer;
	float dTime;

	Lepus3D::BoxMeshUnindexed mesh;

	scene.AddMesh(mesh);

	while (isRunning)
	{
		dTime = timer.restart().asSeconds();
		engine.Update();
		cam.ProcessInput(dTime);
		engine.StartScene(&cam);
		engine.DrawScene(scene);
		engine.EndScene();
		isRunning = engine.Update();
	}
	
	engine.Shutdown();

	return 0;
}
