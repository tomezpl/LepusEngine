# LepusEngine
## OpenGL-based game engine
### About
LepusEngine is a game engine developed in C++ and modern OpenGL (versions >= 3.3).

The renderer uses a programmable pipeline and as a result comes with a bundled set of GLSL shaders.

### Building
The engine now uses CMake (tested version 3.15.2). Download and install it if you don't have it already.

General use of CMake comes down to running `cmake configure` and `cmake generate` (please look up KitWare's documentation to learn more).

Currently the engine only builds on Windows during to its reliance on PhysX. On Windows, you can also use CMake-gui.

#### Dependencies
Download binaries for these libraries and place them in `<lepus_root>/build/deps`:

* GLFW (*build/deps/glfw-x.x.x.bin.WIN64*)
	* The root directory of a 64-bit pre-compiled binary GLFW package for MSVC++ (folder name usually *glfw-x.x.x.bin.WIN64*).
	* For example, *build/deps/glfw-x.x.x.bin.WIN64/lib-vc2015/* should contain your 64-bit *glfw3.lib* files
* GLFW32 (*build/deps/glfw-x.x.x.bin.WIN32*)
	* The root directory of a 32-bit pre-compiled binary GLFW package for MSVC++ (folder name usually *glfw-x.x.x.bin.WIN32*).
	* For example, *build/deps/glfw-x.x.x.bin.WIN32/lib-vc2015/* should contain your 32-bit *glfw3.lib* files
* GLEW (*build/deps/glew-x.x.x-win32*)
	* The root directory of a GLEW package for Win32 (folder name usually *glew-x.x.x-win32/glew-x.x.x*).
	* For example, *build/deps/glew-x.x.x-win32/glew-x.x.x/lib/* should contain the *glew.lib* files
* PHYSX (*build/deps/PhysX-x.x/physx/install/vcxxwin64*)
	* The root directory of the *compiled binaries* for PhysX for 64-bit Visual C++ (folder name usually *PhysX-x.x/physx/install/vcxxwin64*).
	* For example, *build/deps/PhysX-x.x/physx/install/vcxxwin64/PhysX/include/* should contain the *PxPhysXConfig.h* header file.
	* You can find info regarding building PhysX SDK from source on [NVIDIA's official PhysX repository](https://github.com/NVIDIAGameWorks/PhysX).
* PHYSX32 (*build/deps/PhysX-x.x/physx/install/vcxxwin32*)
	* The root directory of the *compiled binaries* for PhysX for 32-bit Visual C++ (folder name usually *PhysX-x.x/physx/install/vcxxwin32*).
	* For example, *build/deps/PhysX-x.x/physx/install/vcxxwin32/PhysX/include/* should contain the *PxPhysXConfig.h* header file.
	* You can find info regarding building PhysX SDK from source on [NVIDIA's official PhysX repository](https://github.com/NVIDIAGameWorks/PhysX).

### Usage
The __RenderEngine__ class (from the _LepusEngine_::_Lepus3D_ namespace) is responsible for drawing pretty much anything to your screen.

__RenderEngine__ also creates and updates a GL context and window.

An example app using the engine would look like this:

```c++
#include <L3D/RenderEngine.h>

using namespace LepusEngine;
using namespace Lepus3D;

int main()
{
	Lepus3D::RenderEngine engine("LepusDemo", 800, 600);

	Lepus3D::Material testMat("Test material", "PerVertexUnlit");
	Lepus3D::BoxMesh testMesh;

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
		transform.SetRotation(Vector3(timeSeconds * 25.f, timeSeconds * 50.f, 0.f));
		transform.SetScale(sin(timeSeconds));
		engine.Update(); // Update window events etc.
		engine.StartScene(); // Start drawing (clear buffers etc.)
		engine.DrawMesh(testMesh, testMat, transform);
		engine.EndScene(); // Finish drawing (display in window)
		running = engine.Update();
	}
	engine.Shutdown();
	return 0;
}
```

Check the header files to see how things work. [RenderEngine.h](https://github.com/tomezpl/LepusEngine/blob/master/Lepus3D/Source/RenderEngine.h) is usually a good place to start.

### Contributing
I myself am focused on creating the renderer. However, I plan on making this a full game engine, and any help would be appreciated.

If you have any questions you can contact me at **tomaszzlc@gmail.com**. If there are any pull requests I'll try to merge them into **master** (assuming they work).

### Licensing
LepusEngine is licensed under the MIT license. This means you are free to use LepusEngine in any way you want, but you must include the copyright notice and the permission notice.

Read the [LICENSE](https://github.com/tomezpl/LepusEngine/blob/master/LICENSE) file for more details.
