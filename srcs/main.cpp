#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <iostream>
#include "Shader.hpp"
#include <math.h>
#include "Model.hpp"
#include "InputHandler.hpp"

void Versiondisplay()
{
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << version << std::endl;
}

int initglfw(void)
{
	if (!glfwInit())
	{
		std::cerr << "GLFW Init Failed" << std::endl;
		return (-1);
	}
	return (0);
}

void initWindowHint(void)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLFWwindow* CreateWindow(int width, int height, std::string title)
{
	int freambuf_width, freambuf_height;
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (!window)
	{
		std::cerr << "Window Creation Failed" << std::endl;
		glfwTerminate();
		return (NULL);
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &freambuf_width, &freambuf_height);
	glViewport(0, 0, freambuf_width, freambuf_height);

	return (window);
}

int main(void)
{

	InputHandler InputHandler;

	if (initglfw())
		return (-1);
	initWindowHint();
	GLFWwindow* window = CreateWindow(1000, 1000, "Scop");
	if (window == NULL)
	{
		std::cerr << "Error : Widnow Create Faile" << std::endl;
		return (-1);
	}
	InputHandler.RegisterCallbacks(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW Init Failed" << std::endl;
		glfwTerminate();
		return (-1);
	}
	Shader defaultShader (
		"srcs/shaders/shader_vert.glsl", // Vertex Shader
		"srcs/shaders/shader_frag.glsl"); // fragment Shader
	defaultShader.PrintActiveUniforms();
	Model teapot("resources/teapot2.obj", &defaultShader);

	Versiondisplay();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		teapot.SetRotation(InputHandler.GetRotX(), InputHandler.GetRotY());
		teapot.SetScale(InputHandler.GetScale());
		teapot.Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return (0);
}
