# LepusEngine - CMake config
## NOTES:
### Wherever file(GLOB ...) is used to find a library/header, 
### the folder structure for that library is based on Windows binaries available from the library's official source.
### TODO: It is assumed that CMake on Linux will just be able to pick the libraries up from system PATH, but that's yet to be tested.

# min. required CMake version
cmake_minimum_required (VERSION 2.6)

# C++ standard
set(CMAKE_CXX_STANDARD 17)

project(Lepus)

# Directory for dependencies (usually ./build/deps)
set(DEPS_PATH ${CMAKE_BINARY_DIR}/deps)

# Directory for produced binaries
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Engine-wide include directories
include_directories(${CMAKE_SOURCE_DIR}/include src)

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
	src/L3D/Vertex.h
)

## Lepus3D: GLEW includes
file(GLOB GLEW_HEADER_DIRS ${DEPS_PATH}/glew*/*/include)
find_path(GLEW_INCLUDES GL/glew.h ${GLEW_HEADER_DIRS})
include_directories(${GLEW_INCLUDES})

## Lepus3D: GLFW includes
file(GLOB GLFW_HEADER_DIRS ${DEPS_PATH}/glfw-*.bin.*/include)
find_path(GLFW_INCLUDES GLFW/glfw3.h ${GLFW_HEADER_DIRS})
include_directories(${GLFW_INCLUDES})

add_library(Lepus3D ${Lepus3D_SRC})

# LepusEngine sources
## LepusEngine: PhysX includes
file(GLOB PHYSX_HEADER_DIRS ${DEPS_PATH}/PhysX-*/physx/include)
file(GLOB PHYSX_PXSHARED_DIRS ${DEPS_PATH}/PhysX-*/pxshared/include)
find_path(PHYSX_INCLUDES PxPhysXConfig.h ${PHYSX_HEADER_DIRS})
find_path(PHYSX_PXSHARED_INCLUDES foundation/Px.h ${PHYSX_PXSHARED_DIRS})
include_directories(${PHYSX_INCLUDES} ${PHYSX_PXSHARED_INCLUDES})

add_library(LepusEngine 
	src/LEngine/Logger.h
		src/LEngine/Logger/Logger.cpp 
	src/LEngine/Physics.h
		src/LEngine/Physics/Physics.cpp 
		src/LEngine/Physics/PhysicsRigidbody.h
		src/LEngine/Physics/PhysicsRigidbody.cpp
)

add_dependencies(Lepus3D LepusEngine)
add_dependencies(LepusDemo Lepus3D)

