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

# LepusUtility
file(GLOB_RECURSE LEPUSUTILITY_HEADERS ${CMAKE_SOURCE_DIR}/src/LUtility/*.h ${CMAKE_SOURCE_DIR}/src/LUtility/**/*.h)
add_custom_target(LepusUtility SOURCES ${LEPUSUTILITY_HEADERS})
source_group(TREE ${CMAKE_SOURCE_DIR}/src/LUtility FILES ${LEPUSUTILITY_HEADERS})

# Lepus3D shaders
file(GLOB LEPUS3D_SHADERS_SRC ${CMAKE_SOURCE_DIR}/Content/GLSL/*.frag ${CMAKE_SOURCE_DIR}/Content/GLSL/*.vert)
add_custom_target(Lepus3D_Shaders SOURCES ${LEPUS3D_SHADERS_SRC})

# Lepus3D sources
file(GLOB_RECURSE Lepus3D_SRC src/L3D/*.h src/L3D/*.cpp src/L3D/**/*.h src/L3D/**/*.cpp)
list(FILTER Lepus3D_SRC EXCLUDE REGEX ".*(3rdparty)+.*")

add_library(Lepus3D ${Lepus3D_SRC})
source_group(TREE ${CMAKE_SOURCE_DIR}/src/L3D FILES ${Lepus3D_SRC})

# LepusEngine sources
file(GLOB_RECURSE LepusEngine_SRC src/LEngine/*.h src/LEngine/*.cpp src/LEngine/**/*.h src/LEngine/**/*.cpp)
add_library(LepusEngine ${LepusEngine_SRC})
source_group(TREE ${CMAKE_SOURCE_DIR}/src/LEngine FILES ${LepusEngine_SRC})

# LepusSystem sources
file(GLOB_RECURSE LepusSystem_SRC src/LSystem/*.h src/LSystem/*.cpp src/LSystem/**/*.h src/LSystem/**/*.cpp)
add_library(LepusSystem ${LepusSystem_SRC})
source_group(TREE ${CMAKE_SOURCE_DIR}/src/LSystem FILES ${LepusSystem_SRC})

# Unit test example files
file(GLOB_RECURSE LEPUS_TESTS_CONTENT_SRC ${CMAKE_SOURCE_DIR}/tests/Content/*.* ${CMAKE_SOURCE_DIR}/tests/Content/**.*)
add_custom_target(Lepus_Tests_Content SOURCES ${LEPUS_TESTS_CONTENT_SRC})

#add_dependencies(Lepus3D)
# TODO: Get LepusEngine to build add_dependencies(Lepus3D LepusEngine LepusUtility)
add_dependencies(LepusDemo Lepus3D)

# Dependency libraries
## Lepus3D: GL3W (core profile loading)
add_library(GL3W
	3rdparty/gl3w/src/gl3w.c
)
include_directories(3rdparty/gl3w/include)
## Lepus3D: GLFW libraries
find_package(glfw3 CONFIG REQUIRED)

# Assimp
find_package(assimp CONFIG REQUIRED)

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

target_link_libraries(Lepus3D PRIVATE GL3W glfw ${OPENGL_gl_LIBRARY} DearImgui LepusEngine LepusSystem)
target_link_libraries(LepusDemo PRIVATE DearImgui assimp::assimp Lepus3D LepusEngine)

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
add_executable(Lepus3D_Tests
	tests/L3D/GraphicsEngine/GraphicsApiTests.h
		tests/L3D/GraphicsEngine/GraphicsApiTests.cpp
	tests/L3D/GraphicsEngine/GraphicsApiOptionsTests.h
		tests/L3D/GraphicsEngine/GraphicsApiOptionsTests.cpp
)

add_executable(LepusSystem_Tests
	tests/LSystem/IO/FileSystemTests.h
		tests/LSystem/IO/FileSystemTests.cpp
)

add_executable(LepusUtility_Tests
	tests/LUtility/MathTests/MatrixTests.cpp
	tests/LUtility/MathTests/VectorTests.cpp
)

add_custom_command(TARGET LepusSystem_Tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/tests/Content" ${CMAKE_BINARY_DIR}/tests/Content)

target_link_libraries(Lepus3D_Tests GTest::gtest_main Lepus3D)
target_link_libraries(LepusSystem_Tests GTest::gtest_main LepusSystem)
target_link_libraries(LepusUtility_Tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Lepus3D_Tests)
gtest_discover_tests(LepusSystem_Tests)
gtest_discover_tests(LepusUtility_Tests)