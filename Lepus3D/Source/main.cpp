#include "GL\glew.h"
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\OpenGL.hpp"
//#include "GL\glfunc.h"
#include "Material.h"
#include <iostream>

GLclampf clampCol(int value) { return value / 255.0f; } // clamp 8-bit colours to 0-1 range

using namespace LepusEngine;

void render(Lepus3D::Material material)
{
	// Vertex declaration
	GLfloat verts[] = { 
		-0.5f, 0.5f, 0.0f, // Top left
		0.5f, 0.5f, 0.0f, // Top right
		-0.5f, -0.5f, 0.0f, // Bottom left
		0.5f, -0.5f, 0.0f }; // Bottom right
	GLuint indices[] = {
		0, 1, 2,
		1, 3, 2 };

	GLuint vBuf; // vertex buffer object
	GLuint vArr; // vertex array object
	GLuint eBuf; // element buffer object (index buffer)

	glGenBuffers(1, &vBuf);
	glGenBuffers(1, &eBuf);
	glGenVertexArrays(1, &vArr);

	// bind VAO to VBO
	glBindVertexArray(vArr);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // pass vertices to GPU

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // pass elements/indices to GPU

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	}
	glBindVertexArray(0); // unbind VAO (vArr)

	/*// Shading
	Lepus3D::Shader shader("Unlit");

	GLint success;
	GLchar infoLog[512];

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vShaderSource = shader.VShaderSrc();
	glShaderSource(vShader, 1, &vShaderSource, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "Compile VShader: " << infoLog << std::endl;
	}


	/*GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fShaderSource = shader.FShaderSrc();
	glShaderSource(fShader, 1, &fShaderSource, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "Compile FShader: " << infoLog << std::endl;
	}

	GLuint shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vShader);
	glAttachShader(shaderProg, fShader);
	glLinkProgram(shaderProg);

	glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProg, 512, NULL, infoLog);
		std::cout << "Link shader program: " << infoLog << std::endl;
	}

	glUseProgram(shaderProg);*/
	material.Use();
	glBindVertexArray(vArr);
	{
		glDrawElements(GL_TRIANGLES, sizeof(verts) / sizeof(GLfloat), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

int main()
{
	sf::Window window(sf::VideoMode(1280, 720), "Lepus3D", sf::Style::Default);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
#ifdef _DEBUG
		std::cout << "glew failed to load: " << glewGetErrorString(err) << std::endl;
#endif
	}

	Lepus3D::Material testMaterial("Test material", "Unlit");
	testMaterial.SetAttributeF4("Color", new GLfloat[4]{ 0.875f, 0.25f, 0.25f, 1.0f }, 1);

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			switch(ev.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				glViewport(0, 0, ev.size.width, ev.size.height);
				break;
			}
		}

		glClearColor(clampCol(100), clampCol(149), clampCol(237), 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		//Lepus3D::GL::Init();

		render(testMaterial);

		window.display();
	}

	return 0;
}