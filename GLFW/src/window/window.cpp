#include "window.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <stb_image.h>

#include "../definitions.h"

namespace Window
{
	void Window::glfwErrorCallback(int error, const char *description)
	{
		std::cout << description << std::endl;
	}

	Window::Window(std::string name, bool isFullscreen, int width, int height) :
		name(name), isFullscreen(isFullscreen)
	{
		screen = new Screen();
		screen->width = width;
		screen->height = height;
		this->init();
	}

	void Window::init()
	{
		glfwSetErrorCallback(glfwErrorCallback);

		if (!glfwInit())
		{
			std::cerr << "Could not initialize glfw" << std::endl;
			exit(ExitStatus::GlfwError);
		}

		if (isFullscreen)
		{
			this->monitor = glfwGetPrimaryMonitor();
			this->mode = glfwGetVideoMode(monitor);

			screen->width = mode->width;
			screen->height = mode->height;
		}
		else
		{
			this->monitor = NULL;
			this->mode = NULL;
		}

		this->window = glfwCreateWindow(screen->width, screen->height, name.c_str(), monitor, NULL);
		if (!window)
		{
			std::cerr << "Could not create window" << std::endl;
			exit(ExitStatus::WindowCreationError);
		}

		if (!monitor)
		{
			this->monitor = glfwGetPrimaryMonitor();
		}
		this->videoModes = glfwGetVideoModes(monitor, &this->modeCount);

		std::cout << "Available video modes:" << std::endl;
		for (int i = 0; i < modeCount; ++i)
		{
			std::cout << videoModes[i].width << "x" << videoModes[i].height << "@"
				<< videoModes[i].refreshRate << std::endl;
		}

		// Set icon
		{
			GLFWimage icon;

			int iconBpp;
			const char *path = "container.jpg";
			icon.pixels = stbi_load(path, &icon.width, &icon.height, &iconBpp, 4);

			if (!icon.pixels)
			{
				std::cerr << "Could not load " << path << std::endl;
			}
			else
			{
				glfwSetWindowIcon(window, 1, &icon);
				stbi_image_free(icon.pixels);
			}
		}

		glfwMakeContextCurrent(window);

		glfwSetWindowUserPointer(window, &inputManager);
		glfwSetKeyCallback(window, InputManager::processKeyInput);
		glViewport(0, 0, screen->width, screen->height);
	}
}