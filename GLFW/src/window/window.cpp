#include "window.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <stb_image.h>

#include "../definitions.h"
#include "../utils/image.h"

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
			const char *path = "container.jpg";	// Temp
			Utils::Image image(path, 4);

			if(image.isLoaded())
			{
				GLFWimage icon;
				icon.height = image.getHeight();
				icon.width = image.getWidth();
				icon.pixels = image.getPixels();
				glfwSetWindowIcon(window, 1, &icon);
			}
		}

		// Bind to OpenGl
		glfwMakeContextCurrent(window);
		glewInit();

		glfwSetWindowUserPointer(window, &inputManager);
		glfwSetKeyCallback(window, InputManager::processKeyInput);
		glViewport(0, 0, screen->width, screen->height);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

		// Wireframe!
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_PROGRAM_POINT_SIZE);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Seems to limit it at 60fps for now
		glfwSwapInterval(1);

		glfwSetTime(0.0);

		// Do render loop here
	}
}