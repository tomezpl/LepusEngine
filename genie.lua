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
	end

	files {
		path.join(PROJ_DIR, "LepusEngine/Source/**.h"),
		path.join(PROJ_DIR, "LepusEngine/Source/**.cpp"),
	}

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
	end

	files {
		path.join(PROJ_DIR, "Lepus3D/Source/**.h"),
		path.join(PROJ_DIR, "Lepus3D/Source/**.cpp"),
	}

	includedirs {
		PROJ_DIR,
		path.join(PROJ_DIR, "include"),
		path.join(os.getenv("SFML_DIR"), "include"),
		path.join(os.getenv("GLEW_DIR"), "include"),
	}

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
	end

	files {
		path.join(PROJ_DIR, "LepusDemo/Source/**.h"),
		path.join(PROJ_DIR, "LepusDemo/Source/**.cpp"),
	}

	includedirs {
		PROJ_DIR,
		path.join(PROJ_DIR, "include"),
		path.join(os.getenv("SFML_DIR"), "include"),
		path.join(os.getenv("GLEW_DIR"), "include"),
	}

	links { "Lepus3D", "LepusEngine" }

	configuration { "windows", "x32" }
		libdirs {
			path.join(os.getenv("GLEW_DIR"), "lib/Release/Win32"),
		}

	configuration { "windows", "x64" }
		libdirs {
			path.join(os.getenv("GLEW_DIR"), "lib/Release/x64"),
		}

	configuration { "windows", "Debug" }
		links { "sfml-graphics-d", "sfml-system-d", "sfml-window-d" }

	configuration { "windows", "Release" }
		links { "sfml-graphics", "sfml-system", "sfml-window" }

	configuration "windows"
		libdirs { path.join(os.getenv("SFML_DIR"), "lib/") }
		links { "opengl32", "glew32" }

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
		postbuildcommands { "mkdir \"$(TargetDir)../../GLSL\"", "xcopy \"$(SolutionDir)\\Lepus3D\\Source\\GLSL\" \"$(TargetDir)\\..\\..\\GLSL\"/C /Y", "copy \"$(SFML_DIR)\\bin\\*.dll\" \"$(TargetDir)\\\" /y", "copy \"$(GLEW_DIR)\\bin\\Release\\$(Platform)\\glew32.dll\" \"$(TargetDir)\" /y" }

	configuration "not windows"
		postbuildcommands { "mkdir -p ./bin/Debug32/GLSL", "mkdir -p ./bin/Debug64/GLSL", "mkdir -p ./bin/Release32/GLSL", "mkdir -p ./bin/Release64/GLSL", "cp GLSL/* ./bin/Debug32/GLSL/", "cp GLSL/* ./bin/Debug64/GLSL/", "cp GLSL/* ./bin/Release32/GLSL/", "cp GLSL/* ./bin/Release64/GLSL/" }
