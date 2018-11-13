#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <glfw/glfw3.h>

#include "input_manager.h"

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

	public:
		Window(std::string name, int width = 640, int height = 480, bool isFullscreen = false);
		static void glfwErrorCallback(int error, const char *description);

		GLFWwindow *const getWindow() const { return window; }
	private:
		void init();
	};

}

#endif