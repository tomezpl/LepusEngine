# LepusEngine
## OpenGL-based game engine
### About
LepusEngine is a game engine developed in C++ and modern OpenGL (versions >= 3.3).

The renderer uses a programmable pipeline and as a result comes with a bundled set of GLSL shaders.

### Building
The engine now uses Branimir Karadžić's [GENie project generator tool](https://github.com/bkaradzic/GENie).

For the most part, using GENie is limited to running this command

```
genie <build-system>
```

_For example_, if you want to build with __Visual Studio 2017__, you run

```
genie vs2017
```

or, if you want to build with GNU Make using a Makefile, you run

```
genie gmake
```

Check out the GENie scripting reference if you need, it's quite compact and shouldn't take you longer than a few minutes to understand.

#### Windows
The GENie script on Windows is optimised specifically for building with Visual Studio. VS2013, 2015 and 2017 should work, and so should any newer releases.

You will need to download the third-party libraries used (SFML and GLEW). In addition, you'll need to set up the following environment variables in your system settings:
* SFML_DIR
	* Points to the root directory of a 64-bit SFML package for MSVC++ (folder name usually *SFML-x.x.x-windows-vc1x-64-bit*).
	* For example, *$(SFML_DIR)/lib/* should point to the *lib* folder in your 64-bit SFML directory
* SFML_DIR32
	* Points to the root directory of a 32-bit SFML package for MSVC++ (folder name usually *SFML-x.x.x-windows-vc1x-32-bit*).
	* For example, *$(SFML_DIR32)/lib/* should point to the *lib* folder in your 32-bit SFML directory
* GLEW_DIR
	* Points to the root directory of a GLEW package for Win32 (folder name usually *glew-x.x.x-win32*).
	* For example, *$(GLEW_DIR)/lib/* should point to the *lib* folder in your GLEW directory

#### GNU/Linux
You'll need to install G++, pkg-config, g++-multilib, GNU make, as well as dependencies: *libgl1-mesa-dev*, *libsfml-dev* and *libglew-dev*.

On Ubuntu, you can run:

```bash
sudo apt-get install g++ make libgl1-mesa-dev libsfml-dev libglew-dev
```

The GENie script on Linux was written with GNU Make in mind, but it is not tested or maintained as regularly as the Windows build tools, so please report any issues and try fixing them where possible. Run ```make LepusDemo``` to build the full engine along with the demo app. You can also append build type and platform to that command, like ```make LepusDemo config='debug64'```. Advised if you're running into "skipping library because incompatible" issues with GCC.

### Usage
The __RenderEngine__ class (from the _LepusEngine_::_Lepus3D_ namespace) is responsible for drawing pretty much anything to your screen.

__RenderEngine__ also creates and updates a GL context and window.

An example app using the engine would look like this:

```c++
#include "../../Lepus3D/Source/RenderEngine.h"

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
