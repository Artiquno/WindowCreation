#include "input_manager.h"
using namespace Window;

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

void Window::InputManager::processKeyInput(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	auto keyCallbacks = static_cast<Window::InputManager*>(glfwGetWindowUserPointer(window))->_keyCallbacks;
	for (auto cb : keyCallbacks)
	{
		cb(window, key, scancode, action, mods);
	}
}

void InputManager::defaultKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		glm::mat4 view;	// Temp
		if (key == GLFW_KEY_A)
		{
			view = glm::translate(view, glm::vec3(0.1f, 0.0f, 0.0f));
		}
		else if (key == GLFW_KEY_D)
		{
			view = glm::translate(view, glm::vec3(-0.1f, 0.0f, 0.0f));
		}

		if (key == GLFW_KEY_W)
		{
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.1f));
		}
		else if (key == GLFW_KEY_S)
		{
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.1f));
		}

		if (key == GLFW_KEY_E)
		{
			view = glm::translate(view, glm::vec3(0.0f, -0.1f, 0.0f));
		}
		else if (key == GLFW_KEY_Q)
		{
			view = glm::translate(view, glm::vec3(0.0f, 0.1f, 0.0f));
		}
	}
}