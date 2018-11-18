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
		_keyCallbacks.push_back(InputManager::defaultKeyCallback);
	}

	std::vector<KeyCallback> InputManager::getKeyCallbacks()
	{
		return _keyCallbacks;
	}

	void InputManager::registerKeyCallback(KeyCallback cb)
	{
		_keyCallbacks.push_back(cb);
	}

	void InputManager::resetKeyCallbacks()
	{
		_keyCallbacks.clear();
		_keyCallbacks.push_back(InputManager::defaultKeyCallback);
	}

	void InputManager::processKeyInput(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		auto keyCallbacks = static_cast<Window *>(glfwGetWindowUserPointer(window))->getInputManager()->_keyCallbacks;
		for (auto cb : keyCallbacks)
		{
			cb(window, key, scancode, action, mods);
		}
	}

	void InputManager::defaultKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			Window *windowManager = static_cast<Window *>(glfwGetWindowUserPointer(window));
			Camera::Camera *camera = windowManager->getCamera();

			if (key == GLFW_KEY_A)
			{
				camera->translate(glm::vec3(-0.1f, 0.0f, 0.0f));
			}
			else if (key == GLFW_KEY_D)
			{
				camera->translate(glm::vec3(0.1f, 0.0f, 0.0f));
			}

			if (key == GLFW_KEY_W)
			{
				camera->translate(glm::vec3(0.0f, 0.0f, -0.1f));
			}
			else if (key == GLFW_KEY_S)
			{
				camera->translate(glm::vec3(0.0f, 0.0f, 0.1f));
			}

			if (key == GLFW_KEY_E)
			{
				camera->translate(glm::vec3(0.0f, 0.1f, 0.0f));
			}
			else if (key == GLFW_KEY_Q)
			{
				camera->translate(glm::vec3(0.0f, -0.1f, 0.0f));
			}

			if (key == GLFW_KEY_LEFT)
			{
				camera->rotate(0.0f, -2.0f);
			}
			else if (key == GLFW_KEY_RIGHT)
			{
				camera->rotate(0.0f, 2.0f);
			}
		}
	}

}