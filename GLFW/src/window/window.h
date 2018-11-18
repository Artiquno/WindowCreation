#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "input_manager.h"
#include "../camera/camera.h"

namespace Window
{

	class Window
	{
	private:
		std::string name;
		bool isFullscreen;

		GLFWerrorfun _errorCallback;
		int modeCount;
		const GLFWvidmode *videoModes;
		const GLFWvidmode *mode;
		// ToDo: Add support for multiple monitors
		GLFWmonitor *monitor;
		GLFWwindow *window;
		struct Screen
		{
			int width;
			int height;
		} *screen;

		InputManager inputManager;

		Camera::Camera mainCamera;

	public:
		Window(std::string name, Camera::Camera camera, bool isFullscreen = false, int width = 640, int height = 480);
		static void glfwErrorCallback(int error, const char *description);

		GLFWwindow *const getWindow() const { return window; }
		InputManager * getInputManager() { return &inputManager; }
		Camera::Camera * getCamera() { return &mainCamera; }
	private:
		void init();
	};

}

#endif