#include "InputHandler.hpp"
#include <csetjmp>
#define MOVE_SPEED 0.0000001

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

float InputHandler::GetScale() const
{
	return (scale);
}

float InputHandler::GetRotX() const
{
	return (rotX);
}

float InputHandler::GetRotY() const
{
	return (rotY);
}

float	InputHandler::GetVelocityX () const
{
	return (velocityX);
}

float	InputHandler::GetVelocityY () const
{
	return (velocityY);
}

float	InputHandler::GetVelocityZ () const
{
	return (velocityZ);
}


bool InputHandler::GetUseTexture() const
{
	return (useTexture);
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
		if (key == 84)
		{
			if (useTexture == false)
				useTexture = true;
			else
				useTexture = false;
		}
		if (key == 87) // W
			velocityZ = -0.1f; // 앞으로 이동
		if (key == 83) // S
			velocityZ = 0.1f;  // 뒤로 이동
		if (key == 65) // A
			velocityX = -0.1f; // 왼쪽으로 이동
		if (key == 68) // D
			velocityX = 0.1f;  // 오른쪽으로 이동
		if (key == 69) // E
			velocityY = 0.1f;  // 위로 이동
		if (key == 81) // Q
			velocityY = -0.1f; // 아래로 이동
		std::cout << "Key : " << key << std::endl;
	}
	if (action == GLFW_RELEASE)
	{
		if (key == 87 || key == 83) velocityZ = 0.0f;
		if (key == 65 || key == 68) velocityX = 0.0f;
		if (key == 81 || key == 69) velocityY = 0.0f;
	}
}