# Dependency libraries
## Lepus3D: OpenGL libraries
find_library(OpenGL REQUIRED)
## Lepus3D: GLEW libraries
message(${CMAKE_VS_PLATFORM_NAME})
file(GLOB GLEW_LIB_DIRS ${DEPS_PATH}/glew*/*/lib/Release/${CMAKE_VS_PLATFORM_NAME})
find_library(GLEW_LIBS glew32 ${GLEW_LIB_DIRS})
## Lepus3D: GLFW libraries
file(GLOB GLFW_LIB_DIRS ${DEPS_PATH}/glfw-*.bin.*/lib-vc2015/)
find_library(GLFW_LIBS glfw3 ${GLFW_LIB_DIRS})
## LepusEngine: Windows-specific PhysX and OpenGL libraries. Also sets debug working directory.
if(${CMAKE_HOST_WIN32})
	file(GLOB PHYSX_LIBS_FOLDER_REL ${DEPS_PATH}/PhysX-*/physx/install/*/PhysX/bin/*/release)
	file(GLOB PHYSX_LIBS_FOLDER_DBG ${DEPS_PATH}/PhysX-*/physx/install/*/PhysX/bin/*/debug)

	find_library(PHYSX_LIB_64 PhysX_64 ${PHYSX_LIBS_FOLDER_REL})
	find_library(PHYSX_CHARKINEMATIC_LIB_64 PhysXCharacterKinematic_static_64 ${PHYSX_LIBS_FOLDER_REL})
	find_library(PHYSX_COMMON_LIB_64 PhysXCommon_64 ${PHYSX_LIBS_FOLDER_REL})
	find_library(PHYSX_COOKING_LIB_64 PhysXCooking_64 ${PHYSX_LIBS_FOLDER_REL})
	find_library(PHYSX_EXT_LIB_64 PhysXExtensions_static_64 ${PHYSX_LIBS_FOLDER_REL})
	find_library(PHYSX_FOUNDATION_LIB_64 PhysXFoundation_64 ${PHYSX_LIBS_FOLDER_REL})
	find_library(PHYSX_TASK_LIB_64 PhysXTask_static_64 ${PHYSX_LIBS_FOLDER_REL})
	find_library(PHYSX_VEHICLE_LIB_64 PhysXVehicle_static_64 ${PHYSX_LIBS_FOLDER_REL})

	find_library(PHYSX_LIB_64_DBG PhysX_64 ${PHYSX_LIBS_FOLDER_DBG})
	find_library(PHYSX_CHARKINEMATIC_LIB_64_DBG PhysXCharacterKinematic_static_64 ${PHYSX_LIBS_FOLDER_DBG})
	find_library(PHYSX_COMMON_LIB_64_DBG PhysXCommon_64 ${PHYSX_LIBS_FOLDER_DBG})
	find_library(PHYSX_COOKING_LIB_64_DBG PhysXCooking_64 ${PHYSX_LIBS_FOLDER_DBG})
	find_library(PHYSX_EXT_LIB_64_DBG PhysXExtensions_static_64 ${PHYSX_LIBS_FOLDER_DBG})
	find_library(PHYSX_FOUNDATION_LIB_64_DBG PhysXFoundation_64 ${PHYSX_LIBS_FOLDER_DBG})
	find_library(PHYSX_TASK_LIB_64_DBG PhysXTask_static_64 ${PHYSX_LIBS_FOLDER_DBG})
	find_library(PHYSX_VEHICLE_LIB_64_DBG PhysXVehicle_static_64 ${PHYSX_LIBS_FOLDER_DBG})

	target_link_libraries(LepusEngine debug ${PHYSX_LIB_64_DBG} optimized ${PHYSX_LIB_64})
	target_link_libraries(LepusEngine debug ${PHYSX_CHARKINEMATIC_LIB_64_DBG} optimized ${PHYSX_CHARKINEMATIC_LIB_64})
	target_link_libraries(LepusEngine debug ${PHYSX_COMMON_LIB_64_DBG} optimized ${PHYSX_COMMON_LIB_64})
	target_link_libraries(LepusEngine debug ${PHYSX_COOKING_LIB_64_DBG} optimized ${PHYSX_COOKING_LIB_64})
	target_link_libraries(LepusEngine debug ${PHYSX_EXT_LIB_64_DBG} optimized ${PHYSX_EXT_LIB_64})
	target_link_libraries(LepusEngine debug ${PHYSX_FOUNDATION_LIB_64_DBG} optimized ${PHYSX_FOUNDATION_LIB_64})
	target_link_libraries(LepusEngine debug ${PHYSX_TASK_LIB_64_DBG} optimized ${PHYSX_TASK_LIB_64})
	target_link_libraries(LepusEngine debug ${PHYSX_VEHICLE_LIB_64_DBG} optimized ${PHYSX_VEHICLE_LIB_64})


	file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Release)
	file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/Debug)

	# Set working directory for debugger
	set_property(TARGET LepusDemo PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/Debug")

	file(GLOB PHYSX_BIN_FOLDER_REL ${DEPS_PATH}/PhysX-*/physx/install/*/PhysX/bin/*/release)
	file(COPY ${PHYSX_BIN_FOLDER_REL}/PhysX_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
	file(COPY ${PHYSX_BIN_FOLDER_REL}/PhysXCommon_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
	file(COPY ${PHYSX_BIN_FOLDER_REL}/PhysXCooking_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
	file(COPY ${PHYSX_BIN_FOLDER_REL}/PhysXFoundation_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)

	file(GLOB PHYSX_BIN_FOLDER_DBG ${DEPS_PATH}/PhysX-*/physx/install/*/PhysX/bin/*/debug)
	file(COPY ${PHYSX_BIN_FOLDER_DBG}/PhysX_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)
	file(COPY ${PHYSX_BIN_FOLDER_DBG}/PhysXCommon_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)
	file(COPY ${PHYSX_BIN_FOLDER_DBG}/PhysXCooking_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)
	file(COPY ${PHYSX_BIN_FOLDER_DBG}/PhysXFoundation_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)
	
	file(GLOB GLEW_BIN_DIRS ${DEPS_PATH}/glew*/*/bin/Release/${CMAKE_VS_PLATFORM_NAME})
	file(COPY ${GLEW_BIN_DIRS}/glew32.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
	file(COPY ${GLEW_BIN_DIRS}/glew32.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)

	target_link_libraries(Lepus3D opengl32.lib)
else()
	find_library(OpenGL REQUIRED)
endif()

target_link_libraries(Lepus3D ${GLEW_LIBS} ${GLFW_LIBS} ${OPENGL_LIBRARIES})
target_link_libraries(Lepus3D LepusEngine)
target_link_libraries(LepusDemo Lepus3D LepusEngine)

# Copy content (models, GLSL, etc.)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Content)
add_custom_command(TARGET LepusDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Content" ${CMAKE_BINARY_DIR}/Content)