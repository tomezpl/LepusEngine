#include "../../Lepus3D/Source/RenderEngine.h"
#include "../../Lepus3D/Source/Camera/FPPCamera.h"
#include "../../Lepus3D/Source/Assets.h"
#include "../../LepusEngine/Source/Logger.h"

using namespace LepusEngine;

#define NDEBUG

#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene = NULL;

PxMaterial*				gMaterial = NULL;

PxPvd*                  gPvd = NULL;

PxReal stackZ = 10.0f;

PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}

void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}

void initPhysics(bool interactive)
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	//PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	//gScene->addActor(*groundPlane);

	/*for (PxU32 i = 0; i < 5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	if (!interactive)
		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
	*/
}

void stepPhysics(bool /*interactive*/)
{
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

void cleanupPhysics(bool /*interactive*/)
{
	gScene->release();
	gDispatcher->release();
	gPhysics->release();
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		transport->release();
	}
	gFoundation->release();
}

int main()
{
	// Prepare PhysX
	initPhysics(false);

	// Enable logging
	LepusEngine::Logger::Enabled = true;

	// Create new graphics engine instance
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	// Termination condition for main loop
	bool isRunning = true;

	// Initialise 3D model importer with uv-mapped cube model file
	// This same instance can be reused throughout the app by calling ModelImporter::Init
	Lepus3D::Assets::ModelImporter modelImp("Models/uv_cube.obj");

	// Read model data in and close stream.
	// This will stall the app's execution due to being synchronous.
	modelImp.Read();
	modelImp.Close();

	// Initialise the scene
	Lepus3D::Scene scene;

	// Prepare the shading
	Lepus3D::Material testMat("Material", "Phong"); // Use the phong shader, assign material name "Material"
	Lepus3D::Texture2D testTex("Textures/cube_test.jpg"); // Load texture from file
	// Pass attributes to shader
	testMat.SetAttributeF("_SpecularStrength", 0.5f);
	testMat.SetAttributeI("_SpecularShininess", 256);
	testMat.SetAttributeF3("_DiffColor", Lepus3D::Color(100, 149, 237, 255).GetVector3());
	testMat.SetAttributeTex("_Texture1", testTex);

	// Prepare the geometry
	// A Renderable encapsulates raw model data to be rendered in a Scene.
	// Scale the box Renderable down to 1/4
	Lepus3D::Renderable* box = new Lepus3D::Renderable(modelImp.GetSubMesh());
	box->SetScale(0.25f);

	// Create box in PhysX scene
	Lepus3D::Transform boxTransform = box->GetTransform();
	Lepus3D::Vector3 boxPos = boxTransform.GetPosition();
	Lepus3D::Vector3 boxRot = boxTransform.GetRotation();
	Lepus3D::Vector3 boxScale = boxTransform.GetScale();
	PxRigidDynamic* boxRigidbody = createDynamic(PxTransform(boxPos.x, boxPos.y, boxPos.z), PxBoxGeometry(0.25, 0.25, 0.25));
	PxTransform boxDynamicTransform = boxRigidbody->getGlobalPose();

	// Prepare the lighting
	// A Light is created at xyz(0, 2.5, 0) with a white RGBA colour and intensity 1.0
	// TODO: The light colour doesn't need Alpha, that one channel could go away
	Lepus3D::Light sceneLight(Lepus3D::Vector3(0.0f, 2.50f, 0.0f), Lepus3D::Color(255, 255, 255, 255), 1.0f);

	// Assign material to mesh
	box->GetMesh()->SetMaterial(testMat);

	// Initialise a transformable FPPCamera (reacts to keyboard & mouse input)
	Lepus3D::FPPCamera cam(*(new Lepus3D::Transform()));
	// Bind to window in order to receive keyboard & mouse input
	cam.SetWindow(engine.GetWindowPtr());

	// Add the box renderable and the light to scene
	scene.AddLight(&sceneLight);
	scene.AddMesh(box);

	// dTime: delta time between frames
	// elapsedTime: total running time, needed for the scene light to orbit around the box
	double dTime, elapsedTime = 0.0;

	// This will be toggled after hitting space to start/stop PhysX
	bool physicsActive = false;

	// Output start message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo starting!");
	while (isRunning)
	{
		// Get delta time from graphics engine
		dTime = engine.LastFrameTime();
		// Add delta time to update total running time
		elapsedTime += dTime;

		if (physicsActive)
		{
			// Call PhysX
			gScene->simulate(1.0f / 60.0f);
			gScene->fetchResults(true);

			// Update box position after PhysX simulation
			boxDynamicTransform = boxRigidbody->getGlobalPose();
			box->SetPosition(Lepus3D::Vector3(boxDynamicTransform.p.x, boxDynamicTransform.p.y, boxDynamicTransform.p.z));
			Lepus3D::Vector3 eulerAngles = Lepus3D::Vector3();
			eulerAngles = Lepus3D::Vector3(glm::eulerAngles(glm::quat(boxDynamicTransform.q.w, boxDynamicTransform.q.x, boxDynamicTransform.q.y, boxDynamicTransform.q.z)));
			box->SetRotation(eulerAngles);
		}

		// Orbit the light around the box over the application's running time
		sceneLight.SetPosition(Lepus3D::Vector3(2.50f * sin(elapsedTime), 2.50f * sin(elapsedTime), 2.50f * cos(elapsedTime)));

		engine.Update(); // Update window before drawing
		cam.ProcessInput(dTime); // Move camera according to input using delta time to maintain consistent speed
		engine.StartScene(&cam); // Set current camera, prepare engine for drawing
		engine.DrawScene(scene); // Draw objects in scene
		engine.EndScene(); // Finish rendering and present in window/screen
		isRunning = engine.Update(); // Update window and check if engine is still running

		// Allow frame dumps (screenshots) on F12 keypress
		// Default filepath is "output.bmp"
		if (glfwGetKey(engine.GetWindowPtr(), GLFW_KEY_F12) == GLFW_PRESS)
			engine.DumpToFile();

		if (glfwGetKey(engine.GetWindowPtr(), GLFW_KEY_SPACE) == GLFW_PRESS)
			physicsActive = !physicsActive;
	}
	// Output shutdown message to console
	LepusEngine::Logger::LogInfo("", "main", "Demo shutting down!");

	// Close the rendering context(s), release resources
	engine.Shutdown();

	// Shutdown PhysX
	cleanupPhysics(false);

	return 0;
}

int snippetMain(int, const char*const*)
{
#ifdef RENDER_SNIPPET
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}
