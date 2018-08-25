#include "../../Lepus3D/Source/RenderEngine.h"
#include "../../Lepus3D/Source/Camera/FPPCamera.h"
#include "../../Lepus3D/Source/Assets.h"

using namespace LepusEngine;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	bool isRunning = true;

	Lepus3D::Assets::ModelImporter modelImp("Models/ball.obj");
	modelImp.Read();
	modelImp.Close();

	Lepus3D::Scene scene;
	Lepus3D::Material testMat("Material", "Phong");
	Lepus3D::Renderable* box = new Lepus3D::Renderable(modelImp.GetSubMesh());
	Lepus3D::Light sceneLight(Lepus3D::Vector3(0.0f, 2.50f, 0.0f), Lepus3D::Color(255, 255, 255, 255), 1.0f);
	testMat.SetAttributeF("_SpecularStrength", 0.5f);
	testMat.SetAttributeI("_SpecularShininess", 256);
	testMat.SetAttributeF3("_DiffColor", Lepus3D::Color(100, 149, 237, 255).GetVector3());
	box->GetMesh()->SetMaterial(testMat);
	box->SetScale(0.25f);

	Lepus3D::FPPCamera cam(*(new Lepus3D::Transform())); // init a transformable fpp camera
	cam.SetWindow(engine.GetWindowPtr()); // bind camera to window to receive input

	scene.AddLight(&sceneLight);
	scene.AddMesh(box);

	sf::Clock timer;
	float dTime; // delta time between frames
	float elapsedTime = 0.0f;

	while (isRunning)
	{
		elapsedTime += dTime = timer.restart().asSeconds(); // running time is needed for the scene light to orbit

		sceneLight.SetPosition(Lepus3D::Vector3(0.0f, 2.50f * sin(elapsedTime), 2.50f * cos(elapsedTime))); // orbit the light around the box

		engine.Update(); // update window before drawing
		cam.ProcessInput(dTime); // move camera according to input
		engine.StartScene(&cam); // set current camera, prepare engine for drawing
		engine.DrawScene(scene); // draw objects in scene
		engine.EndScene(); // finish rendering and present
		isRunning = engine.Update(); // update window and check if engine is still running
	}

	engine.Shutdown();

	return 0;
}
