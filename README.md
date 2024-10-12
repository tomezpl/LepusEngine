# LepusEngine
## OpenGL-based game engine
### About
LepusEngine is a game engine developed in C++ and modern OpenGL (versions >= 3.3).

### Building
The engine uses CMake (>= 3.26) to generate build files. [Download and install it](https://cmake.org/download/) if you don't have it already.

Dependencies for the project are resolved and installed using [vcpkg](https://vcpkg.io/). See [Dependencies](#dependencies) for more info.

General use of CMake comes down to running `cmake configure` and `cmake generate` (please look up [KitWare's documentation](https://cmake.org/documentation/) to learn more).

Some IDEs such as Visual Studio, Visual Studio Code, and CLion come with CMake support or offer "CMake Tools" extensions that allow you to generate the build files from your IDE.
On Windows, you can also use CMake-gui.

The project is primarily built with Visual C++ 2022 during development and [GCC 11.4.0 on GitHub Actions](https://github.com/tomezpl/LepusEngine/actions),
and as such those are the "officially supported" compilers at this time.

#### Dependencies
This project uses [vcpkg](https://vcpkg.io/) to locate and install dependencies, such as code libraries, while keeping the need for platform-specific configuration down to a minimum (or none). 

vcpkg integrates with CMake via a CMake toolchain file. Provided you have a `VCPKG_ROOT` environment variable pointing at your **bootstrapped vcpkg** path,
the CMakeLists script for LepusEngine should automatically pick up the toolchain file and install the dependencies as needed.
See [Microsoft's vcpkg documentation](https://learn.microsoft.com/en-gb/vcpkg/get_started/get-started?pivots=shell-powershell) for more details.

Additionally, some open-source cross-platform dependencies, such as gl3w and imgui are included in the `3rdparty` directory.
For imgui, you'll need to run `git submodule update` in order to fetch code from the imgui repository before building LepusEngine.

### Usage
See the ["demo" example program](https://github.com/tomezpl/LepusEngine/tree/master/src/examples/demo) for an example of how to use the engine's API.


### Licensing
LepusEngine is licensed under the MIT license. This means you are free to use LepusEngine in any way you want, but you must include the copyright notice and the permission notice.

Read the [LICENSE](https://github.com/tomezpl/LepusEngine/blob/master/LICENSE) file for more details.
