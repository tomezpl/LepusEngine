# LepusEngine - CMake config
## NOTES:
### Wherever file(GLOB ...) is used to find a library/header,
### the folder structure for that library is based on Windows binaries available from the library's official source.
### TODO: It is assumed that CMake on Linux will just be able to pick the libraries up from system PATH, but that's yet to be tested.
### This build config assumes the use of Microsoft's vcpkg tool. It helps with managing libraries for cross-platform builds.
### Make sure that you have VCPKG_ROOT environment variable set as the path to your vcpkg root directory.
### Be sure to define your system's triplet to ensure correct target architecture (32/64-bit). https://vcpkg.readthedocs.io/en/latest/users/triplets/#additional-remarks

# min. required CMake version
cmake_minimum_required (VERSION 2.6)

# VCPKG directory
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# VCPKG triplet to use (architecture, e.g. x86-windows)
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 17)

# Ensure correct library bitness if running Windows
if(${CMAKE_HOST_WIN32})
	if(ENV{VCPKG_DEFAULT_TRIPLET} STREQUAL "x64-windows")
		set(CMAKE_SIZEOF_VOID_P 8)
	endif()
endif()

project(Lepus)

# Directory for dependencies (usually ./build/deps)
set(DEPS_PATH ${CMAKE_BINARY_DIR}/deps)

# Directory for compiled libraries obtained via VCPKG
set(VCPKG_DEPS_PATH "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
set(VCPKG_INCLUDE_PATH ${VCPKG_DEPS_PATH}/include)
set(VCPKG_LIBS_DBG_PATH ${VCPKG_DEPS_PATH}/debug/lib)
set(VCPKG_LIBS_REL_PATH ${VCPKG_DEPS_PATH}/lib)
set(VCPGK_DLL_DBG_PATH ${VCPKG_DEPS_PATH}/debug/bin)
set(VCPGK_DLL_REL_PATH ${VCPKG_DEPS_PATH}/bin)

# Directory for produced binaries
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Engine-wide include directories
include_directories(${CMAKE_SOURCE_DIR}/include src ${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${VCPKG_INCLUDE_PATH})

# DearImgui
file(GLOB IMGUI_SRC ${CMAKE_SOURCE_DIR}/3rdparty/imgui/*.cpp ${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.cpp ${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.cpp)

add_library(DearImgui ${IMGUI_SRC})
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui)

# Demo executable
add_executable(LepusDemo src/LDemo/main.cpp)

# Lepus3D sources
set(Lepus3D_SRC
	src/L3D/Assets.h
		src/L3D/Assets/ModelImporter.h src/L3D/Assets/ModelImporter.cpp
	src/L3D/Camera.h
		src/L3D/Camera/FPPCamera.h src/L3D/Camera/Camera.cpp src/L3D/Camera/FPPCamera.cpp
	src/L3D/Color.h
		src/L3D/Color/Color.cpp
	src/L3D/Light.h
		src/L3D/Light/Light.cpp
	src/L3D/Material.h
		src/L3D/Material/MaterialAttrib.h src/L3D/Material/Material.cpp
	src/L3D/Mesh.h
		src/L3D/Mesh/Mesh.cpp
	src/L3D/Renderable.h
		src/L3D/Renderable/Renderable.cpp
	src/L3D/RenderEngine.h
		src/L3D/RenderEngine/RenderEngine.cpp
	src/L3D/Scene.h
		src/L3D/Scene/Scene.cpp
	src/L3D/Shader.h
		src/L3D/Shader/SLProgram.h src/L3D/Shader/Shader.cpp src/L3D/Shader/SLProgram.cpp
	src/L3D/Texture.h
		src/L3D/Texture/Texture2D.cpp
	src/L3D/Transform.h
		src/L3D/Transform/Transform.cpp src/L3D/Transform/Vector.h
	src/L3D/Quaternion.h
		src/L3D/Quaternion/Quaternion.cpp
	src/L3D/Vertex.h
)

add_library(Lepus3D ${Lepus3D_SRC})

add_library(LepusEngine
	src/LEngine/Logger.h
		src/LEngine/Logger/Logger.cpp
	src/LEngine/Physics.h
		src/LEngine/Physics/Physics.cpp
		src/LEngine/Physics/PhysicsRigidbody.h
		src/LEngine/Physics/PhysicsRigidbody.cpp
	src/LEngine/Entity.h
		src/LEngine/Entity/Entity.cpp
	src/LEngine/World.h
		src/LEngine/World.cpp
)

add_dependencies(Lepus3D LepusEngine)
add_dependencies(LepusDemo Lepus3D)

# Dependency libraries
## Lepus3D: OpenGL libraries
find_library(OpenGL REQUIRED)
## Lepus3D: GLEW libraries
find_package(GLEW REQUIRED)
target_link_libraries(Lepus3D PRIVATE GLEW::GLEW)
## Lepus3D: GLFW libraries
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Lepus3D PRIVATE glfw)

## Lepus3D: GLEW includes
include_directories(Lepus3D ${GLEW_INCLUDES})

## Lepus3D: GLFW includes
include_directories(Lepus3D ${GLFW_INCLUDES})

# LepusEngine sources
## LepusEngine: PhysX includes
set(PHYSX_INCLUDES ${VCPKG_DEPS_PATH}/include/physx)
include_directories(${PHYSX_INCLUDES})

## LepusEngine: Bullet3 libraries
find_library(BULLET_COMMON_LIB Bullet3Common ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BULLET_COLLISION_LIB BulletCollision ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BULLET_DYNAMICS_LIB BulletDynamics ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BULLET_LINMATH_LIB LinearMath ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)

find_library(BULLET_COMMON_LIB_DBG Bullet3Common_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BULLET_COLLISION_LIB_DBG BulletCollision_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BULLET_DYNAMICS_LIB_DBG BulletDynamics_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BULLET_LINMATH_LIB_DBG LinearMath_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)

# Link Bullet for debug and release targets
target_link_libraries(LepusEngine debug ${BULLET_COMMON_LIB_DBG} optimized ${BULLET_COMMON_LIB})
target_link_libraries(LepusEngine debug ${BULLET_COLLISION_LIB_DBG} optimized ${BULLET_COLLISION_LIB})
target_link_libraries(LepusEngine debug ${BULLET_DYNAMICS_LIB_DBG} optimized ${BULLET_DYNAMICS_LIB})
target_link_libraries(LepusEngine debug ${BULLET_LINMATH_LIB_DBG} optimized ${BULLET_LINMATH_LIB})

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Release)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Debug)

# Set working directory for debugger
set_property(TARGET LepusDemo PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/Debug")

if(${CMAKE_HOST_WIN32})
	target_link_libraries(Lepus3D PRIVATE opengl32.lib)
else()
	find_library(OpenGL REQUIRED)
endif()

target_link_libraries(Lepus3D ${OPENGL_LIBRARIES})
target_link_libraries(Lepus3D PRIVATE DearImgui)
target_link_libraries(Lepus3D PRIVATE LepusEngine)
target_link_libraries(LepusDemo PRIVATE DearImgui Lepus3D LepusEngine)

# Copy content (models, GLSL, etc.)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Content)
add_custom_command(TARGET LepusDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Content" ${CMAKE_BINARY_DIR}/Content)