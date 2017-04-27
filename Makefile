# Directories
ObjDir=./build/obj/
BinDir=./build/bin/
IncDir=./include/

L3DDir=./Lepus3D/Source/
LEDir=./LepusEngine/Source/
DemoDir=./LepusDemo/Source/
ProjDir=`pwd`/

# Compiler
CC=g++ -std=c++11 -I $(IncDir) -I $(ProjDir)
CCL3D=$(CC) -I $(L3DDir)
CCLE=$(CC) -I $(LEDir)

# Dependencies
LibGL=`pkg-config --cflags --libs gl`
LibSFML=`pkg-config --cflags --libs sfml-graphics`
LibGLEW=`pkg-config --cflags --libs glew`

## Dependency sets
## Use these to combine dependencies into one variable
LibGraphics=$(LibGL) $(LibSFML) $(LibGLEW)

mkdirs:
	mkdir -p $(ObjDir)
	mkdir -p $(BinDir)

all: mkdirs glm LepusDemo copyglsl

copyglsl:
	mkdir -p $(ProjDir)GLSL/
	cp $(L3DDir)GLSL/* $(ProjDir)GLSL/

glm:
	$(CC) -c $(IncDir)glm/detail/glm.cpp -o$(ObjDir)glm.o

L3DMaterial:
	$(CCL3D) -c $(L3DDir)Material/Material.cpp -o$(ObjDir)l3d_material.o $(LibGraphics)

L3DMesh:
	$(CCL3D) -c $(L3DDir)Mesh/Mesh.cpp -o$(ObjDir)l3d_mesh.o $(LibGraphics)

L3DRenderEngine:
	$(CCL3D) -c $(L3DDir)RenderEngine/RenderEngine.cpp -o$(ObjDir)l3d_renderengine.o $(LibGraphics)

L3DShader:
	$(CCL3D) -c $(L3DDir)Shader/Shader.cpp -o$(ObjDir)l3d_shader.o $(LibGraphics)

L3DSLProgram:
	$(CCL3D) -c $(L3DDir)Shader/SLProgram.cpp -o$(ObjDir)l3d_slprogram.o $(LibGraphics)

L3DTexture:
	$(CCL3D) -c $(L3DDir)Texture/Texture2D.cpp -o$(ObjDir)l3d_texture2d.o $(LibGraphics)

L3DTransform:
	$(CCL3D) -c $(L3DDir)Transform/Transform.cpp -o$(ObjDir)l3d_transform.o $(LibGraphics)

L3D: L3DMaterial L3DMesh L3DRenderEngine L3DShader L3DSLProgram L3DTexture L3DTransform
	

LELogger:
	$(CC) -c $(LEDir)Logger/Logger.cpp -o$(ObjDir)le_logger.o

LE: LELogger
	

LepusDemo: glm L3D LE
	$(CC) $(DemoDir)main.cpp $(ObjDir)/l3d_*.o $(ObjDir)/le_logger.o -o$(BinDir)/LepusDemo $(LibGraphics)
