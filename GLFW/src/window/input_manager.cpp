#include "input_manager.h"
using namespace Window;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"

namespace Window
{

	InputManager::InputManager()
	{
		keyCallbacks.push_back(InputManager::defaultKeyCallback);
		mouseCallbacks.push_back(InputManager::defaultMouseCallback);

		lastX = 100;
		lastY = 100;
	}

	void InputManager::registerKeyCallback(KeyCallback cb)
	{
		keyCallbacks.push_back(cb);
	}

	void InputManager::registerMouseCallback(MouseCallback cb)
	{
		mouseCallbacks.push_back(cb);
	}

	void InputManager::resetKeyCallbacks()
	{
		keyCallbacks.clear();
		keyCallbacks.push_back(InputManager::defaultKeyCallback);
	}
	void InputManager::resetMouseCallbacks()
	{
		mouseCallbacks.clear();
		mouseCallbacks.push_back(InputManager::defaultMouseCallback);
	}

	void InputManager::processKeyInput(GLFWwindow *window)
	{
		for (auto cb : keyCallbacks)
		{
			cb(window);
		}
	}
	void InputManager::processMouseInput(GLFWwindow *window, double x, double y)
	{
		auto mouseCallbacks = static_cast<Window *>(glfwGetWindowUserPointer(window))
			->getInputManager()->mouseCallbacks;
		for (auto cb : mouseCallbacks)
		{
			cb(window, x, y);
		}
	}

	void InputManager::defaultKeyCallback(GLFWwindow *window)
	{
		if(glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		if(glfwGetKey(window, GLFW_KEY_V))
			std::cout << glfwGetClipboardString(window) << std::endl;
		if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			glfwSetClipboardString(window, "Yaaay I'm a clipboard!");
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			glfwHideWindow(window);
		if (glfwGetKey(window, GLFW_KEY_M))
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
			{
				glfwMaximizeWindow(window);
			}
			else
			{
				glfwIconifyWindow(window);
			}
		}
	}

	void InputManager::defaultMouseCallback(GLFWwindow *window, double x, double y)
	{
		
	}

	void InputManager::scrollCallback(GLFWwindow *window, double x, double y)
	{
		Camera::Camera *camera = static_cast<Window *>(glfwGetWindowUserPointer(window))->getCamera();
		float sensitivity = 0.1f;
		camera->setFov(camera->getFov() - ((float)y * sensitivity));
	}
}