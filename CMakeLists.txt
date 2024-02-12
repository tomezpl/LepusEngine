# LepusEngine - CMake config
## NOTES:
### Wherever file(GLOB ...) is used to find a library/header,
### the folder structure for that library is based on Windows binaries available from the library's official source.
### TODO: It is assumed that CMake on Linux will just be able to pick the libraries up from system PATH, but that's yet to be tested.
### This build config assumes the use of Microsoft's vcpkg tool. It helps with managing libraries for cross-platform builds.
### Make sure that you have VCPKG_ROOT environment variable set as the path to your vcpkg root directory.
### Be sure to define your system's triplet to ensure correct target architecture (32/64-bit). https://vcpkg.readthedocs.io/en/latest/users/triplets/#additional-remarks

# min. required CMake version
cmake_minimum_required (VERSION 3.26)
include(FindOpenGL)
include(CMakePrintHelpers)

set(VCPKG_FEATURE_FLAGS "manifests,registries,versions")

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
set(VCPKG_INCLUDE_PATH ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/)
set(VCPKG_LIB_PATH ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/)
cmake_print_variables(VCPKG_INCLUDE_PATH)

# Directory for produced binaries
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Engine source root
set(LEPUS_SRC_DIR ${CMAKE_SOURCE_DIR}/src/lepus)

