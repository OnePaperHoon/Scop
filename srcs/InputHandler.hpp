/**
 * @file InputHandler.hpp
 * @author OnePaperHoon (janhan@student.42Seoul.kr)
 * @brief OpenGL에서의 이벤트 핸들러
 * @version 0.1
 * @date 2025-04-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#define GL_SILENCE_DEPRECATION 1
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <iostream>
#include "UVRotation.hpp"

class InputHandler
{
	public:
		InputHandler();
		~InputHandler();

		/**
		 * @brief
		 * Registe All Call Back Function
		 * @param window
		 */
		void		RegisterCallbacks(GLFWwindow* window);
		void		Update();

		/**
		 * @brief OnePaperHoon
		 * set 'bool' reset state
		 * @param value
		 */
		void		SetResetState(bool value);
		/**
		 * @brief OnePaperHoon
		 * Get Scale Value (float)
		 * @return float
		 */
		float		GetScale() const;
		/**
		 * @brief OnePaperHoon
		 * Get Rotate Value Y (float)
		 * @return float
		 */
		float		GetRotY() const;
		/**
		 * @brief OnePaperHoon
		 * Get Rotate Value X (float)
		 * @return float
		 */
		float		GetRotX() const;
		/**
		 * @brief OnePaperHoon
		 * Get Velocity Value X (float)
		 * @return float
		 */
		float		GetVelocityX () const;
		/**
		 * @brief OnePaperHoon
		 * Get Velocity Value Y (float)
		 * @return float
		 */
		float		GetVelocityY () const;
		/**
		 * @brief OnePaperHoon
		 * Get Velocity Value Z (float)
		 * @return float
		 */
		float		GetVelocityZ () const;
		/**
		 * @brief OnePaperHoon
		 * Get Use Texture State
		 * @return true
		 * @return false
		 */
		bool		GetUseTexture() const;
		/**
		 * @brief OnePaperHoon
		 * Get Reset State
		 * @return true
		 * @return false
		 */
		bool		GetResetState() const;
		/**
		 * @brief OnePaperHoon
		 *  Get enum class UVRotation Mode
		 * None, FlipVertical, FlipHorizontal, Rotate90, Rotate180
		 *
		 * @return UVRotation
		 */
		UVRotation	GetRotationMode() const;

	private:
		/**
		 * @brief
		 * GLFW Set Mouse Pos Call Back Function
		 * @param window
		 * @param xpos
		 * @param ypos
		 */
		static void MouseCallBack(GLFWwindow* window, double xpos, double ypos);
		/**
		 * @brief
		 * GLFW Set Scroll Call Back Function
		 * @param window
		 * @param xoffset
		 * @param yoffset
		 */
		static void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
		/**
		 * @brief
		 * GLFW Set Mouse Button Call Back Function
		 * @param window
		 * @param button
		 * @param action
		 * @param mods
		 */
		static void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
		/**
		 * @brief
		 * GLFW Set Key Call Back Function
		 * @param window
		 * @param key
		 * @param scancode
		 * @param action
		 * @param mods
		 */
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
		inline static bool resetPosition = false;
		inline static UVRotation RotationMode = UVRotation::None;
};
