solution "LepusAll"
	configurations {
		"Debug",
		"Release",
	}
	platforms {
		"x32",
		"x64",
		"Native",
	}
	language "C++"
	startproject "LepusDemo"

PROJ_DIR = path.getabsolute(".")

project "LepusEngine"
	kind "StaticLib"

	if string.startswith(_ACTION, "vs") then
		buildoptions { "/std:c++latest"}
	elseif string.startswith(_ACTION, "gmake") then
		buildoptions { "-std=c++17" }
	end

	files {
		path.join(PROJ_DIR, "LepusEngine/Source/**.h"),
		path.join(PROJ_DIR, "LepusEngine/Source/**.cpp"),
	}

	configuration { "vs*", "Debug" }
		buildoptions { "/Zi" }

	configuration { "Debug", "x32" }
		objdir (path.join(PROJ_DIR, "obj/Debug32/LepusEngine/"))
		targetdir (path.join(PROJ_DIR, "lib/Debug32/"))

	configuration { "Debug", "x64" }
		objdir (path.join(PROJ_DIR, "obj/Debug64/LepusEngine/"))
		targetdir (path.join(PROJ_DIR, "lib/Debug64/"))

	configuration { "Release", "x64" }
		objdir (path.join(PROJ_DIR, "obj/Release64/LepusEngine/"))
		targetdir (path.join(PROJ_DIR, "lib/Release64/"))

	configuration { "Release", "x32" }
		objdir (path.join(PROJ_DIR, "obj/Release32/LepusEngine/"))
		targetdir (path.join(PROJ_DIR, "lib/Release32/"))


project "Lepus3D"
	kind "StaticLib"

	if string.startswith(_ACTION, "vs") then
		buildoptions { "/std:c++latest"}
	elseif string.startswith(_ACTION, "gmake") then
		buildoptions { "-std=c++17" }
	end

	files {
		path.join(PROJ_DIR, "Lepus3D/Source/**.h"),
		path.join(PROJ_DIR, "Lepus3D/Source/**.cpp"),
	}

	includedirs {
		PROJ_DIR,
		path.join(PROJ_DIR, "include"),
	}

	configuration "windows"
		includedirs {
			path.join(os.getenv("GLFW_DIR"), "include"),
			path.join(os.getenv("GLEW_DIR"), "include"),
			path.join(PROJ_DIR, "include\\glm-windows"),
		}

	configuration { "vs*", "Debug" }
		buildoptions { "/Zi" }

	configuration "linux"
		links { "glfw3", "GLEW", "GLU", "GL" }
		includedirs { path.join(PROJ_DIR, "include/glm-linux") }

	configuration { "Debug", "x32" }
		objdir (path.join(PROJ_DIR, "obj/Debug32/Lepus3D/"))
		targetdir (path.join(PROJ_DIR, "lib/Debug32/"))

	configuration { "Debug", "x64" }
		objdir (path.join(PROJ_DIR, "obj/Debug64/Lepus3D/"))
		targetdir (path.join(PROJ_DIR, "lib/Debug64/"))

	configuration { "Release", "x64" }
		objdir (path.join(PROJ_DIR, "obj/Release64/Lepus3D/"))
		targetdir (path.join(PROJ_DIR, "lib/Release64/"))

	configuration { "Release", "x32" }
		objdir (path.join(PROJ_DIR, "obj/Release32/Lepus3D/"))
		targetdir (path.join(PROJ_DIR, "lib/Release32/"))



project "LepusDemo"
	kind "ConsoleApp"

	if string.startswith(_ACTION, "vs") then
		buildoptions { "/std:c++latest"}
	elseif string.startswith(_ACTION, "gmake") then
		buildoptions { "-std=c++17" }
	end

	files {
		path.join(PROJ_DIR, "LepusDemo/Source/**.h"),
		path.join(PROJ_DIR, "LepusDemo/Source/**.cpp"),
	}

	includedirs {
		PROJ_DIR,
		path.join(PROJ_DIR, "include"),
		path.join(os.getenv("GLFW_DIR"), "include"),
		path.join(os.getenv("GLEW_DIR"), "include"),
	}

	links { "Lepus3D", "LepusEngine" }

	configuration "linux"
		links { "glfw3", "GLEW", "GLU", "GL" }
		includedirs { path.join(PROJ_DIR, "include/glm-linux") }

	configuration "windows"
		includedirs { path.join(PROJ_DIR, "include/glm-windows") }

	configuration { "windows", "x32" }
		libdirs {
			path.join(os.getenv("GLEW_DIR"), "lib/Release/Win32"),
		}

	configuration { "windows", "x64" }
		libdirs {
			path.join(os.getenv("GLEW_DIR"), "lib/Release/x64"),
		}

	configuration "windows"
		libdirs { path.join(os.getenv("GLFW_DIR"), "lib-vc2015/") }
		links { "glfw3", "opengl32", "glew32" }

	configuration { "vs*", "Debug" }
		buildoptions { "/Zi" }
		linkoptions { "/DEBUG" }

	configuration { "Debug", "x32" }
		objdir (path.join(PROJ_DIR, "obj/Debug32/LepusDemo/"))
		targetdir (path.join(PROJ_DIR, "bin/Debug32/"))
		debugdir (path.join(PROJ_DIR, "bin/Debug32/"))

	configuration { "Debug", "x64" }
		objdir (path.join(PROJ_DIR, "obj/Debug64/LepusDemo/"))
		targetdir (path.join(PROJ_DIR, "bin/Debug64/"))
		debugdir (path.join(PROJ_DIR, "bin/Debug64/"))

	configuration { "Release", "x64" }
		objdir (path.join(PROJ_DIR, "obj/Release64/LepusDemo/"))
		targetdir (path.join(PROJ_DIR, "bin/Release64/"))
		debugdir (path.join(PROJ_DIR, "bin/Release64/"))

	configuration { "Release", "x32" }
		objdir (path.join(PROJ_DIR, "obj/Release32/LepusDemo/"))
		targetdir (path.join(PROJ_DIR, "bin/Release32/"))
		debugdir (path.join(PROJ_DIR, "bin/Release32/"))

	configuration "windows"
		postbuildcommands { "mkdir \"$(TargetDir)../../Content/GLSL\"", "xcopy \"$(SolutionDir)\\Lepus3D\\Source\\GLSL\" \"$(TargetDir)\\..\\..\\Content\\GLSL\"/C /Y", "copy \"$(GLEW_DIR)\\bin\\Release\\$(Platform)\\glew32.dll\" \"$(TargetDir)\" /y" }

	configuration "not windows"
		postbuildcommands { "mkdir -p ./Content/GLSL", "cp ./Lepus3D/Source/GLSL/* ./Content/GLSL/" }