# Engine-wide include directories
include_directories(${CMAKE_SOURCE_DIR}/include src ${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${VCPKG_INCLUDE_PATH})
include_directories(${VCPKG_INCLUDE_PATH}/bullet)

link_directories(${VCPKG_LIB_PATH})

# DearImgui
file(GLOB IMGUI_SRC ${CMAKE_SOURCE_DIR}/3rdparty/imgui/*.cpp ${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.cpp ${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.cpp)

add_library(DearImgui ${IMGUI_SRC})
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui)

# Demo executable
add_executable(LepusDemo src/examples/demo/DemoApp.h src/examples/demo/main.cpp)

# LepusUtility
# file(GLOB_RECURSE LEPUSUTILITY_HEADERS ${LEPUS_SRC_DIR}/utility/*.h ${LEPUS_SRC_DIR}/utility/**/*.h)
# add_custom_target(LepusUtility SOURCES ${LEPUSUTILITY_HEADERS})
# source_group(TREE ${LEPUS_SRC_DIR}/utility FILES ${LEPUSUTILITY_HEADERS})

# LepusGfx shaders
file(GLOB LEPUS3D_SHADERS_SRC ${CMAKE_SOURCE_DIR}/Content/GLSL/*.frag ${CMAKE_SOURCE_DIR}/Content/GLSL/*.vert)
add_custom_target(LepusGfx_Shaders SOURCES ${LEPUS3D_SHADERS_SRC})

# LepusGfx sources
file(GLOB_RECURSE LepusGfx_SRC ${LEPUS_SRC_DIR}/gfx/*.h ${LEPUS_SRC_DIR}/gfx/*.cpp ${LEPUS_SRC_DIR}/gfx/**/*.h ${LEPUS_SRC_DIR}/gfx/**/*.cpp)
list(FILTER LepusGfx_SRC EXCLUDE REGEX ".*(3rdparty)+.*")

add_library(LepusGfx ${LepusGfx_SRC})
source_group(TREE ${LEPUS_SRC_DIR}/gfx FILES ${LepusGfx_SRC})

# LepusEngine sources
file(GLOB_RECURSE LepusEngine_SRC ${LEPUS_SRC_DIR}/engine/*.h ${LEPUS_SRC_DIR}/engine/*.cpp ${LEPUS_SRC_DIR}/engine/**/*.h ${LEPUS_SRC_DIR}/engine/**/*.cpp)
add_library(LepusEngine ${LepusEngine_SRC})
source_group(TREE ${LEPUS_SRC_DIR}/engine FILES ${LepusEngine_SRC})

# LepusUtility sources
file(GLOB_RECURSE LepusUtility_SRC ${LEPUS_SRC_DIR}/utility/*.h ${LEPUS_SRC_DIR}/utility/*.cpp ${LEPUS_SRC_DIR}/utility/**/*.h ${LEPUS_SRC_DIR}/utility/**/*.cpp)
add_library(LepusUtility ${LepusUtility_SRC})
source_group(TREE ${LEPUS_SRC_DIR}/utility FILES ${LepusUtility_SRC})

# LepusSystem sources
file(GLOB_RECURSE LepusSystem_SRC ${LEPUS_SRC_DIR}/system/*.h ${LEPUS_SRC_DIR}/system/*.cpp ${LEPUS_SRC_DIR}/system/**/*.h ${LEPUS_SRC_DIR}/system/**/*.cpp)
add_library(LepusSystem ${LepusSystem_SRC})
source_group(TREE ${LEPUS_SRC_DIR}/system FILES ${LepusSystem_SRC})

# Unit test example files
file(GLOB_RECURSE LEPUS_TESTS_CONTENT_SRC ${CMAKE_SOURCE_DIR}/tests/Content/*.* ${CMAKE_SOURCE_DIR}/tests/Content/**.*)
add_custom_target(Lepus_Tests_Content SOURCES ${LEPUS_TESTS_CONTENT_SRC})

add_dependencies(LepusDemo LepusGfx)

# Dependency libraries
## LepusGfx: GL3W (core profile loading)
add_library(GL3W
	3rdparty/gl3w/src/gl3w.c
)
include_directories(3rdparty/gl3w/include)
## LepusGfx: GLFW libraries
find_package(glfw3 CONFIG REQUIRED)

## LepusGfx: GLEW includes
include_directories(LepusGfx ${GLEW_INCLUDES})

## LepusGfx: GLFW includes
include_directories(LepusGfx ${GLFW_INCLUDES})

# LepusEngine sources
## LepusEngine: PhysX includes
set(PHYSX_INCLUDES ${VCPKG_DEPS_PATH}/include/physx)
include_directories(${PHYSX_INCLUDES})

## LepusEngine: Bullet3 libraries

#find_library(BULLET_COMMON_LIB Bullet3Common ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
set(BULLET_COMMON_LIB ${Bullet3Common})
#find_library(BULLET_COLLISION_LIB BulletCollision ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
set(BULLET_COLLISION_LIB ${BulletCollision})
#find_library(BULLET_DYNAMICS_LIB BulletDynamics ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
set(BULLET_DYNAMICS_LIB ${BulletDynamics})
#find_library(BULLET_LINMATH_LIB LinearMath ${VCPKG_LIBS_REL_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
set(BULLET_LINMATH_LIB ${LinearMath})

#find_library(BULLET_COMMON_LIB_DBG Bullet3Common_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
#find_library(BULLET_COLLISION_LIB_DBG BulletCollision_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
#find_library(BULLET_DYNAMICS_LIB_DBG BulletDynamics_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
#find_library(BULLET_LINMATH_LIB_DBG LinearMath_Debug ${VCPKG_LIBS_DBG_PATH} NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)

# Link Bullet for debug and release targets
target_link_libraries(LepusEngine PRIVATE LinearMath Bullet3Common BulletDynamics BulletCollision)

#target_link_libraries(LepusEngine debug Bullet3Common_Debug optimized Bullet3Common)
#target_link_libraries(LepusEngine debug BulletDynamics_Debug optimized BulletDynamics)
#target_link_libraries(LepusEngine debug BulletCollision_Debug optimized BulletCollision)

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Release)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Debug)

# Set working directory for debugger
set_property(TARGET LepusDemo PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/Debug")

if(OpenGL::GL) 
	set(GL_LIBRARY OpenGL::GL)
elseif(OpenGL::OpenGL)
	set(GL_LIBRARY OpenGL::OpenGL)
endif(OpenGL::GL)
target_link_libraries(LepusGfx PRIVATE GL3W glfw ${GL_LIBRARY} DearImgui LepusEngine LepusUtility LepusSystem)
target_link_libraries(LepusDemo PRIVATE DearImgui LepusGfx LepusUtility LepusEngine)

# Copy content (models, GLSL, etc.)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Content)
add_custom_command(TARGET LepusDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Content" ${CMAKE_BINARY_DIR}/Content)

# Unit testing framework
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Unit test projects for each library
add_executable(LepusGfx_Tests
	tests/L3D/GraphicsEngine/GraphicsApiTests.h
		tests/L3D/GraphicsEngine/GraphicsApiTests.cpp
	tests/L3D/GraphicsEngine/GraphicsApiOptionsTests.h
		tests/L3D/GraphicsEngine/GraphicsApiOptionsTests.cpp
	tests/L3D/SceneGraph/SceneGraphTests.h
		tests/L3D/SceneGraph/SceneGraphTests.cpp
)

add_executable(LepusSystem_Tests
	tests/LSystem/IO/FileSystemTests.h
		tests/LSystem/IO/FileSystemTests.cpp
)

add_executable(LepusUtility_Tests
	tests/LUtility/MathTests/MatrixTests.cpp
	tests/LUtility/MathTests/VectorTests.cpp
	tests/LUtility/MathTests/TransformTests.cpp
)

add_custom_command(TARGET LepusSystem_Tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/tests/Content" ${CMAKE_BINARY_DIR}/tests/Content)

target_link_libraries(LepusGfx_Tests GTest::gtest_main LepusGfx)
target_link_libraries(LepusSystem_Tests GTest::gtest_main LepusSystem)
target_link_libraries(LepusUtility_Tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(LepusGfx_Tests)
gtest_discover_tests(LepusSystem_Tests)
gtest_discover_tests(LepusUtility_Tests)

# Warnings
set(LepusTargets LepusEngine LepusSystem LepusGfx LepusSystem_Tests LepusGfx_Tests LepusUtility_Tests)
if(MSVC)
	# Ignore warnings:
	# 4100: unreferenced param, 
	# 4514: unreferenced inline function removed,
	# 4464: parent relative include,
	# 4820: struct padding,
	# 4263: member not overriding base class virtual
	# 4265-5027: copy ctor, move ctor, assignment, move assignment implicitly deleted (this messes up gtest projects)
	# 5045: "compiler will insert Spectre mitigation for memory load"
	set(MSVCDisabledWarnings 4100 4514 4464 4820 4263 4625 5026 4626 5027 5045)

	set(MSVCDisabledWarningsFormatted "")
	foreach(Warning IN LISTS MSVCDisabledWarnings)
		set(MSVCDisabledWarningsFormatted ${MSVCDisabledWarningsFormatted} /wd${Warning})
	endforeach()

	foreach(Target IN LISTS LepusTargets)
		target_compile_options(${Target} PRIVATE /Wall ${MSVCDisabledWarningsFormatted} /WX /external:W3)
	endforeach()
else()
	foreach(Target IN LISTS LepusTargets)
		target_compile_options(${Target} PRIVATE -Wall -Wextra -Wpedantic)
	endforeach()
endif()
