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

	if (action == GLFW_PRESS)	// Only on (probably) key up
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_V:
			std::cout << glfwGetClipboardString(window) << std::endl;
			break;
		case GLFW_KEY_C:
			glfwSetClipboardString(window, "Yaaay I'm a clipboard!");
			break;
		/*case GLFW_KEY_W:
			changeMode(window);
			break;*/
		/*case GLFW_KEY_R:
			changeRefreshRate(window);
			break;*/
		case GLFW_KEY_H:
			glfwHideWindow(window);
			break;
		case GLFW_KEY_M:
			if (mods & GLFW_MOD_SHIFT)
			{
				glfwMaximizeWindow(window);
			}
			else
			{
				glfwIconifyWindow(window);
			}
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_A:
		case GLFW_KEY_S:
		case GLFW_KEY_D:
		case GLFW_KEY_E:
		case GLFW_KEY_Q:
			break;
		default:
			const char *key_name = glfwGetKeyName(key, scancode);
			std::cout << key << ": ";
			if (key_name)
			{
				std::cout << key_name;
			}
			else
			{
				std::cout << scancode;
			}
			std::cout << std::endl;
			break;
		}
	}
}