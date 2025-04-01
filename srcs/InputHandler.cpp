#include "InputHandler.hpp"

InputHandler::InputHandler()
{}

InputHandler::~InputHandler()
{}

void InputHandler::RegisterCallbacks(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, MouseCallBack);
	glfwSetScrollCallback(window, ScrollCallBack);
	glfwSetMouseButtonCallback(window, MouseButtonCallBack);
	glfwSetKeyCallback(window, KeyCallBack);
}

void InputHandler::Update()
{

}

float InputHandler::GetRotX() const
{
	return (rotX);
}

float InputHandler::GetRotY() const
{
	return (rotY);
}

float InputHandler::GetScale() const
{
	return (scale);
}

/* Private */
void InputHandler::MouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
	if (dragging) {
		std::cout << "Dragging : x : " << xpos << ", y : " << ypos << std::endl;
		float dx = static_cast<float>(xpos - lastX);
		float dy = static_cast<float>(ypos - lastY);

		rotX += dy * 0.005f;
		rotY += dx * 0.005f;
		std::cout << "rot X : " << rotX << " rot Y : " << rotY << std::endl;
		lastX = xpos;
		lastY = ypos;
	}
}

void InputHandler::ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	scale *= (1.0 + yoffset * 0.1);
	if (scale < 0.5)
		scale = 0.5;
	else if (scale > 4)
		scale = 4;
}

void InputHandler::MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &lastX, &lastY);
			dragging = true;
		}
		else if (action == GLFW_RELEASE)
		{
			dragging = false;
		}
	}
}

void InputHandler::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, 1);
			std::cout << "Set Window Close" << std::endl;
		}
		std::cout << key << std::endl;
		if (key == 49)
		{
			if (viewMode == false)
			{
				viewMode = true;
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				viewMode = false;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
	}
}

