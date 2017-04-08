#pragma once

// Header file to store functions from GLEXT
// some (e.g. glGenBuffers() are not available by default)

#include "glext.h"

PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;

namespace LepusEngine
{
	namespace Lepus3D
	{
		namespace GL
		{
			void Init()
			{
				glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
				glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
				glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
				glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
			}
		}
	}
}