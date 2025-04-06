#pragma once

#define GL_SILENCE_DEPRECATION 1
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <iostream>

class InputHandler
{
	public:
		InputHandler();
		~InputHandler();

		/**
		 * @brief
		 * InputHandler에서 관리하고 있는 이벤트 들을 일괄 등록
		 * @param window 등록 시킬 윈도우
		 */
		void RegisterCallbacks(GLFWwindow* window);
		void Update();

		float	GetScale() const;
		float	GetRotY() const;
		float	GetRotX() const;
		float	GetVelocityX () const;
		float	GetVelocityY () const;
		float	GetVelocityZ () const;
		bool	GetUseTexture() const;
	private:
		static void MouseCallBack(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
		static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

		inline static float scale = 1.0f;
		inline static float rotX = 0.0f;
		inline static float rotY = 0.0f;
		inline static float velocityX = 0.0f;
		inline static float velocityY = 0.0f;
		inline static float velocityZ = 0.0f;

		inline static double lastX = 0.0f;
		inline static double lastY = 0.0f;
		inline static bool dragging = false;
		inline static bool viewMode = false;
		inline static bool useTexture = false;
};
